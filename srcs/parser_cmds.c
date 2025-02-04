#include "../incl/minishell.h"

void check_redir(t_token *token)
{
	int	i;
	int	len;

	i = 0;
	if (!token->content && token->type)
		return ;
	len = ft_strlen(token->content);
	while (i < len)
	{
		if (token->content[i] == '<' && ((i + 1 < len)
			&& (token->content[i + 1] == '<')))
			token->type = HEREDOC;
		else if ((i + 1 < len) && (token->content[i] == '>'
			&& token->content[i + 1] == '>'))
			token->type = APPEND;
		else if (token->content[i] == '<' && !token->type)
			token->type = REDIR_IN;
		else if (token->content[i] == '>' && !token->type)
			token->type = REDIR_OUT;
		i++;
	}
	printf("type:%d\n", token->type);
}

int	tokenize_individual_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd || !cmd->og_str)
		return (FAIL);
	if (init_tokens(cmd))
		return (FAIL);
	if (split_tokens(cmd))
		return (FAIL);
	while (i < cmd->token_count)
	{
		if (cmd->tokens[i].content && !cmd->tokens[i].type)
		{
			printf("cmd->tokens[i].content:%s\n", cmd->tokens[i].content);
			check_redir(&cmd->tokens[i]);
		}
		i++;
	}
	return (SUCCESS);
}

int	parse_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		printf("current cmd->og_str:%s\n", mini->cmds[i]->og_str);
		if (tokenize_individual_cmd(mini->cmds[i]) == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}
