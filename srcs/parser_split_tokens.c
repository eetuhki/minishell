#include "../incl/minishell.h"

int	split_tokens_fail(t_token *token)
{
	ft_putstr_fd("Failed to store token to token struct.\n", 2);
	free_ptr(token->content);
	return (FAIL);
}

// splits whitespace delimited characters to tokens
int	split_tokens(t_cmd *cmd)
{
	int	i;
	int	start;
	int	index;

	i = 0;
	start = 0;
	index = 0;
	while (cmd->og_str[i])
	{
		while (cmd->og_str[i] && ft_isspace(cmd->og_str[i]))
			i++;
		if (!cmd->og_str[i])
			break;
		start = i;
		while (cmd->og_str[i] && (!ft_isspace(cmd->og_str[i]) || check_quotes(cmd->og_str, i)))
			i++;
		cmd->tokens[index].content = ft_strndup(&cmd->og_str[start], i - start);
		if (!cmd->tokens[index].content)
			return (split_tokens_fail(&cmd->tokens[index]));
		index++;
	}
	return (SUCCESS);
}
