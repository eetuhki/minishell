#include "../incl/minishell.h"

void	free_tokens(t_cmd *cmd, t_token *token)
{
	int	i;

	if (!token)
		return;
	i = 0;
	while (i < cmd->token_count && &cmd->tokens[i])
	{
		if (token[i].content)
		{
			free(token[i].content);
			token[i].content = NULL;
		}
		i++;
	}
	free(token);
	token = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;
	if (cmd->tokens)
		free_tokens(cmd, cmd->tokens);
	if (cmd->cmd_name)
		free(cmd->cmd_name);
	if (cmd->heredoc_name)
		free(cmd->heredoc_name);
	if (cmd->og_str)
		free(cmd->og_str);
	free(cmd);
	cmd = NULL;
}

void	free_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	if (!mini->cmds)
		return ;
	while (mini->cmds[i])
	{
		if (mini->cmds[i]->in_file)
			close_fd(&mini->cmds[i]->in_file);
		if (mini->cmds[i]->out_file)
			close_fd(&mini->cmds[i]->out_file);
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
	clean_hd_temps(mini);
	free_cmds(mini);
	free_arr(mini->env);
	free_cmds_tbl(mini->cmds_tbl);
	mini->cmds_tbl = NULL;
	free_ptr(mini->pids);
	close_fd(&mini->std_fds[1]);
	close_fd(&mini->std_fds[0]);
	free_ptr(mini);
	rl_clear_history();
	exit(exit_code);
}
