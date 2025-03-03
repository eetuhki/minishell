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
# include <sys/stat.h>

// custom headers & libraries
# include "../libft/incl/libft.h"
# include "macros.h"

extern volatile sig_atomic_t	g_sig;

/* typedef enum e_symbol_type
{
	PIPE,
	INFILE,
	OUTFILE,
	AND,
	OR,
	D_QUOTE,
	S_QUOTE,
}	t_symbol_type; */

typedef enum e_type
{
	ARG,
	BUILTIN,
	CMD,
	INFILE,
	OUTFILE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	LIMITER,
	APPEND,
	APP_OUT,
}	t_type;

typedef struct	s_token
{
	t_type	type;
	int		index;
	char	*content;
	int		is_unset;

}	t_token;

/* typedef struct	s_redir
{
	char	*file;
	int		type;
	bool	expand;
}	t_redir; */

typedef struct s_cmd
{
	t_token	*tokens;
	int		cmd_num;
	int		i;
	int		token_count;
	bool	cmd_found;
	char	*cmd_name;
	char	*heredoc_name;
	int		heredoc_index;
	int		hd_fd;
	int		in_file;
	int		out_file;
	char	*og_str;
	bool	eof_exit;
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
	bool	heredoc_expand;
	bool	in_pipe;
	int		pipes;
	char	*input;
	char	*prev_input;
	char	**env;
	int		exit_code;
	char    ***cmds_tbl;
	int		in_fd;
	int		fd[2];
	int		std_fds[2];
	pid_t	*pids;
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

// free
void	free_and_exit(t_mini *mini);
void	free_arr(char **arr);
void	free_cmd(t_cmd *cmd);
void	free_cmds(t_mini *mini);
void	free_ptr(void *ptr);
void	free_str(char **ptr);
int		free_ptr_fail(void *ptr);
void	free_cmds_tbl(char ***cmds_tbl);
void	free_tokens(t_cmd *cmd, t_token *token);

// builtin functions
int		handle_builtin(t_mini *mini, int i);
int		ft_env(t_mini *mini, int fd);
int		ft_exit(t_mini *mini, char **args);
int		ft_pwd(int fd);
int		ft_cd(t_mini *mini, char **cmd_args);
int		cd_path_error(char *path);

// builtin_echo.c
int		ft_echo(char **args);

// builtin_export.c
int		ft_export_single(t_mini *mini, char *cmd_arg);
int		ft_export(t_mini *mini, char **cmd_args);

// builtin_utils.c
void	update_env_vars(t_mini *mini);
char	**copy_env(char **env, ssize_t count);
int		search_key_in_env(t_mini *mini, char *var);
void	print_err_builtin(char *cmd, char *arg, char *msg);


// builtin_export_utils.c
char	*extract_key(char *cmd_arg);
char	*extract_value(char *cmd_arg);
int		add_env_pair(t_mini *mini, char *key, char *value, bool has_value);

// builtin unset
int		ft_unset(t_mini *mini, char **cmd_args);

// command_table_setup.c
int		prepare_cmd_table(t_mini *mini);

// close.c
void	close_fds(int *fd);
void	close_fd(int fd);

// cmd_path_finder.c
void	check_full_cmd_path(char **cmd_table, t_cmd *cmd, char **env);

// execution
int		builtin_only(char *cmd);
int		check_pid(pid_t pid);
void	child_process(t_mini *mini, int i);
int		cmd_table_size(t_mini *mini);
int		execute(t_mini *mini);
void	exec_fail(t_mini *mini, char *cmd);
int		exec_no_pipes(t_mini *mini);
int		is_there_type(t_mini *mini, t_type type, int i);
int		wait_multi(t_mini *mini);
int		wait_single(t_mini *mini, pid_t pid, int *status);

//execution_redis
int		setup_redirs(t_mini *mini, t_cmd *cmd);
int		handle_redirs(t_cmd *cmd, bool in_pipe, t_mini *mini);
void	reset_std_fds(t_mini *mini);

// heredocs
char	*get_filename(t_cmd *cmd);
int		get_heredoc(t_mini *mini, t_cmd *cmd, t_token *token);
int		handle_heredocs(t_mini *mini);
void	check_fd(int fd);
int		process_heredoc(t_mini *mini, t_cmd *cmd, t_token *token);
void	trim_limiter(t_token *token);
int		setup_heredocs_redir(t_mini *mini);

// parsing
void	assign_token_types(t_mini *mini, t_cmd *cmd, t_token *token);
void	check_for_builtins(t_cmd *cmd, t_token *token);
int		check_quotes(char *input, int limiter);
int		cmd_is_full_path(t_mini *mini, char *cmd);
int		count_pipes(t_mini *mini);
int		count_tokens(t_cmd *cmd);
void	err_cmd_is_dir(char *cmd);
void	err_cmd_not_found(char *cmd);
void	init_cmd_elements(t_cmd *cmd);
int		init_cmd_structs(t_mini *mini);
void	init_token_elements(t_token *token);
int		init_tokens(t_cmd *cmd);
bool	is_redir(int c);
int		parser(t_mini *mini);
int		parse_cmds(t_mini *mini);
char	*skip_whitespace(char *og_str);
int		skip_redir(char *str, int i);
void	skip_space(t_cmd *cmd);
int		skip_word(char *str, int i);
int		split_cmds(t_mini *mini);
int		split_redir(char *str, int i);
int		split_tokens(t_cmd *cmd);
int		split_tokens_fail(t_token *token);
int     tokenize_cmd_segment(t_mini *mini, t_cmd *cmd);
void	tokenize_cmd(t_mini *mini, t_cmd *cmd, t_token *token);
void	tokenize_in_out(t_token *token, int token_count);
void	tokenize_redir(t_token *token);
int		validate_cmd(t_mini *mini, char *cmd);

// print export
int		print_export(t_mini *mini);
void	sort_env(char **env_copy, ssize_t size);

// process cmd files
int		process_cmd_files(t_mini *mini);
void	err_opening_file(char *file_name, int err_no);

// signals
void	sig_init(void);
void	sig_init_child(void);
void	sig_init_heredoc(void);
void	sig_handler(int sig);
void	sig_handler_heredoc(int sig);
void	sig_handler_wait(int sig);
int		heredoc_sigint_hook(void);

// syntax
int		input_is_whitespace(char *input);
int		invalid_chars(t_mini *mini, size_t i, char direction);
int		pipe_is_whitespace(char *input, int i);
char	*redir_special_chars(char direction);
size_t	skip_quotes(char *input);
void	syntax_check(t_mini *mini);
int		syntax_pipes(t_mini *mini);
int		syntax_print_error(char token);
int		syntax_quotes(t_mini *mini);
int		syntax_redir(t_mini *mini, char direction);
int		validate_redir(t_mini *mini, size_t i, char direction);

// validate cmds
void	validate_cmd_access(t_mini *mini, char *cmd);

// variable expansion
int		expand_variables(t_mini *mini, t_token *token, char **str);

// variable expansion utils
int		xp_init(t_expansion *xp);
void	extract_env_var_name(char *input, char *var_name, int size);
bool	update_quote_flags(t_expansion *xp, char input);
int		expand_exit_status(t_mini *mini, t_expansion *xp, char **input);
void	copy_expantion_res(char **str, t_expansion xp);
#endif
