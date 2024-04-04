NAME = minishell
LIBFT_DIR = includes/libft
LIBFT = $(LIBFT_DIR)/libft.a
READLINELIB = -lreadline
CFLAGS = -Wall -Wextra -Werror -g
CC = cc
COMPRESS = ar rcs
RM = rm -rf

SRC_DIR = sources
OBJ_DIR = objects
SRC = $(wildcard $(SRC_DIR)/*.c)						 # we will need to change this in the end (do it on libft too)
OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))

# Colours
GREEN = \033[1;32m
ORANGE = \033[1;33m
RED = \033[1;31m
CYAN = \033[1;36m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(READLINELIB)
	@echo "$(CYAN)make$(RESET)   $@ $(GREEN)[OK]$(RESET)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo "$(ORANGE)$@$(RESET)  $(NAME) $(GREEN)[OK]$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "$(RED)$@$(RESET) $(NAME) $(GREEN)[OK]$(RESET)"

re: fclean all

run: all
	 valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=ignore_readline.supp ./minishell

.PHONY: all clean fclean re
