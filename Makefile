# Color definitions
RESET   = \033[0m
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
CYAN    = \033[36m

# Variables
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LIBFT_DIR = ./includes/libft
LIBFT    = $(LIBFT_DIR)/libft.a
INCLUDES = -I./includes -I$(LIBFT_DIR)
SRC_DIR  = ./srcs/*
SRC      = $(wildcard $(SRC_DIR)/*.c)
OBJ_DIR  = ./obj
OBJ      = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXEC     = minishell

# Default rule
all: $(LIBFT) $(EXEC)

# Compile the program
$(EXEC): $(OBJ)
	@echo "$(CYAN)🔨  Compiling $(EXEC)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(EXEC) -lreadline
	@echo "$(GREEN)✅  Compilation successful!$(RESET)"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)📂  Creating object file: $@$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build libft library
$(LIBFT):
	@echo "$(CYAN)🔨  Building libft library...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean up object files
clean:
	@echo "$(RED)🧹  Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "$(GREEN)✅  Object files cleaned!$(RESET)"

# Clean up all generated files
fclean: clean
	@echo "$(RED)🗑️   Deleting executable and libft library...$(RESET)"
	@rm -f $(EXEC)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✅  Executable and library deleted!$(RESET)"

# Rebuild everything
re: fclean all
	@echo "$(CYAN)🔄  Rebuilding everything...$(RESET)"

# Phony targets
.PHONY: all clean fclean re