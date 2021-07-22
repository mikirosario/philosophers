/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   marx.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 04:16:36 by miki              #+#    #+#             */
/*   Updated: 2021/07/22 05:44:53 by mrosario         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** This function will determine whether the id passed as id is part of the group
** of ids of the id stored in the ration_card.
**
** If the id is a multiple of 2 times the id stored in the ration_card up to
** max_concurrent_eaters out from the id, it is part of the group.
**
** The modulo of the ration_card id divided by the number_of_philosophers is
** taken for circular arithmetic.
**
** For example, here we have 7 philosophers.
**
** Max concurrent eaters = 7 / 2 = 3.
**
**	 0 + 2 = 2 + 2 = 4 + 2 = 6
** |-0-| 1 |-2-| 3 |-4-| 5 | 6
**	 |_______|_______|
**		  GROUP 1
**
** Our table is circular, so after we reach the last diner we need to start
** again at the beginning. That is, 6 + 2 = 1 and 5 + 2 = 0 in this scenario.
**
** = 0	 1 + 2 = 3 + 2 = 5 + 2
** | 0 | 1 | 2 | 3 | 4 | 5 | 6
**		 |_______|_______|
**			  GROUP 2
**
** We code this by using MODULO to do some circular arithmetic, taking
** philosopher_id MODULO total_philosophers as the first member of each group,
** like so:
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
** This creates a precessive pattern that ensures all philosophers have the same
** opportunity to eat.
**
** If the id passed as id is part of the group identified by ration_card, 1 is
** returned. Otherwise, 0 is returned.
*/

int	ration_card(int id, t_progdata *progdata)
{
	int		ration_card = progdata->ration_card;
	size_t	i;

	i = progdata->max_concurrent_eaters;
	while (i--)
	{
		if (ration_card % progdata->number_of_philosophers == id)
			return (1);
		ration_card += 2;
	}
	return (0);
}

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
** at -1, which means no one eats. In our new communist society, philosophers
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
** neighbours will never be able to eat together because they share a fork, so a
** group will always consist of every second philosopher after the first member
** of the group until max_concurrent_eaters is reached.
**
** For example, here we have 7 philosophers.
**
** Max concurrent eaters = 7 / 2 = 3.
**
**	 0 + 2 = 2 + 2 = 4 + 2 = 6
** |-0-| 1 |-2-| 3 |-4-| 5 | 6
**	 |_______|_______|
**		  GROUP 1
**
** Our table is circular, so after we reach the last diner we need to start
** again at the beginning. That is, 6 + 2 = 1 and 5 + 2 = 0 in this scenario.
**
** = 0	 1 + 2 = 3 + 2 = 5 + 2
** | 0 | 1 | 2 | 3 | 4 | 5 | 6
**		 |_______|_______|
**			  GROUP 2
**
** We can code this by using MODULO to do some circular arithmetic, taking
** philosopher_id MODULO total_philosophers as the first member of each group,
** like so:
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
** Note that the % operator in C is only a remainder operator, not a true modulo
** operator, so it returns a negative remainder for negative dividends. If we
** subtract 1 from 0, then -1 % 7 = -1, not 6, like we want. We can easily
** program a modulo function, though, using the formula mod = (-a % b) + b when
** a is a negative integer. In that case, -1 mod 7 = 6. I use pl_mod for this,
** so we can do circular subtraction and addition on the philosopher group.
**
** Having discovered this, Karl Marx instituted the ration_card, where he will
** always indicate the first member of a group. Using that information, each
** philosopher can determine whether they belong to the group whose turn it is
** to eat. While a group is eating, Marx waits. When Marx sees that all forks
** have been returned to the table, he updates the ration_card value within
** ~50 microseconds to signal the next group to eat. Marx is the only person in
** the People's Republic authorized to update the ration_card value, so no mutex
** is needed.
**
** The philosophers are on CPU-bound wait timers continuously monitoring the
** status of pdata->ration_card. When it changes to include them, they leave
** their while to get their forks, which should now be readily available for
** them. To give them time to do this, after updating the ration_card value
** Marx will sleep for ~50 microseconds before checking the forks again. This
** will work as long as the philosophers sleep for some amount of time. Though
** on my Dual Core CPU at home the CPU chokes on the edge cases, with some
** philosophers taking too long to get their forks, on the more bourgeois CPUs
** at school it works like a charm. ;p
**
** If Marx's policy fails and he causes a famine, he will quietly vacate the
** premises and blame Lenin. If it succeeds, he will take credit.
** 
** In either case, when a philosopher dies, or all philosophers are full, the
** stop flag will be triggered, causing Marx to leave his while loop and
** terminate his thread, which will then be joined into main.
**
** Some additional explanation (in Spanish, cause I'm too lazy to translate rn):
**
** Marx divide al grupo de filósofos en grupos de comensales simultáneos.
**
** Marx actualiza la ration card con el id de un miembro del grupo cuando ve que
** todos los tenedores están en la mesa.
**
** A partir del id de cualquier miembro del grupo, se puede calcular los ids de
** todos los miembros del grupo usando la aritmética circular.
**
** Los miembros del grupo salen a recoger sus tenedores (quitándolos de la
** mesa). Marx espera ~50 microsegundos para darles tiempo a hacerlo antes de
** volver a mirar los tenedores.
**
** Marx espera a que vuelvan a estar todos los tenedores disponibles para
** actualizar la ration card para que salga el grupo siguiente. Esto ocurrirá
** cuando el último miembro del grupo anterior termine de comer.
**
** Cada miembro del grupo comprueba su ration card continuamente (depende de la
** CPU), así que todos salen a comer casi al mismo tiempo, por lo que deberían
** terminar de comer también casi al mismo tiempo y volver a mirar su ration
** card casi al mismo tiempo.
**
** Marx DEBE llegar a mirar los tenedores antes de que el primer comensal del
** último grupo vuelva a mirar su ration card, y todos los miembros del grupo
** DEBEN haber terminado su comida antes de que el primero de ellos vuelva a
** mirar su ration card para que no vuelvan a comer los mismos. Como todos miran
** su ration card continuamente mientras esperan, empiezan a comer con muy poca
** diferencia entre sí (en una CPU multi-hilos moderna al menos xD), y también
** acaban de comer con muy poca diferencia (variaría en función del ordenador,
** pero del orden de unos ciclos de la CPU).
**
** Marx duerme ~50 microsegundos después de cambiar la ration card y dar la
** señal de comer, pero el mínimo tiempo para dormir y para comer de un filósofo
** es de 60 milisegundos (6000 * 2 = 12000 microsegundos). Marx, por lo tanto,
** siempre vuelve a mirar los tenedores antes de que vuelvan los filósofos a
** mirar su ration card, y le da tiempo de sobra cuando devuelven sus tenedores
** a señalar al grupo siguiente.
*/

void	*communism(void *progdata)
{
	t_progdata *pdata;
	int			ration;

	pdata = (t_progdata *)progdata;
	pdata->ration_card = -1;
	pdata->free_forks = pdata->number_of_forks;
	pdata->max_concurrent_eaters = pdata->number_of_philosophers / 2;
	ration = 1;
	//espera a los comensales
	while (pdata->comrades_present != pdata->number_of_philosophers)
		{
		}
	//a comer!!!!
	while (!pdata->stop)
	{
		//ration = pl_mod(--ration, pdata->number_of_philosophers);
		if (ration > 0)
			ration--;
		else
			ration = pdata->number_of_philosophers - 1;
		while (pdata->free_forks != pdata->number_of_forks && !pdata->stop)
			{
			}
		pdata->ration_card = ration;
		usleep(50);
	}
	return (NULL);
}
