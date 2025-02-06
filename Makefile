CC = cc
CFLAGS = -Wall -Werror -Wextra -g -Iinclude
NAME = minishell

LIBFT_DIR = libft
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJS_DIR = objs

SRC =  parsing.c \
		execute.c \
		main.c \
		signals.c \
		envp/envp_errors.c \
		envp/envp_to_str.c \
		envp/envp_utils.c \
		envp/ft_getenv.c  \
		envp/ft_setenv.c \
		envp/init_envp.c \
		builtin_functions/cd.c \
		builtin_functions/echo.c \
		builtin_functions/env.c \
		builtin_functions/export.c \
		builtin_functions/pwd.c \
		builtin_functions/unset.c \
		builtin_functions/exit.c \
		expander.c

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
