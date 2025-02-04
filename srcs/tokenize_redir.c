#include "../incl/minishell.h"

void tokenize_redir(t_token *token)
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

/*
// initializes the elements for each separate redir struct
void	init_redir_elements(t_redir *redir)
{
	redir->file = NULL;
	redir->type = 0;
	redir->expand = true;
}

// count the redirections within a single pipe/command segment
int	count_redirs(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->og_str[i])
	{
		if (ft_strchr(REDIRS, cmd->og_str[i]))
		{
			cmd->redir_count++;
			if (ft_strchr(REDIRS, cmd->og_str[i + 1]))
				i++;
		}
		i++;
	}
	return (cmd->redirs);
}

// init redir structs by first counting the needed redirs, allocating memory,
// and finally assign the initial values for the elements of every redir struct
int	init_redirs(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->redir_count = count_redirs(cmd);
	if (!cmd->redir_count)
		return (FAIL);
	cmd->redirs = ft_calloc(cmd->redir_count, sizeof(t_redir));
	if (!cmd->redirs)
	{
		ft_putendl_fd("Memory allocation failed for redirection structs.", 2);
		return (FAIL);
	}
	while (i < cmd->redir_count)
	{
		init_redir_elements(&cmd->redirs[i]);
		i++;
	}
	return (SUCCESS);
}

// check the redir type and call the correct redir functions
int	parse_redir(t_cmd *cmd)
{
	int	len;

	len = ft_strlen(cmd->og_str);
	while (cmd->i < len)
	{
		if ((cmd->i + 1 < len) && (cmd->og_str[cmd->i] == '<'
			&& cmd->og_str[cmd->i + 1] == '<'))
			parse_heredoc(cmd);
		if ((cmd->i + 1 < len) && (cmd->og_str[cmd->i] == '>'
			&& cmd->og_str[cmd->i + 1] == '>'))
			parse_append(cmd);
		else if (cmd->og_str[cmd->i] == '<')
			parse_redir_in(cmd);
		else if (cmd->og_str[cmd->i] == '>')
			parse_redir_out(cmd);
		else
			break ;
	}
	return(cmd->i);
}
 */