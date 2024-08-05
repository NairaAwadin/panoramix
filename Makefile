##
## EPITECH PROJECT, 2024
## B-CCP-400-PAR-4-1-panoramix-naira.awadin
## File description:
## Makefile
##

SRC	=	panoramix.c	\
		main.c

OBJ	=	$(SRC:.c=.o)

NAME	=	panoramix

CPPFLAGS =	-I./include/

CFLAGS =	-std=gnu17 -Wall -Wextra

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ) $(CPPFLAGS) $(CFLAGS)

debug:	CFLAGS += -g3
debug:	all

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	re all clear fclean debug
