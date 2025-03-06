#include "../incl/minishell.h"

volatile sig_atomic_t	g_sig = 0;

static void	get_input(t_mini *mini)
{
	if (isatty(fileno(stdin)))
		sig_init();
	mini->input = readline("mini> ");
	if (!mini->input)
	{	
		free_ptr(mini->input);
		free_and_exit(mini);
	}
	if (mini->input)
		addhistory(mini);
}

static void	init_mini(t_mini *mini)
{
	mini->env = NULL;
	mini->input = NULL;
	mini->prev_input = NULL;
	mini->exit_code = 0;
	mini->redir_count = 0;
	mini->redir_open = 0;
	mini->pipes = 0;
	mini->heredoc_expand = true;
	mini->cmds_tbl = NULL;
	mini->pids = NULL;
	mini->in_fd = STDIN;
	mini->std_fds[0] = -1;
	mini->std_fds[1] = -1;
	mini->in_pipe = false;
}

int	main(int ac, char **av, char **env)
{
	t_mini	*mini;

	arg_check(ac, av);
	mini = ft_calloc(1, sizeof(t_mini));
	if (!mini)
		return (1);
	init_mini(mini);
	env_copy(mini, env);
	while (1)
	{
		get_input(mini);
		if (syntax_check(mini) == FAIL)
		{
			mini->exit_code = 2;
			continue ;
		}
		if (parser(mini) == FAIL)
			continue ;
		prepare_cmd_table(mini);
		execute(mini);
	}
	free_and_exit(mini);
	return (SUCCESS);
}
