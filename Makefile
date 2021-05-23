#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-ghar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/26 16:06:44 by mel-ghar          #+#    #+#              #
#    Updated: 2021/05/23 12:19:25 by mel-ghar         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = ./minishell
SRC = 	*.c

all:$(NAME)
$(NAME):
	gcc $(SRC)  -o $(NAME)
clean:
	rm -f $(NAME)
fclean:clean

re:fclean all

.PHONY: re clean fclean bonus
