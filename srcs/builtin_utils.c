#include "minishell.h"

//updates env variables PWD and OLDPWD after executing the cd command
void update_env_vars(t_mini *mini)
{
    char	*old_pwd;
    char	*new_pwd;

	old_pwd = env_get_var(mini, "PWD");
	new_pwd = getcwd(NULL, 0);

    if (!new_pwd)
		return ;
	if (old_pwd)
	{
		if (env_set_var(mini, "OLDPWD", old_pwd) == FAIL)
			ft_putstr_fd("mini: cd: Failed to update OLDPWD\n", 2);
		if (env_set_var(mini, "PWD", new_pwd) == FAIL)
			ft_putstr_fd("mini: cd: Failed to update PWD\n", 2);
   		free_ptr(new_pwd);
	}
}

char	*extract_key(char *cmd_arg)
{
	size_t	i;
	char	*key;

	i = 0;
	while (cmd_arg[i])
	{
		if (cmd_arg[i] == '=')
			break;
		i++;
	}
	key = ft_substr(cmd_arg ,0 ,i);
	return (key);
}

char	*extract_value(char *cmd_arg)
{
	size_t	i;
	char	*value;

	i = 0;
	if (!ft_strchr(cmd_arg, '='))
		return (NULL);
	while (cmd_arg[i])
	{
		if (cmd_arg[i] == '=')
			break;
		i++;
	}
	 if (cmd_arg[i + 1] == '\0')
        return (ft_strdup(""));
	value = ft_substr(cmd_arg ,i + 1 ,(ft_strlen(cmd_arg) - i));
	return (value);
}

// adds an env pair, return: SUCCESS or FAIL
int	add_env_pair(t_mini *mini, char *key, char *value)
{
	size_t	count;
	char	*temp;
	char	*new_pair;
	char	**new_env;

	count = env_count_variables(mini->env);
	new_env = realloc(mini->env, (count + 2) * sizeof(char **));
	if (!new_env)
		return (FAIL);
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (FAIL);
	new_pair = ft_strjoin(temp, value);
	free_ptr(temp);
	if (!new_pair)
		return(FAIL);
	new_env[count] = new_pair;
	new_env[count + 1] = NULL;
	//free_arr(mini->env);
	mini->env = new_env;
	return (SUCCESS);
}
