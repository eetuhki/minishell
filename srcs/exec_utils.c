#include "../incl/minishell.h"

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
char	**build_cmd_table(t_cmd *cmd)
{
	int		arg_count;
	char	**cmd_table;
	int		i;
	int		j;

	if (!cmd || !cmd->tokens || cmd->token_count == 0)
        return (NULL);
	arg_count = count_cmd_args(cmd);
	cmd_table = malloc(sizeof (char*) * (arg_count + 1));
	if (!cmd_table)
		return (NULL);

	i = 0;
	j = 0;
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i].type == CMD || cmd->tokens[i].type == BUILTIN || cmd->tokens[i].type == ARG)
		{
			if (cmd->tokens[i].content != NULL)
			{
				cmd_table[j] = ft_strdup(cmd->tokens[i].content);
				if (!cmd_table[j])
				{
                	ft_putstr_fd("mini: exec: memory allocation failed\n", 2);
               		return (NULL);
            	}
			}
			j++;
		}
		i++;
	}
	cmd_table[j] = NULL;

	int k;
	k = 0;
	while ( k < arg_count)
		printf("End of cmd_table = %s \n", cmd_table[k++]);
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
        mini->cmds_tbl[i] = build_cmd_table(mini->cmds[i]);
		if (!mini->cmds_tbl[i])
			return (FAIL);
        i++;
    }
    mini->cmds_tbl[i] = NULL;
	return (SUCCESS);
}
