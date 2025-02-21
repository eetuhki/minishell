#include "../incl/minishell.h"

void	free_tokens(t_cmd *cmd, t_token *token)
{
	int	i;

	if (!token)
		return;
	i = 0;
	while (i < cmd->token_count && &cmd->tokens[i])
	{
		free_ptr(token[i].content);
		token[i].content = NULL;
		i++;
	}
	if (token)
	{
		free(token);
		token = NULL;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;
	free_tokens(cmd, cmd->tokens);
	free(cmd->cmd_name);
	free(cmd->heredoc_name);
	free(cmd->og_str);
	free(cmd);
}

void	free_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		free_cmd(mini->cmds[i]);
		i++;
	}
	free(mini->cmds);
	mini->cmds = NULL;
}

void	free_and_exit(t_mini *mini)
{
	int	exit_code;

	exit_code = mini->exit_code;
	free_ptr(mini->input);
	free_cmds(mini);
	free_arr(mini->env);
	free_cmds_tbl(mini->cmds_tbl);
	free_ptr(mini->pids);
	free_ptr(mini);
	exit(exit_code);
}
