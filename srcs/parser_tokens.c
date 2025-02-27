#include "../incl/minishell.h"

bool	is_redir(int c)
{
	if (ft_strchr(REDIRS, c))
		return (true);
	return (false);
}

void	init_token_elements(t_token *token)
{
	token->type = 0;
	token->index = 0;
	token->content = NULL;
}

int	count_tokens(t_cmd *cmd)
{
	int	i;
	int	token_count;

	i = 0;
	token_count = 0;
	while (cmd->og_str[i])
	{
		if (is_redir(cmd->og_str[i]) && !check_quotes(cmd->og_str, i))
		{
			token_count++;
			i++;
			if (is_redir(cmd->og_str[i]))
				i++;
		}
		if (!cmd->og_str[i + 1])
			token_count++;
		else if (!ft_isspace(cmd->og_str[i]) && (ft_isspace(cmd->og_str[i + 1])
			|| is_redir(cmd->og_str[i + 1]))
				&& !check_quotes(cmd->og_str, i + 1))
			token_count++;
		// printf("count_tokens: token count:%d at char:%c\n", token_count, cmd->og_str[i]);
		i++;
	}
	return (token_count);
}

int	init_tokens(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->token_count = count_tokens(cmd) + 1;
	if (!cmd->token_count)
		return (FAIL);
	cmd->tokens = ft_calloc(cmd->token_count, sizeof(t_token));
	if (!cmd->tokens)
	{
		ft_putendl_fd("Memory allocation failed for token structs.", 2);
		return (FAIL);
	}
	while (i < cmd->token_count)
	{
		init_token_elements(&cmd->tokens[i]);
		i++;
	}
	return (SUCCESS);
}
