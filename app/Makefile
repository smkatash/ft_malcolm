# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/11 11:19:19 by kanykei           #+#    #+#              #
#    Updated: 2024/05/17 16:51:25 by kanykei          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================= Target =================================== #
NAME		:= ft_malcolm

# ================================ Sources =================================== #

VPATH		:= src

SRC			:= main.c parser.c utils.c network.c socket.c malcolm.c packet.c

# ================================ Flags ===================================== #
OBJ			:= $(addprefix obj/,$(notdir $(SRC:.c=.o)))
CC			:= gcc
CFLAGS 		:= -Wall -Wextra -Werror #-fsanitize=address

# ============================== Libraries ==================================== #
INCFL		:=	-I./include -I./libs/libft
LIBS		:= 	-L./libs/libft -lft
LIBFT		:=	libs/libft/libft.a

# ================================ Rules ===================================== #
.SILENT:

all: $(NAME) 

$(NAME)		: $(OBJ) $(LIBFT) 
			$(CC) $(OBJ) $(LIBS) -o $(NAME)
			@echo "\033[1;32m ft_malcolm is compiled \033[0m"

obj/%.o		: %.c | obj
			$(CC) $(CFLAGS) $(INCFL) -c $< -o $@

obj			:
			mkdir obj

$(LIBFT):
			- (cd libs/libft && make && make clean)

clean		:
			rm -rf obj
			@echo "deleting object files..."

fclean		: clean
			rm $(NAME)
			@echo "deleting ft_malcolm..."
			- (cd libs/libft && make fclean)

re			: clean all

.PHONY		:	all clean fclean re 
