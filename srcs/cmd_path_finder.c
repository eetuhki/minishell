#include "../incl/minishell.h"

static char	*get_path_var(char **env)
{
	char	*path;
	char	*ptr_path;
	int		i;

	i = 0;
	path = NULL;
	if (!env)
		return (NULL);
	while (env[i])
	{
		ptr_path = ft_strnstr(env[i], "PATH=", 5);
		if (ptr_path)
		{
			path = ft_substr(env[i], 5, ft_strlen(ptr_path));
			break ;
		}
		i++;
	}
	return (path);
}

static char	*get_valid_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free_ptr(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free_ptr(full_path);
		i++;
	}
	return (NULL);
}

static char	*get_full_cmd_path(char *cmd, char **env)
{
	char	*path_var;
	char	**paths;
	char	*full_path;

	if (!cmd || !env)
		return (NULL);
	path_var = get_path_var(env);
	if (!path_var || path_var[0] == '\0')
		return (NULL);
	paths = ft_split(path_var, ':');
	free_ptr(path_var);
	if (!paths)
		return (NULL);
	full_path = get_valid_cmd_path(paths, cmd);
	free_arr(paths);
	return (full_path);
}

void	check_full_cmd_path(char **cmd_table, t_cmd *cmd, char **env)
{
	char	*full_path;
	(void)cmd;

	full_path = NULL;
	if (cmd_table && cmd_table[0]
		/* && (cmd->tokens[0].type == CMD || cmd->tokens[0].type == ARG ) */)
	{
		full_path = get_full_cmd_path(cmd_table[0], env);
		if (full_path)
		{
			free_ptr(cmd_table[0]);
			cmd_table[0] = full_path;
		}
	}
}
