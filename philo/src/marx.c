/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   marx.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 04:16:36 by miki              #+#    #+#             */
/*   Updated: 2021/07/12 02:40:01 by miki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Seeing that some philosophers were starving due to the greed of others, Karl
** Marx decided that while philosophers only interpret the world, the point is
** to change it. He left the table and went into the kitchen, overthrew the chef
** and started a revolution.
**
** From now on, Karl Marx will distribute all available food equally among the
** guests! Philosophers will take their forks and eat together in groups. While
** one group is eating, all other groups will wait their turn. When a group is
** finished eating, Marx will issue a new ration card identifying the next
** group that can eat.
**
** The ration_card has been introduced to identify the groups. It is initialized
** in -1, which means no one eats. In our new communist society, philosophers
** must all begin eating together. They will now wait for all of their comrades
** to arrive at the table before they start eating.
**
** Marx will use the free_forks variable to keep track of the number of free
** forks. When a philosopher takes a fork, it will decrement this variable. When
** it releases a fork, it will increment the variable. These decrements and
** increments will, of course, be mutexed.
**
** At any given table there is always a maximum number of philosophers who can
** dine simultaneously. To maximize concurrency, we always want to hit this
** maximum. As it happens, the maximum is always number_of_philosophers / 2.
** This is true EVEN if there is only ONE philosopher at the table (1 / 2 = 0)
** or ZERO (0 / 2 = 0)! Integer divison, of course.
**
** 3 / 2 = 1
**
** f0 SOCRATES f2<--ZAMBRANO-->f3 ARISTOTLE
**
** 4 / 2 = 2
**
** f0<--SOCRATES-->f2 ZAMBRANO f3<--ARISTOTLE-->f4 DE BEAUVOIR
**
** 5 / 2 = 2
**
** f0<--SOCRATES-->f2 ZAMBRANO f3 ARISTOTLE f4<--DE BEAUVOIR-->f5 RAND
**
** If we know the ID number of the first philosopher in an eating group, we can
** derive the ID numbers of the rest of the members of the eating group. Two
** neighbours will never be able to eat together, so a group will consist of
** every second philosopher after the first until max_concurrent_eaters is
** reached.
**
** For example, here we have 7 philosophers.
**
** Max concurrent eaters = 7 / 2 = 3.
**
**	 0 + 2 = 2 + 2 = 4
** |-0-| 1 |-2-| 3 |-4-| 5 | 6
**
** Our table is circular, so after we reach the last diner we need to start
** again at the beginning. That is, 6 + 2 = 1 and 5 + 2 = 0 in this scenario.
**
** = 0	 1 + 2 = 3 + 2 = 5 + 2
** | 0 | 1 | 2 | 3 | 4 | 5 | 6
**
** We can code this by taking philosopher_id MODULO total_philosophers, like so:
**
** 0 + 2 = 2 % 7 = 2.	-|
** 2 + 2 = 4 % 7 = 4.	 | GROUP 1
** 4 + 2 = 6 % 7 = 6.	-|
** 6 + 2 = 8 % 7 = 1.	-|
** 1 + 2 = 3 % 7 = 3.	 | GROUP 2
** 3 + 2 = 5 % 7 = 5.	-|
** 5 + 2 = 7 % 7 = 0.	-|
** 0 + 2 = 2 % 7 = 2.	 | GROUP 3
** 2 + 2 = 4 % 7 = 4.	-|
**
** This alternation pattern ensures all philosophers get a chance to eat.
**
** Having discovered this, Karl Marx instituted the ration_card, where he will
** always indicate the first member of a group. Using that information, each
** philosopher can determine whether they belong to the group whose turn it is
** to eat. While a group is eating, Marx waits. When Marx sees that all forks
** have been returned to the table, he updates the ration_card value within
** ~50 microseconds. Marx is the only person in the People's Republic authorized
** to update the ration_card value, so no mutex is needed.
**
** The philosophers are on ~50 microsecond wait timers monitoring the status of
** pdata->ration_card. When it changes to include them, they leave their while
** within ~50 microseconds to get their forks, which should be readily available
** for them. To give them time to do this, after updating the ration_card value
** Marx will sleep for double that time, 100 microseconds. Then, after the
** philosophers finish sleeping, they will also think for at least 100 extra
** microseconds to ensure that Ayn Rand does not check her ration_card again
** until after Marx has had a chance to update it to the next group's ID,
** because he knows how she can get.
**
** If Marx's policy fails and he causes a famine, he will quietly vacate the
** premises and blame Lenin. If it succeeds, he will take credit.
** 
** In either case, when a philosopher dies, or all philosophers are full, the
** stop flag will be triggered, causing Marx to leave his while loop and
** terminate his thread, which will then be joined into main.
*/

void	*communism(void *progdata)
{
	t_progdata *pdata;
	int			ration;

	pdata = (t_progdata *)progdata;
	pdata->ration_card = -1;
	pdata->free_forks = pdata->number_of_forks;
	pdata->max_concurrent_eaters = pdata->number_of_philosophers / 2;
	ration = 0;
	//espera a los comensales
	while (pdata->comrades_present != pdata->number_of_philosophers)
		usleep(50);
	//a comer!!!!
	while (!pdata->stop)
	{
		if (ration > 0)
			ration--;
		else
			ration = pdata->number_of_philosophers - 1;
		//while (pdata->free_forks <= (pdata->number_of_forks - (pdata->max_concurrent_eaters * 2)) && !pdata->stop)
		//	usleep(50);
		while (pdata->free_forks != pdata->number_of_forks && !pdata->stop)
			{
			}
		pdata->ration_card = ration;
		usleep(100);
		//Marx calcula la actualización de la ration card.
		//
		//Marx espera a que vuelvan a estar todos lo tenedores disponibles.
		//
		//Marx actualiza ration card. Marx debe esperar a que el primer miembro
		//del grupo se dé cuenta y coja sus tenedores antes de volver a mirar
		//el estado de los tenedores. Le da ~100 microsegundos, y cada miembro
		//del grupo comprueba su ration card cada ~50 microsegundos.
		//
		//Marx DEBE llegar a comprobar los tenedores antes de que los que están
		//comiendo vuelvan a mirar su ration card, y todos los del grupo DEBEN haber
		//terminado su comida antes de que el primero vuelva a mirar su ration card
		//para que no vuelva a comer.
		//
		//En el peor caso el primer miembro de un grupo tardará ~50 microsegundos en
		//coger su tenedor y Marx habrá esperado ~50 de sus ~100 microsegundos y le
		//quedarán por esperar ~50 microsegundos. En el mejor caso no tardará nada y
		//a Marx le quedarán por esperar ~100 microsegundos.
		//
		//Cada miembro del grupo debe pasar al menos ~200 microsegundos pensando
		//antes de volver a comprobar su ration card para darle tiempo a Marx a
		//volver a mirar los tenedores y actualizar la ration card para el siguiente
		//grupo.
		//
		//En el mejor caso (cogen su tenedor de inmediato, time_to_eat = 0,
		//time_to_sleep = 0) volverán a pensar de inmediato y a Marx le quedarán 100
		//microsegundos de espera, pero ellos deberán pensar ~400 microsegundos, así
		//que Marx llegará a hacer su comprobación ~300 microsegundos antes de que
		//lleguen. En el peor caso (cogen su tenedor con ~50 microsegundos de demora,
		//time_to_eat = 0, time_to_sleep = 0) volverán a pensar con ~50 microsegundos
		//de demora y a Marx le quedarán 50 microsegundos de espera, pero ellos deberán
		//pensar ~400 microsegundos, así que Marx llegará a hacer su comprobación ~350
		//microsegundos antes de que lleguen.
		//
		//Cada miembro empieza a comer como mucho con ~50 microsegundos de diferencia
		//los unos de los otros, por lo que todos deben terminar de comer con ~50
		//microsegundos de diferencia, dejándole a Marx entre ~50 y ~60 microsegundos
		//de margen para darse cuenta y actualizar la ration card antes de que ninguno
		//vuelva a mirarla.
	}
	return (NULL);
}