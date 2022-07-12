# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: makhtar <makhtar@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/21 16:36:51 by hawadh            #+#    #+#              #
#    Updated: 2022/07/12 14:24:08 by makhtar          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	cub3d

SRCS		=	trial.c


MLX			=	libmlx.a

MLXDIR		=	./minilibx_opengl

MLXFLG		=	./libmlx.a -framework OpenGL -framework Appkit

OBJS		=	${SRCS:.c=.o}

CC			=	gcc -g3

CFLAGS		=	-Wall -Werror -Wextra

${NAME}:	${OBJS}
			${MAKE} -C ${MLXDIR}
			cp ${MLXDIR}/${MLX} ./
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} ${GNL} ${MLXFLG} -o ${NAME} -Ofast

all:	${NAME}

vg:		${NAME}
		valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3d file.cub

clean:
		rm -rf ${OBJS}
		rm -rf ${MLX}
		${MAKE} clean -C ${MLXDIR}

fclean:	clean
		rm -rf ${NAME}

re: fclean all

.PHONY : all clean re fclean