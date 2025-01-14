#include "../incl/minishell.h"

void	get_input(t_mini *mini)
{
	mini->input = readline("mini> ");
	if (!mini->input)
		exit(0);
	addhistory(mini);
}

void	init_mini(t_mini *mini)
{
	mini->env = NULL;
	mini->input = NULL;
	mini->prev_input = NULL;
	mini->redir_count = 0;
	mini->redir_open = 0;
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
		handle_builtin(mini);
		syntax_check(mini);
	}
	return (SUCCESS);
}
