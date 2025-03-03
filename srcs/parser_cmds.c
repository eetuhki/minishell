#include "../incl/minishell.h"

// checks whether a token is a redir, filename/delimiter after a redir or if it's
// the first token we validate if it's a command/builtin. in every other case we
// assign token type ARG
static void	assign_token_types(t_mini *mini, t_cmd *cmd, t_token *token)
{
	tokenize_redir(token);
	tokenize_in_out(token, cmd->token_count);
	if (!token->type && !cmd->cmd_found)
	{
		check_for_builtins(cmd, token);
		tokenize_cmd(mini, cmd, token);
	}
	if (!token->type)
	{
		token->type = ARG;
		/* if (!cmd->cmd_found)
			err_cmd_not_found(token->content); */
	}
}

// initializes and splits the tokens and then runs the tokens
// one-by-one in a while loop and assigns the token types to every token
static int	tokenize_cmd_string(t_mini *mini, t_cmd *cmd)
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
			assign_token_types(mini, cmd, &cmd->tokens[i]);
		//printf("token:%s at tokens[%d] is type:%d\n", cmd->tokens[i].content, i, cmd->tokens[i].type);
		i++;
	}
	return (SUCCESS);
}

static int expand_tokens(t_mini *mini, t_cmd *cmd)
{
    int		i;

    i = 0;
    while (cmd && cmd->tokens && cmd->tokens[i].content != NULL
		&& !input_is_whitespace(cmd->tokens[i].content))
    {
        if (cmd->tokens[i].type != LIMITER)
        {
            if (expand_variables(mini, &cmd->tokens[i], &cmd->tokens[i].content) == FAIL)
				return (FAIL);
			if (cmd->tokens[i].is_unset)
			{
				cmd->tokens[i].content = NULL;
			}
        }
        i++;
    }
	return (SUCCESS);
}

// goes through the command segments one-by-one in a while loop
int	parse_cmds(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds[i])
	{
		if (tokenize_cmd_string(mini, mini->cmds[i]) == FAIL)
			return (FAIL);
		if (expand_tokens(mini, mini->cmds[i]) == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}
