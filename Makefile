NAME = codexion
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread
SRC = src/parsing.c src/main.c src/display.c src/coders_utils.c src/init_mutex.c \
		src/dongles_utils.c src/queue_utils.c src/schedule_utils.c src/scheduler_org.c \
		src/routine.c src/monitoring.c

OBJ = $(SRC:.c=.o)

all:$(NAME)

%.o : %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME):$(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean:
	@rm -f $(NAME)

re: fclean all clean

.PHONY: clean fclean re all
