#include "../incl/minishell.h"

// if input is valid, adds input to history, then sets current input to prev 
// ready to receive the next input
void	addhistory(t_mini *mini)
{
	if (mini->input)
	{
		add_history(mini->input);
		if (mini->prev_input)
			free_ptr(mini->prev_input);
		mini->prev_input = mini->input;
	}
}
