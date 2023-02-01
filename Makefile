NAME = Matt_daemon
FLAGS = -Wall -Wextra -Werror -std=c++17

SRCS =      main.cpp \
			Matt_daemon.cpp \
			Server.cpp

OBJSRCS = $(SRCS:.cpp=.o)
LOG_DIR = /var/log/matt_daemon
LOCK_DIR = /var/lock

all: ${NAME}

$(NAME) : $(OBJSRCS)
		@c++ -o $(NAME) $(FLAGS) $(OBJSRCS)
		@echo "[\033[32mOK\033[0m] ${NAME} "

%.o: %.cpp
		@c++ $(FLAGS) -o $@ -c $<

clean:
		@${RM} ${OBJSRCS}
		@echo "[\033[31mDELETED\033[0m] Objects"

fclean: clean
		@${RM} ${NAME}
		@echo "[\033[31mDELETED\033[0m] ${NAME} "
		@sudo ${RM} -r $(LOG_DIR)
		@sudo ${RM} -r $(LOCK_DIR)

re: fclean all

debug: FLAGS += -DDEBUG
debug: re

.PHONY: all clean fclean re debug
