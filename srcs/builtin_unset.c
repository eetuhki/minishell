#include "../incl/minishell.h"

void	ft_unset_single(t_mini *mini, char *var)
{
	//now this is the var that i want to unset
	char	*key;
	int		env_var_index;
	size_t	count;
	char	**new_env;
	int		i;
	int		j;

	key = extract_key(var);
	env_var_index = search_key_in_env(mini, key);
	if (env_var_index == -1)
	{
		free_str(&key);
		return ;
	}
	count = env_count_variables(mini->env);
	//count bcoz num of var -1 + NULL
	new_env = ft_calloc(count, sizeof(char *));
	if (!new_env)
	{
		free_str(&key);
		return ;
	}
	i = 0;
	j = 0;
	while (mini->env[i])
	{
		if (env_var_index == i)
		{
			i++;
			continue;
		}
		new_env[j] = ft_strdup(mini->env[i]);
		if (!new_env[j])
		{
			free_arr(new_env);
			free_str(&key);
			return ;
		}
		j++;
		i++;
	}
	free_str(&key);
	free_arr(mini->env);
	mini->env = new_env;
}
int	ft_unset(t_mini *mini, char **cmd_args)
{
	int	i;

	if (!mini || !mini->env || !*mini->env || !cmd_args || !*cmd_args)
		return (0);
	i = 1;
	while (cmd_args[i])
	{
		ft_unset_single(mini, cmd_args[i]);
		i++;
	}
	return (SUCCESS);
}
