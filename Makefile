CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
NAME = minishell

LIBFT_DIR = libft
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJS_DIR = objs

SRC = main.c envp.c signals.c parsing.c builtin_functions/cd.c

# Automatically find the full paths of the source files
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Rule to create .o files while preserving the subdirectory structure
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	@mkdir -p $(dir $@)  # Ensure subdirectories exist
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
