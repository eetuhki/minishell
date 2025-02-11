#include "../incl/minishell.h"

void	skip_space(t_cmd *cmd)
{
	while (cmd->og_str[cmd->i] && ft_isspace(cmd->og_str[cmd->i]))
		cmd->i++;
}

static int	store_redir(t_cmd *cmd, t_token *token)
{
	int	start;

	if (cmd->og_str[cmd->i] && is_redir(cmd->og_str[cmd->i]))
	{
		start = cmd->i;
		if (is_redir(cmd->og_str[cmd->i])
			&& (cmd->og_str[cmd->i + 1] && is_redir(cmd->og_str[cmd->i + 1])))
			cmd->i++;
		cmd->i++;
		token[token->index].content
			= ft_strndup(&cmd->og_str[start], cmd->i - start);
		if (!token->content)
			return (split_tokens_fail(&cmd->tokens[token->index]));
		// printf("store_redir: token:%s i:%d token[%d]\n", token[token->index].content, cmd->i, token->index);
		token->index++;
	}
	return (SUCCESS);
}


static int	store_word(t_cmd *cmd, t_token *token)
{
	int	start;

	if (cmd->og_str[cmd->i] && !ft_isspace(cmd->og_str[cmd->i]))
	{
		start = cmd->i;
		while (cmd->og_str[cmd->i] && !ft_isspace(cmd->og_str[cmd->i])
			&& !is_redir(cmd->og_str[cmd->i]))
			cmd->i++;
		token[token->index].content
			= ft_strndup(&cmd->og_str[start], cmd->i - start);
		if (!token->content)
			return (split_tokens_fail(&cmd->tokens[token->index]));
		// printf("store_word: token:%s i:%d token[%d]\n", token[token->index].content, cmd->i, token->index);
		token->index++;
	}
	return (SUCCESS);
}

// prints error message and frees the given token str, returns FAIL
int	split_tokens_fail(t_token *token)
{
	ft_putstr_fd("Failed to store token to token struct.\n", 2);
	free_ptr(token->content);
	return (FAIL);
}

// splits whitespace delimited characters to tokens
int	split_tokens(t_cmd *cmd)
{
	cmd->i = 0;
	while (cmd->og_str[cmd->i])
	{
		skip_space(cmd);
		if (!cmd->og_str)
			break;
		if (store_redir(cmd, cmd->tokens) == FAIL)
			return (FAIL);
		skip_space(cmd);
		if (store_word(cmd, cmd->tokens) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
