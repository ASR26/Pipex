# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/01 11:37:31 by asolano-          #+#    #+#              #
#    Updated: 2022/07/01 12:46:51 by asolano-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = clang

CFLAGS = -Wall -Wextra -Werror

HEADER = pipex.h

SRC = pipex.c utils.c

OBJ = $(SRC:c=o)

SRCB = bonus.c utils.c

OBJB = $(SRCB:c=o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@rm -f $(OBJ) $(OBJB)

fclean:
	@rm -f $(OBJ) $(OBJB)
	@rm -f $(NAME)

re: fclean all

bonus: $(OBJB)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJB)

.PHONY: clean fclean re bonus
