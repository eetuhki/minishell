#include "../incl/minishell.h"

/* // handles heredoc
int	parse_heredoc(t_cmd *cmd, t_redir *redir)
{
	cmd->i += 2; // jump over the '<<'
	if (ft_strchr(QUOTES, cmd->og_str[cmd->i]))
		redir->expand = false;
	parse_filename(cmd, redir, )
	redir->type = HEREDOC;


} */