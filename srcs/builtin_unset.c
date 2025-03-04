#include "../incl/minishell.h"

static int	copy_env_excl_var(t_mini *m, char ***new_env, char **key, int var_i)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (m->env[i])
	{
		if (i == var_i)
		{
			i++;
			continue ;
		}
		(*new_env)[j] = ft_strdup(m->env[i]);
		if (!(*new_env)[j])
		{
			free_arr(*new_env);
			*new_env = NULL;
			free_str(key);
			return (FAIL);
		}
		j++;
		i++;
	}
	return (SUCCESS);
}

static void	ft_unset_single(t_mini *mini, char *var)
{
	char	*key;
	int		env_var_index;
	size_t	count;
	char	**new_env;

	key = extract_key(var);
	env_var_index = search_key_in_env(mini, key);
	if (env_var_index == -1)
	{
		free_str(&key);
		return ;
	}
	count = env_count_variables(mini->env);
	new_env = ft_calloc(count, sizeof(char *));
	if (!new_env)
	{
		free_str(&key);
		return ;
	}
	if (copy_env_excl_var(mini, &new_env, &key, env_var_index) == FAIL)
		return ;
	free_str(&key);
	free_arr(mini->env);
	mini->env = new_env;
}

int	ft_unset(t_mini *mini, char **cmd_args)
{
	int	i;
	int	ret;

	if (!mini || !mini->env || !*mini->env || !cmd_args || !*cmd_args)
		return (0);
	i = 1;
	ret = SUCCESS;
	while (cmd_args[i])
	{
		if (!valid_key(cmd_args[i], true))
		{
			print_err_builtin("unset: `", cmd_args[i],
				"': not a valid identifier");
			ret = FAIL;
			i++;
			continue ;
		}
		ft_unset_single(mini, cmd_args[i]);
		i++;
	}
	return (ret);
}
