NAME = codexion
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread
SRC = parsing.c main.c display.c coders_utils.c init_mutex.c \
		dongles_utils.c queue_utils.c

OBJ = $(SRC:.c=.o)

all:$(NAME)

%.o : %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME):$(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
