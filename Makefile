NAME = Matt_daemon
FLAGS = -Wall -Wextra -Werror -std=c++17
IFLAGS = -I.

BUILD = .build
SRCS =      main.cpp \
			Matt_daemon.cpp \
			Server.cpp

OBJ = $(SRCS:%.cpp=$(BUILD)/%.o)
LOG_DIR = /var/log/matt_daemon
LOCK_DIR = /var/lock

# COLORS
NONE                    = \033[0m
CL_LINE                 = \033[2K
B_RED                   = \033[1;31m
B_GREEN                 = \033[1;32m
B_GREY                  = \033[1;38m
B_MAGENTA               = \033[1;35m
B_CYAN                  = \033[1;36m
B_BLUE                  = \033[1;34m
B_YELLOW                = \033[1;33m
B_WHITE                 = \033[1;37m

all: ${NAME}

$(NAME) : $(OBJ)
	@printf "$(CL_LINE)"
	@c++ $(FLAGS) $(OBJ) -o $@
	@echo "[1 / 1] - $(B_MAGENTA)$@"
	@echo "$(B_GREEN)Compilation done !$(NONE)"

$(BUILD):
	@mkdir $(BUILD)

$(BUILD)/%.o: %.cpp | $(BUILD)
	@printf "$(CL_LINE)Compiling srcs object : $(B_CYAN)$< $(NONE)...\r"
	@c++ $(FLAGS) $(IFLAGS) -o $@ -c $<

clean:
	@${RM} -rf ${BUILD}
	@echo "$(B_GREY)$(BUILD)$(NONE): $(B_YELLOW)Delete$(NONE)"

fclean: clean
	@${RM} ${NAME}
	@echo "$(B_GREY)$(NAME)$(NONE): $(B_YELLOW)Delete$(NONE)"
	@sudo ${RM} -r $(LOG_DIR)
	@sudo ${RM} -r $(LOCK_DIR)

re: fclean all

debug: FLAGS += -DDEBUG
debug: re

.PHONY: all clean fclean re debug
