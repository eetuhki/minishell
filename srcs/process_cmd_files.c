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
		//printf("closing infile[%d]. [%s] <- new_file\n", *in_file, file_name);
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
		//printf("closing prev outfile [%s] <- newfile \n", file_name);
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
		/* if (cmd->tokens[i].type == LIMITER)
		{
			if (cmd->hd_fd != -1)
				cmd->in_file = cmd->hd_fd;
		} */
		if (cmd->tokens[i].type == INFILE)
		{
			if (open_infile(cmd->tokens[i].content, &cmd->in_file) == FAIL)
				return (FAIL);
			//printf("INFILE opened [%d] , %s \n", cmd->in_file, cmd->tokens[i].content);
		}
		else if (cmd->tokens[i].type == OUTFILE || cmd->tokens[i].type == APP_OUT)
		{
			if (open_outfiles(cmd->tokens[i].content, &cmd->out_file, cmd->tokens[i].type) == FAIL)
				return (FAIL);
			//printf("OUTFILE opened [%d] , %s\n", cmd->out_file, cmd->tokens[i].content);
		}
		i++;
	}
	return (SUCCESS);
}

int	process_cmd_files(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini && mini->cmds[i] != NULL)
	{
		if (handle_cmd_files(mini->cmds[i]) == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}
