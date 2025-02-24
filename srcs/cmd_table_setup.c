#include "../incl/minishell.h"

static int	count_cmd_args(t_cmd *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i].content != NULL
			/* && !input_is_whitespace(cmd->tokens[i].content) */)
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

static int	fill_cmd_table(t_cmd *cmd, char	**cmd_table)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < cmd->token_count)
	{
		if ((cmd->tokens[i].type == CMD || cmd->tokens[i].type == BUILTIN
				|| cmd->tokens[i].type == ARG) && cmd->tokens[i].content != NULL
			/* && !input_is_whitespace(cmd->tokens[i].content)*/)
		{
			cmd_table[j] = ft_strdup(cmd->tokens[i].content);
			if (!cmd_table[j])
			{
				ft_putstr_fd("mini: exec: memory allocation failed\n", 2);
				return (FAIL);
			}
			j++;
		}
		i++;
	}
	cmd_table[j] = NULL;
	return (SUCCESS);
}

static char	**build_cmd_table(t_cmd *cmd, char **env)
{
	int		arg_count;
	char	**cmd_table;

	if (!cmd || !cmd->tokens || cmd->token_count == 0)
		return (NULL);
	arg_count = count_cmd_args(cmd);
	cmd_table = malloc(sizeof (char *) * (arg_count + 1));
	if (!cmd_table)
		return (NULL);
	if (fill_cmd_table(cmd, cmd_table) == FAIL)
		return (NULL);
 	/* int t = 0;
    while (t < cmd->token_count)
    {
        printf("Token[%d] = %s and TYPE= %d \n", t, cmd->tokens[t].content, cmd->tokens[t].type);
        t++;
    } */
	check_full_cmd_path(cmd_table, cmd, env);
	return (cmd_table);
}

int	prepare_cmd_table(t_mini *mini)
{
	int	cmds_in_pipe;
	int	i;

	cmds_in_pipe = 0;
	while (mini && mini->cmds && mini->cmds[cmds_in_pipe])
		cmds_in_pipe++;
	if (mini->cmds_tbl)
		free_cmds_tbl(mini->cmds_tbl);
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
	/* int k = 0;
    while (k < cmds_in_pipe && mini->cmds_tbl[k])
    {
        printf("CMD_TABLE[%d] exists\n", k);
        int j = 0;
        while (mini->cmds_tbl[k][j])
        {
            if (mini->cmds_tbl[k][j][0] == ' ')  // Check if it's an empty string
                printf("CMD_TABLE[%d][%d] is an EMPTY STRING\n", k, j);
            else
                printf("CMD_TABLE[%d][%d] = [%s]\n", k, j, mini->cmds_tbl[k][j]);
            j++;
        }

        k++;
    } */
	return (SUCCESS);
}
