# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aez-zaou <aez-zaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/17 13:15:07 by aez-zaou          #+#    #+#              #
#    Updated: 2021/06/18 15:06:21 by aez-zaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

GCC			= gcc -Wall -Wextra -Werror -ltermcap

SRCS			= src/minishell.c				\
				src/_builtins.c				\
				src/builtins.c				\
				src/cmdstruct.c				\
				src/ex_one_cmd.c			\
				src/excute.c				\
				src/excute1.c				\
				src/expand.c				\
				src/export.c				\
				src/export_print.c			\
				src/filterComplete.c		\
				src/find_in_path.c			\
				src/ft_env.c				\
				src/ft_exit.c				\
				src/gather_env.c			\
				src/get_cmd.c				\
				src/get_cmd2.c				\
				src/h10_workonline.c		\
				src/h1_gather_env.c			\
				src/h1_sublibft.c			\
				src/h1_workonline.c			\
				src/h2_gather_env.c			\
				src/h2_sublibft.c			\
				src/h2_workonline.c			\
				src/h3_gather_env.c			\
				src/h3_workonline.c			\
				src/h4_gather_env.c			\
				src/h4_workonline.c			\
				src/h5_gather_env.c			\
				src/h5_workonline.c			\
				src/h6_workonline.c			\
				src/h7_workonline.c			\
				src/h8_workonline.c			\
				src/h9_workonline.c			\
				src/he2_newexpan.c			\
				src/he3_newexpan.c			\
				src/he_exonecmd.c			\
				src/he_mult_pipe.c			\
				src/help1_newexpan.c		\
				src/help_cd.c				\
				src/help_ex_one_cmd.c		\
				src/help_excute.c			\
				src/help_excute1.c			\
				src/help_export.c			\
				src/help_export_print.c		\
				src/help_filter_compl.c		\
				src/help_find_in_path.c		\
				src/help_gather_env.c		\
				src/help_newexpan.c			\
				src/help_norm_cmdstruct.c	\
				src/history.c				\
				src/levelbash.c				\
				src/mult_pip_cmd.c			\
				src/newexpan.c				\
				src/random.c				\
				src/readline.c				\
				src/signal.c				\
				src/sublibft.c				\
				src/unset.c					\
				src/update_env_variables.c	\
				src/workonline.c			\


all : $(NAME)

$(NAME) : $(SRCS)
		$(GCC) $(SRCS) -o $(NAME)

clean :
		rm -f $(NAME)

fclean : clean

re: fclean all
