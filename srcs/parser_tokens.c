#include "../incl/minishell.h"

void	init_token_elements(t_token *token)
{
	token->type = 0;
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
		if (!ft_isspace(cmd->og_str[i]) && (ft_isspace(cmd->og_str[i + 1])
			|| ft_isspace(cmd->og_str[i + 1] == '\0'))
				&& !check_quotes(cmd->og_str, i))
			token_count++;
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
