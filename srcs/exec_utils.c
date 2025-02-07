#include "../incl/minishell.h"

static char *get_path_var(char **env)
{
	char *path;
	char *ptr_path;
	int	i;

	i = 0;
	path = NULL;
	if(!env)
		return (NULL);
	while (env[i])
	{
		ptr_path = ft_strnstr(env[i], "PATH=" , 5);
		if (ptr_path)
		{
			path = ft_substr(env[i], 5, ft_strlen(ptr_path));
			break;
		}
		i++;
	}
	return (path);
}

char	*get_valid_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while(paths[i])
	{
		temp =  ft_strjoin(paths[i], "/");
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

char *get_full_cmd_path(char *cmd, char **env)
{
    char	*path_var;
    char	**paths;
    char	*full_path;

    if(!cmd || !env)
        return (NULL);
    path_var = get_path_var(env);
    if(!path_var || path_var[0] == '\0')
        return (NULL);
    paths = ft_split(path_var, ':');
    if(!paths)
		return (NULL);
	full_path = get_valid_cmd_path(paths, cmd);
    free_arr(paths);
    return (full_path);
}

int	count_cmd_args(t_cmd *cmd)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i].content != NULL)
		{
			if (cmd->tokens[i].type == CMD || cmd->tokens[i].type == BUILTIN)
				count++;
			else if (cmd->tokens[i].type == ARG)
				count++;
		}
		i++;
	}                                         
	return (count);
}

int	fill_cmd_table(t_cmd *cmd, char	**cmd_table)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i].type == CMD || cmd->tokens[i].type == BUILTIN 
			|| cmd->tokens[i].type == ARG)
		{
			if (cmd->tokens[i].content != NULL)
			{
				cmd_table[j] = ft_strdup(cmd->tokens[i].content);
				if (!cmd_table[j])
				{
                	ft_putstr_fd("mini: exec: memory allocation failed\n", 2);
               		return (FAIL);
            	}
				j++;
			}
		}
		i++;
	}
	cmd_table[j] = NULL;
	return (SUCCESS);
}

void check_full_cmd_path(char **cmd_table, t_cmd *cmd, char **env)
{
	char	*full_path;

	full_path = NULL;
    if (cmd_table && cmd_table[0] && cmd->tokens[0].type == CMD)
    {
		printf("\ncmd_table[0] before full path = %s\n", cmd_table[0]);
        full_path = get_full_cmd_path(cmd_table[0], env);
        if (full_path)
        {
            free_ptr(cmd_table[0]);
            cmd_table[0] = full_path;
        }
		printf("cmd_table[0] after fullpath = %s\n", cmd_table[0]);
    }
}

// returns cmd_table or NULL
char	**build_cmd_table(t_cmd *cmd, char **env)
{
	int		arg_count;
	char	**cmd_table;

	if (!cmd || !cmd->tokens || cmd->token_count == 0)
        return (NULL);
	arg_count = count_cmd_args(cmd);
	cmd_table = malloc(sizeof (char*) * (arg_count + 1));
	if (!cmd_table)
		return (NULL);

/* 	int t = 0;
	while (t < cmd->token_count)
	{
    	printf("Token[%d] = %s and TYPE= %d \n", t, cmd->tokens[t].content, cmd->tokens[t].type);
		t++;
	} */ 
	if(fill_cmd_table(cmd, cmd_table) == FAIL)
		return (NULL);
	check_full_cmd_path(cmd_table, cmd, env);
	return (cmd_table);
}

int	prep_exec(t_mini *mini)
{
	int cmds_in_pipe;
	int	i;

	cmds_in_pipe = 0;
	while (mini && mini->cmds[cmds_in_pipe])
		cmds_in_pipe++;
	mini->cmds_tbl = malloc(sizeof(char **) * (cmds_in_pipe + 1));
	if (!mini->cmds_tbl)
	{
		ft_putstr_fd("mini: exec: memory allocation failed\n", 2);
		return (FAIL);
	}
	i = 0;
    while (i < cmds_in_pipe)
	{
        mini->cmds_tbl[i] = build_cmd_table(mini->cmds[i], mini->env);
		if (!mini->cmds_tbl[i])
		{
			free_cmds_tbl(mini->cmds_tbl);
			return (FAIL);
		}
        i++;
    }
    mini->cmds_tbl[i] = NULL;

	int k;
	k = 0;
	while (k < cmds_in_pipe && mini->cmds_tbl && mini->cmds_tbl[k])
	{
		int j = 0;
		while (mini->cmds_tbl[k][j])
		{
			printf("CMD_TABLE [%d] = %s \n", k, mini->cmds_tbl[k][j]);
			j++;
		}
		k++;
	}
	return (SUCCESS);
}
