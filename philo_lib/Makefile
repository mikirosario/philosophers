# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miki <miki@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/17 16:01:12 by miki              #+#    #+#              #
#    Updated: 2021/06/25 17:04:06 by miki             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libphilo.a

GRN = \e[1;32m
YEL = \e[1;33m
RED = \e[1;31m
DEL = \e[2K\r
RST = \e[0m

SRC = ./pl_putstr_fd.c ./pl_strlen.c ./pl_isdigit.c ./pl_atoi.c \
./pl_msec_diff.c ./pl_bzero.c \
./check_args.c ./get_next_arg.c

OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Werror -Wextra -I ../includes -c

all: $(NAME)

$(NAME):
	@printf "$(YEL)Compiling library...$(RST)"
	@gcc $(FLAGS) $(SRC)
	@ar -rcs $(NAME) $(OBJ)
	@printf "$(DEL)$(GRN)Compiled library\n$(RST)"

clean:
	@printf "$(YEL)Deleting library object files...$(RST)"
	@rm -f *.o
	@printf "$(DEL)$(GRN)Deleted library object files\n$(RST)"

fclean: clean
	@printf "$(YEL)Deleting library...$(RST)"
	@rm -f $(NAME)
	@printf "$(DEL)$(GRN)Deleted library\n$(RST)"

re: fclean all

.PHONY: all clean fclean re $(GRN) $(YEL) $(RED) $(RST) $(DEL)