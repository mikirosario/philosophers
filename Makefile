# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mrosario <mrosario@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/05 20:32:49 by miki              #+#    #+#              #
#    Updated: 2021/07/10 22:26:09 by mrosario         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

BONUS = philo_bonus

GRN = \e[1;32m
RED = \e[1;31m
YEL = \e[1;33m
DEL = \e[2K\r
RST = \e[0m

P1 = ./$(NAME)/$(NAME)

PB = ./$(BONUS)/$(BONUS)

P1SRC = ./$(NAME)/src/

PBSRC = ./$(BONUS)/src/

P1FILES = $(P1SRC)main.c $(P1SRC)philo_init.c $(P1SRC)iamerror.c \
$(P1SRC)life_cycle.c $(P1SRC)self_identification.c $(P1SRC)is_functions.c \
$(P1SRC)inform.c $(P1SRC)setup.c

PBFILES = $(PBSRC)main_bonus.c  $(PBSRC)philo_init_bonus.c $(PBSRC)inform_bonus.c \
$(PBSRC)iamerror_bonus.c $(PBSRC)life_cycle_bonus.c $(PBSRC)is_functions_bonus.c \
$(PBSRC)setup_bonus.c $(PBSRC)reap_philosophers_bonus.c

PHILO_LIB = ./philo_lib/libphilo.a

FLAGS = -Wall -Werror -Wextra

LIBS = -I ./includes -L ./philo_lib/ -lphilo -pthread

$(NAME): $(PHILO_LIB) $(P1)

$(BONUS): $(PHILO_LIB) $(PB)

$(P1):
	@printf "$(YEL)Compiling program...$(RST)"
	@gcc $(FLAGS) $(P1FILES) -o $(P1) $(LIBS)
	@printf "$(DEL)$(GRN)Compiled program\n$(RST)"

$(PB):
	@printf "$(YEL)Compiling bonus program...$(RST)"
	@gcc $(FLAGS) $(PBFILES) -o $(PB) $(LIBS)
	@printf "$(DEL)$(GRN)Compiled bonus program\n$(RST)"

$(PHILO_LIB):
	@make --no-print-directory -C ./philo_lib
	@make clean --no-print-directory -C ./philo_lib

all: $(NAME)

bonus: $(BONUS)

debug: $(PHILO_LIB)
	#gcc $(FLAGS) $(P1FILES) -g3 -fsanitize=address -o $(P1) $(LIBS)
	@gcc $(FLAGS) $(P1FILES) -g -o $(P1) $(LIBS)

debonus: $(PHILO_LIB)
	@gcc $(FLAGS) $(PBFILES) -g -o $(PB) $(LIBS)

rebug: fclean debug

rebonus: fclean debonus

clean:
	@printf "$(YEL)Deleting program object files...$(RST)"
	@rm -f *.o
	@printf "$(DEL)$(GRN)Deleted program object files\n$(RST)"

cleanlib:
	@make --no-print-directory fclean -C ./philo_lib

fclean: clean cleanlib
	@printf "$(YEL)Deleting program...$(RST)"
	@rm -f $(P1) $(PB)
	@printf "$(DEL)$(GRN)Deleted program\n$(RST)"

re: fclean all

rebug: fclean debug

.PHONY: all debug debonus clean fclean re rebug cleanlib bonus rebonus $(NAME) $(BONUS) $(GRN) $(RED) $(YEL) $(RST) $(DEL) $(P1SRC) $(PBSRC)
