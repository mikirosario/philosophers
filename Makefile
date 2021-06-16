# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/05 20:32:49 by miki              #+#    #+#              #
#    Updated: 2021/06/16 21:12:28 by mrosario         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

P1 = philo_one
P2 = philo_two
P3 = philo_three

P1DIR = ./philo_one/
P2DIR = ./philo_two/
P3DIR = ./philo_three/

P1FILES = $(P1DIR)main.c

CFLAG = -Wall -Werror -Wextra

LIBS = 

$(NAME): $(P1)

$(P1):
	gcc $(CFLAG) $(P1FILES) -o $(P1) $(LIBS)

all: $(NAME)

debug:
	#gcc $(CFLAG) $(P1FILES) -g3 -fsanitize=address -o $(P1) $(LIBS)
	gcc $(CFLAG) $(P1FILES) -g -o $(P1) $(LIBS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(P1)

re: fclean all

rebug: fclean debug

.PHONY: all debug clean fclean re rebug
