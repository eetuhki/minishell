#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <stdbool.h>

// custom headers & libraries
# include "../libft/incl/libft.h"
# include "macros.h"

typedef enum e_symbol_type
{
	PIPE,
	INFILE,
	OUTFILE,
	AND,
	OR,
	D_QUOTE,
	S_QUOTE,
}	t_symbol_type;

typedef struct s_cmd
{
	int		cmd_num;
	char	*cmd_name;
	char	*og_str;
	char	*expd_str;
}	t_cmd;

typedef struct s_expansion
{
	bool	in_single_quote;
	bool	in_double_quote;
	char	*env_value;
	char	*expanded_str;
} t_expansion;

typedef struct s_mini
{
	t_cmd	**cmds;
	int		redir_open;
	int		redir_count;
	int		pipes;
	char	*input;
	char	*prev_input;
	char	**env;
	int		exit_code;
}	t_mini;

// add_history.c
void	addhistory(t_mini *mini);

// arg_check.c
void	arg_check(int ac, char **av);

// env_copy.c
int		env_copy(t_mini *mini, char **env);
size_t	env_count_variables(char **env);

// env_shlvl.c
int		env_shlvl(t_mini *mini);

// env_utils.c
int		env_find_index(t_mini *mini, char *var);
char	*env_get_var(t_mini *mini, char *var);
int		env_is_key_valid(char *var);
char	**env_realloc(t_mini *mini, int size);
int		env_set_var(t_mini *mini, char *var, char *new_val);

// free.c
void	free_arr(char **arr);
void	free_ptr(void *ptr);

// builtin functions
void	handle_builtin(t_mini *mini);
void	ft_env(t_mini *mini, int fd);
void	ft_exit(t_mini *mini, char **args);
void	ft_pwd(int fd);
void	ft_cd(t_mini *mini, char *path);

// builtin_export.c
void	ft_export(t_mini *mini, char *cmd_arg);

// builtin_utils.c
void	update_env_vars(t_mini *mini);
char	**copy_env(char **env, ssize_t count);

// builtin_export_utils.c
char	*extract_key(char *cmd_arg);
char	*extract_value(char *cmd_arg);
int		add_env_pair(t_mini *mini, char *key, char *value, bool has_value);
void	sort_env(char **env_copy, ssize_t size);

// parsing
int		check_quotes(char *input, int limiter);
int		count_pipes(t_mini *mini);
int		handle_individual_cmd(t_mini *mini, t_cmd *cmd, int start, int end);
void	init_cmd_elements(t_cmd *cmd);
int		init_cmd_structs(t_mini *mini);
int		parser(t_mini *mini);
char	*skip_whitespace(char *og_str);
int		split_cmds(t_mini *mini);

// signals
void	sig_init(void);
void	sig_handler_sigint(int sig);

// syntax
int		input_is_whitespace(char *input);
int		invalid_chars(t_mini *mini, size_t i, char direction);
char	*redir_special_chars(char direction);
size_t	skip_quotes(char *input);
void	syntax_check(t_mini *mini);
void	syntax_fail(t_mini *mini);
int		syntax_pipes(t_mini *mini);
int		syntax_print_error(char token);
int		syntax_quotes(t_mini *mini);
int		syntax_redir(t_mini *mini, char direction);
int		validate_redir(t_mini *mini, size_t i, char direction);

// variable expansion
void	expand_variables(t_mini *mini, char *str, char **expanded);

// variable expansion utils
int		xp_init(t_expansion *xp);
void	extract_env_var_name(char *input, char *var_name, int size);
bool	update_quote_flags(t_expansion *xp, char input);
int		expand_exit_status(t_mini *mini, t_expansion *xp, char **input);

#endif
