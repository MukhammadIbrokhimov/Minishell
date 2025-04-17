# Name of the executable
NAME = sadaf

# Directories
SRC_DIR = srcs
OBJ_DIR = obj
INC_DIR = includes
BUILTINS_DIR = $(SRC_DIR)/builtins
EXECUTION_DIR = $(SRC_DIR)/execution
PARSING_DIR = $(SRC_DIR)/parsing
UTILS_DIR = $(SRC_DIR)/utils

# Source files
SRC_FILES = main.c shell.c
BUILTINS_FILES = builtins.c
EXECUTION_FILES = runcmd.c path.c signals.c heredoc.c
PARSING_FILES = parser.c cmd_constructors.c expansion.c
UTILS_FILES = utils.c env_utils.c string_utils.c

# Add directory prefixes
SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
       $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES)) \
       $(addprefix $(EXECUTION_DIR)/, $(EXECUTION_FILES)) \
       $(addprefix $(PARSING_DIR)/, $(PARSING_FILES)) \
       $(addprefix $(UTILS_DIR)/, $(UTILS_FILES))

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I$(INC_DIR)

# Find readline library
READLINE_DIR = $(shell find /usr -name readline 2>/dev/null | grep include | head -n 1 | xargs dirname 2>/dev/null)
READLINE_LIB = $(shell find /usr -name libreadline.a 2>/dev/null | head -n 1 | xargs dirname 2>/dev/null)

ifeq ($(READLINE_DIR),)
	READLINE_DIR = /usr/include
endif

ifeq ($(READLINE_LIB),)
	READLINE_LIB = /usr/lib
endif

INC += -I$(READLINE_DIR)
LIBS = -L$(READLINE_LIB) -lreadline -lncurses

# Rules
all: directories $(NAME)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/utils

# Compile the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Compile each source file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Clean object files
clean:
	rm -rf $(OBJ_DIR)

# Clean all generated files
fclean: clean
	rm -f $(NAME)
	rm -f readline_suppress.supp

# Rebuild
re: fclean all

# Create readline suppression file for valgrind
readline_suppress.supp:
	@echo "{" > readline_suppress.supp
	@echo "    Readline" >> readline_suppress.supp
	@echo "    Memcheck:Leak" >> readline_suppress.supp
	@echo "    ..." >> readline_suppress.supp
	@echo "    fun:readline" >> readline_suppress.supp
	@echo "}" >> readline_suppress.supp
	@echo "{" >> readline_suppress.supp
	@echo "    AddHistory" >> readline_suppress.supp
	@echo "    Memcheck:Leak" >> readline_suppress.supp
	@echo "    ..." >> readline_suppress.supp
	@echo "    fun:add_history" >> readline_suppress.supp
	@echo "}" >> readline_suppress.supp

# Memory leak check
leak_check: $(NAME) readline_suppress.supp
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline_suppress.supp ./$(NAME)

# Full memory leak check
full_leak_check: $(NAME) readline_suppress.supp
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=readline_suppress.supp ./$(NAME)

.PHONY: all clean fclean re directories leak_check full_leak_check