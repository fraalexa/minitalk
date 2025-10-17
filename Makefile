# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fraalexa <fraalexa@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 16:19:01 by fraalexa          #+#    #+#              #
#    Updated: 2025/10/17 11:48:11 by fraalexa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Werror -Wextra
NAME1 = server
NAME2 = client
INC = ./includes/minitalk.h

all : $(NAME1) $(NAME2)

$(NAME1) :
	cc $(CFLAGS) server.c -o $(NAME1)
$(NAME2) :
	cc $(CFLAGS) client.c -o $(NAME2)

clean :
	@echo "Nothing to clean (no .o files)."
	
fclean : clean
	rm -rf server client
	
re : fclean all

.PHONY : all clean fclean re

