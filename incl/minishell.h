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

typedef struct s_token
{
	t_type	type;
	int		index;
	char	*content;
	bool	is_var_empty;
	bool	is_var_only;

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
}	t_expansion;

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
	char	***cmds_tbl;
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
char	*env_get_var(t_mini *mini, char *var);
int		env_set_var(t_mini *mini, char *var, char *new_val);

// free
void	free_and_exit(t_mini *mini);
void	free_arr(char **arr);
void	free_cmd(t_cmd *cmd);
void	free_cmds(t_mini *mini);
void	free_env(t_mini *mini);
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
int		valid_key(char *cmd_arg, bool is_unset);

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
void	close_fd(int *fd);

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
void	handle_fds(t_mini *mini, pid_t pid, int i);
int		wait_multi(t_mini *mini);
int		wait_single(t_mini *mini, pid_t pid, int *status);

//execution_redis
int		setup_redirs(t_mini *mini, t_cmd *cmd);
int		handle_redirs(t_cmd *cmd, bool in_pipe, t_mini *mini);
void	reset_std_fds(t_mini *mini);
int		handle_infile_redir(t_cmd *cmd);
int		handle_outfile_redir(t_cmd *cmd);

// heredocs
int		handle_heredocs(t_mini *mini);
void	check_fd(int fd);
void	clean_hd_temps(t_mini *mini);
void	trim_limiter(t_token *token);
int		setup_heredocs_redir(t_mini *mini);

// parsing
void	check_for_builtins(t_cmd *cmd, t_token *token);
int		check_quotes(char *input, int limiter);
void	err_cmd_is_dir(char *cmd);
void	err_cmd_not_found(char *cmd);
int		init_tokens(t_cmd *cmd);
bool	is_redir(int c);
int		parser(t_mini *mini);
int		parse_cmds(t_mini *mini);
int		skip_redir(char *str, int i);
int		skip_word(char *str, int i);
int		split_cmds(t_mini *mini);
int		split_redir(char *str, int i);
int		split_tokens(t_cmd *cmd);
int		tokenize_cmd_segment(t_mini *mini, t_cmd *cmd);
void	tokenize_cmd(t_mini *mini, t_cmd *cmd, t_token *token);
void	tokenize_in_out(t_token *token, int token_count);
void	tokenize_redir(t_token *token);

// print export
int		print_export(t_mini *mini);

// process cmd files
int		process_cmd_files(t_mini *mini);
void	err_opening_file(char *file_name, int err_no);

// signals
void	sig_init(void);
void	sig_init_child(void);
void	sig_init_heredoc(void);
int		heredoc_sigint_hook(void);

// syntax
int		input_is_whitespace(char *input);
int		pipe_is_whitespace(char *input, int i);
size_t	skip_quotes(char *input);
int		syntax_check(t_mini *mini);
int		syntax_pipes(t_mini *mini);
int		syntax_print_error(char token);
int		syntax_quotes(t_mini *mini);
int		syntax_redir(t_mini *mini, char direction);

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
