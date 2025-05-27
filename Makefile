#COLORS
GREEN       = \033[0;32m
RED         = \033[0;31m
BOLD        = \033[1m
NORMAL      = \033[0m
UP          = \033[A
CUT         = \033[K

#OUTPUT
NAME        = philo

HEADS_LIST  = philo.h

SRCS        =	checks.c \
				checks2.c \
				error.c \
				init_end.c \
				init_end2.c \
				init_end3.c \
				main.c \
				routine.c \
				utils.c \
				take_forks.c \

OBJS        := ${SRCS:.c=.o}

#COMMANDS
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g # -g is for debugging symbols
LDFLAGS     = -pthread
RM          = rm -rf

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

all: ${NAME}

#Compile normal executable
${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) ${OBJS} -o ${NAME} $(LDFLAGS) # Added $(LDFLAGS) for -pthread
	@echo "$(GREEN)[$(BOLD)OK$(NORMAL)$(GREEN)]$(NORMAL) created and compiled object files"
	@echo "$(GREEN)[$(BOLD)OK$(NORMAL)$(GREEN)]$(NORMAL) $(BOLD)$(NAME)$(NORMAL) is ready"

#Clean obj files
clean:
	@${RM} ${OBJS}
	@echo "$(GREEN)[$(BOLD)OK$(NORMAL)$(GREEN)]$(NORMAL) object files deleted"

#Clean objs files and name
fclean: clean
	@${RM} ${NAME}
	@echo "$(GREEN)[$(BOLD)OK$(NORMAL)$(GREEN)]$(NORMAL) $(BOLD)$(NAME)$(NORMAL) deleted"

re: fclean all

valgrind: $(NAME)
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)
helgrind: $(NAME)
		valgrind --tool=helgrind --history-level=full ./$(NAME) $(ARGS)

.PHONY: all clean fclean re
