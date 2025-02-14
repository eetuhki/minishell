NAME = minishell

SRC_DIR = srcs
SRCS = $(addprefix $(SRC_DIR)/, \
		add_history.c \
		arg_check.c \
		builtin_echo.c \
		builtin_exit.c \
		builtin_functions.c \
		builtin_export.c \
		builtin_export_utils.c \
		builtin_utils.c \
		env_copy.c \
		env_shlvl.c \
		env_utils.c \
		error.c \
		free.c \
		heredocs.c \
		heredocs_utils.c \
		minishell.c \
		parser.c \
		parser_cmds.c \
		parser_quotes.c \
		parser_split_cmds.c \
		parser_split_tokens.c \
		parser_tokens.c \
		signal.c \
		syntax.c \
		syntax_error.c \
		syntax_pipes.c \
		syntax_quotes.c \
		syntax_redir.c \
		syntax_utils.c \
		tokenize_cmd.c \
		tokenize_redir.c \
		expansion.c \
		expansion_utils.c \
		cmd_table_setup.c \
		cmd_path_finder.c \
		process_cmd_files.c)

OBJ_DIR = objs
OBJECTS	= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./libft/incl -I./incl

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

RM = rm -rf

GREEN = \033[0;92m
BLUE = \033[0;94m
END_COLOR = \033[0m

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) -o $(NAME) -lreadline
	@echo "$(GREEN)Minishell compiled$(END_COLOR)"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(BLUE)Cleaned minishell object files$(END_COLOR)"
	@make -C $(LIBFT_DIR) clean

fclean:
	@$(RM) $(NAME) $(OBJ_DIR)
	@echo "$(BLUE)Fully cleaned minishell$(END_COLOR)"
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
