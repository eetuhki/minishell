#include "../incl/minishell.h"

void	check_for_builtins(t_cmd *cmd, t_token *token)
{
	if (!token->content || token->type || cmd->cmd_found)
		return ;
	if (!ft_strcmp(token->content, "echo") || !ft_strcmp(token->content, "cd")
		|| !ft_strcmp(token->content, "pwd") || !ft_strcmp(token->content, "export")
		|| !ft_strcmp(token->content, "exit") || !ft_strcmp(token->content, "unset")
		|| !ft_strcmp(token->content, "env"))
	{
		token->type = BUILTIN;
		cmd->cmd_found = true;
	}
}

static int	cmd_is_full_path(t_mini *mini, char *cmd)
{
	if (!access(cmd, F_OK))
	{
		if (open(cmd, __O_DIRECTORY) != -1)
		{
			//err_cmd_is_dir(cmd);
			mini->exit_code = 126;
			return (FAIL);
		}
		if (access(cmd, X_OK))
			return (FAIL);
		return (SUCCESS);
	}
	return (FAIL);
}

static int	validate_path(char *path_dir, char *cmd)
{
	char	*full_path;
	char	*add_slash;

	add_slash = ft_strjoin(path_dir, "/");
	full_path = ft_strjoin(add_slash, cmd);
	free_ptr(add_slash);
	if (!access(full_path, F_OK | X_OK))
	{
		free_ptr(full_path);
		return (SUCCESS);
	}
	free_ptr(full_path);
	return (FAIL);
}

// looks into the env to validate the command, returns SUCCESS if
// valid cmd or FAIL
static int	validate_cmd(t_mini *mini, char *cmd)
{
	int		i;
	char	**path_dirs;

	i = -1;
	while (mini->env[++i] && !ft_strnstr(mini->env[i], "PATH=", 5))
		;
	if (!mini->env[i] || !mini->env[i][5])
		return (FAIL);
	path_dirs = ft_split(mini->env[i] + 5, ':');
	i = -1;
	while (path_dirs[++i])
	{
		if (validate_path(path_dirs[i], cmd) == SUCCESS)
		{
			free_arr(path_dirs);
			return (SUCCESS);
		}
	}
	free_arr(path_dirs);
	return (FAIL);
}

// we check if the token is referring to a valid command (or is the full path)
// we then either assign type CMD to the token or return.
void	tokenize_cmd(t_mini *mini, t_cmd *cmd, t_token *token)
{
	if (!token->content || token->type || cmd->cmd_found)
		return ;
	if (ft_strchr(token->content, '/'))
	{
		if (cmd_is_full_path(mini, token->content))
			return ;
	}
	if (validate_cmd(mini, token->content) == FAIL)
		return ;
	token->type = CMD;
	cmd->cmd_found = true;
}
