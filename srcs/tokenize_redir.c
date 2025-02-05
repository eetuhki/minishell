#include "../incl/minishell.h"

// if the current token is a redir, the next token should be either
// a filename or heredoc limiter
void	tokenize_in_out(t_token *token, int token_count)
{
    int	i;

	i = 0;
    while (i < token_count)
    {
        if (token[i].type == REDIR_IN || token[i].type == REDIR_OUT ||
            token[i].type == APPEND || token[i].type == HEREDOC)
        {
            if (i + 1 < token_count && !token[i + 1].type)
            {
                if (token[i].type == REDIR_IN)
                    token[i + 1].type = INFILE;
                else if (token[i].type == REDIR_OUT)
                    token[i + 1].type = OUTFILE;
				else if (token[i].type == APPEND)
					token[i + 1].type = APP_OUT;
                else if (token[i].type == HEREDOC)
                    token[i + 1].type = LIMITER;
            }
        }
        i++;
    }
}

// if the current token has redirect symbols ('<', '>')
// we check the redirect type and assing the corresponding token type
void	tokenize_redir(t_token *token)
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
}
