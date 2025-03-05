#include "../incl/minishell.h"

// trim whitespace from the start and end of the command string
static char	*skip_whitespace(char *og_str)
{
	int	start;
	int	end;

	if (!og_str)
		return (NULL);
	start = 0;
	end = ft_strlen(og_str) - 1;
	while (og_str[start] && ft_isspace(og_str[start]))
		start++;
	while (end >= start && ft_isspace(og_str[end]))
		end--;
	ft_strcpy(og_str, og_str + start);
	return (og_str);
}

// store and trim individual command to cmd->og_string
static int	handle_individual_cmd(t_mini *mini, t_cmd *cmd, int start, int end)
{
	char	*trim_cmd;
	int		len;

	len = end - start;
	trim_cmd = (char *)malloc(len + 1);
	if (!trim_cmd)
	{
		ft_putstr_fd("Memory allocation for an individual command failed.\n", 2);
		return (FAIL);
	}
	ft_memcpy(trim_cmd, mini->input + start, len);
	trim_cmd[len] = '\0';
	trim_cmd = skip_whitespace(trim_cmd);
	cmd->og_str = trim_cmd;
	if (!cmd->og_str)
	{
		ft_putstr_fd("Failed to store original string in command struct.\n", 2);
		free_ptr(cmd->og_str);
		return (FAIL);
	}
	return (SUCCESS);
}

// splits input with pipes (if unquoted)
int	split_cmds(t_mini *mini)
{
	int	i;
	int	start;
	int	cmd_num;

	i = 0;
	start = 0;
	cmd_num = 0;
	while (mini->input[i])
	{
		if (mini->input[i] == '|' && !check_quotes(mini->input, i))
		{
			mini->cmds[cmd_num]->cmd_num = cmd_num;
			if (handle_individual_cmd(mini, mini->cmds[cmd_num], start, i))
				return (FAIL);
			start = i + 1;
			cmd_num++;
		}
		i++;
	}
	mini->cmds[cmd_num]->cmd_num = cmd_num;
	if (handle_individual_cmd(mini, mini->cmds[cmd_num], start, i))
		return (FAIL);
	return (SUCCESS);
}
