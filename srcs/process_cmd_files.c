#include "../incl/minishell.h"

void	err_opening_file(char *file_name, int err_no)
{
	if (file_name)
	{
		ft_putstr_fd("mini: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(err_no), 2);
		ft_putstr_fd("\n", 2);
	}
}

int	open_infile(char *file_name, int *in_file)
{
	if (*in_file != -1)
	{
		printf("closing infile[%d]. [%s] <- new_file\n", *in_file, file_name);
		close(*in_file);
	}
	*in_file = open(file_name, O_RDONLY);
	if (*in_file == -1)
	{
		err_opening_file(file_name, errno);
		return (FAIL);
	}
	return (SUCCESS);
}

int	open_outfiles(char *file_name, int *out_file, int type)
{
	int temp_fd;
	int flags;

	temp_fd = open(file_name, __O_DIRECTORY);
	if (temp_fd != -1)
	{
		close(temp_fd);
		err_cmd_is_dir(file_name);
		return (FAIL);
	}
	if (*out_file != -1)
	{
		printf("closing prev outfile [%s] <- newfile \n", file_name);
		close(*out_file);
	}
	if (type == OUTFILE)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		flags = O_CREAT | O_WRONLY | O_APPEND;
	*out_file = open(file_name, flags, 0644);
	if (*out_file == -1)
	{
		err_opening_file(file_name, errno);
		return (FAIL);
	}
	return (SUCCESS);
}

int	handle_cmd_files(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd && cmd->tokens && cmd->tokens[i].content)
	{
/* 		if (cmd->tokens[i].type == LIMITER)
		{
			if (cmd->hd_fd != -1)
				cmd->in_file = cmd->hd_fd;
		} */
		if (cmd->tokens[i].type == INFILE)
		{
			if (open_infile(cmd->tokens[i].content, &cmd->in_file) == FAIL)
				return (FAIL);
			printf("INFILE opened [%d] , %s \n", cmd->in_file, cmd->tokens[i].content);
		}
		else if (cmd->tokens[i].type == OUTFILE || cmd->tokens[i].type == APP_OUT)
		{
			if (open_outfiles(cmd->tokens[i].content, &cmd->out_file, cmd->tokens[i].type) == FAIL)
				return (FAIL);
			printf("OUTFILE opened [%d] , %s\n", cmd->out_file, cmd->tokens[i].content);
		}
		i++;
	}
	return (SUCCESS);
}

bool	is_heredoc_last(t_cmd *cmd)
{
	int i;
	bool is_heredoc_last;

	i = 0;
	is_heredoc_last = false;
	while (cmd && cmd->tokens && cmd->tokens[i].content)
	{
		if (cmd->tokens[i].type == INFILE)
		{
			is_heredoc_last = false;
		}
		else if (cmd->tokens[i].type == LIMITER)
		{
			is_heredoc_last = true;
		}
		i++;
	}
	return (is_heredoc_last);
}

// <infile <infile1 <infile2 <<EOF <infile3 <<EOF
int	process_cmd_files(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini && mini->cmds[i] != NULL)
	{
		if (handle_cmd_files(mini->cmds[i]) == FAIL)
			return (FAIL);

		if (is_heredoc_last(mini->cmds[i]))
		{
			printf("mini->cmds[i]->in_file BEFORE [%d]\n", mini->cmds[i]->in_file);
			ft_putstr_fd("here doc is last\n", 2);
			ft_putnbr_fd(mini->cmds[i]->hd_fd, 2);
			ft_putstr_fd("hd_fd up ^\n", 2);
			if (mini->cmds[i]->hd_fd != -1)
			{
    			if (mini->cmds[i]->in_file != -1)
        			close(mini->cmds[i]->in_file);
   		 		mini->cmds[i]->in_file = mini->cmds[i]->hd_fd;
			}

			printf("mini->cmds[i]->in_file AFTER [%d]\n", mini->cmds[i]->in_file);
		}
		else
			ft_putstr_fd("infile is last\n", 2);
		i++;
	}
	return (SUCCESS);
}
