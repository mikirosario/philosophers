/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   marx.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miki <miki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 04:16:36 by miki              #+#    #+#             */
/*   Updated: 2021/07/13 15:13:28 by miki             ###   ########.fr       */
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
		//ration = pl_mod(--ration, pdata->number_of_philosophers);
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
		usleep(50);
		//Marx calcula la actualización de la ration card.
		//
		//Marx espera a que vuelvan a estar todos lo tenedores disponibles.
		//
		//Marx actualiza ration card. Marx debe esperar a que el primer miembro
		//del grupo se dé cuenta y coja sus tenedores antes de volver a mirar
		//el estado de los tenedores. Marx espera ~50 microsegundos. Cada miembro
		//del grupo comprueba su ration card continuamente, así que todos salen
		//a comer casi al mismo tiempo, por lo que deberían terminar de comer también
		//casi al mismo tiempo y volver a mirar su ration card casi al mismo tiempo.
		//
		//Marx DEBE llegar a mirar los tenedores antes de que los que están
		//comiendo vuelvan a mirar su ration card, y todos los del grupo DEBEN haber
		//terminado su comida antes de que el primero vuelva a mirar su ration card
		//para que no vuelvan a comer los mismos. Como todos miran su ration card
		//continuamente mientras esperan empiezan a comer con muy poca diferencia
		//entre sí, y también acaban con muy poca diferencia (variaría en función
		//del ordenador, pero del orden de unos ciclos de la CPU).
		//
		//Marx duerme ~50 microsegundos después de cambiar la ration card y dar la
		//señal de comer, pero el mínimo tiempo para dormir y para comer de un filósofo
		//es de 60 milisegundos (6000 * 2 = 12000 microsegundos). Marx, por lo tanto,
		//siempre vuelve a mirar los tenedores antes de que vuelvan los filósofos a
		//mirar su ration card, y le da tiempo de sobra cuando devuelvan sus tenedores
		//a señalar al grupo siguiente.
	}
	return (NULL);
}