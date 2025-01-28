CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = minishell

LIBFT_DIR = libft
LDFLAGS = -L$(LIBFT_DIR) -I$(LIBFT_DIR) -lft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
SRC = main.c builtin_functions/cd.c builtin_functions/echo.c builtin_functions/env.c builtin_functions/exit.c builtin_functions/unset.c builtin_functions/export.c envp_functions/envp.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re