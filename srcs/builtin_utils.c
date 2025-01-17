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
int	add_env_pair(t_mini *mini, char *key, char *value, bool has_value)
{
	size_t	count;
	char	*temp;
	char	*new_pair;
	char	**new_env;

	count = env_count_variables(mini->env);
	new_env = realloc(mini->env, (count + 2) * sizeof(char **));
	if (!new_env)
		return (FAIL);
	if (has_value)
	{
		temp = ft_strjoin(key, "=");
		if (!temp)
			return (FAIL);
	}
	else
		temp = key;
	if (value != NULL)
		new_pair = ft_strjoin(temp, value);
	else 
		new_pair = ft_strdup(temp);
	free_ptr(temp);
	if (!new_pair)
		return(FAIL);
	new_env[count] = new_pair;
	new_env[count + 1] = NULL;
	mini->env = new_env;
	return (SUCCESS);
}

char **copy_env(char **env, ssize_t count)
{
    char    **env_copy;
    size_t    i;

    i = 0;
    
    env_copy = ft_calloc(count + 1, sizeof(char **));
    if (!env_copy)
        return (NULL);
    while (env[i])
    {
        env_copy[i] = ft_strdup(env[i]);
        if (!env_copy[i])
        {
            free_arr(env_copy);
            return (NULL);
        }
        i++;
    }
    return (env_copy);
}

void    sort_env(char **env_copy, ssize_t size)
{
    ssize_t i;
    ssize_t j;
    char    *temp;
    int     swapped;

    i = 0;
    while (i < size - 1)
    {
        j = 0;
        swapped = 0;
        while (j < size - i - 1)
        {
            if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
            {
                temp = env_copy[j];
                env_copy[j] = env_copy[j + 1];
                env_copy[j + 1] = temp;
                swapped = 1;
            }
            j++;
        }
        if (!swapped)
            break;
        i++;
    }
}

void print_export(t_mini *mini)
{
    int    i;
    int     j;
    size_t    count;

    char    **env_copy;
    //free env copy later
    count = env_count_variables(mini->env);
    env_copy = copy_env(mini->env, count);
    sort_env(env_copy, count);
    i = 0;
    while (env_copy && env_copy[i] != NULL)
    {
        if (strncmp(env_copy[i], "_=", 2) == 0)
        {
            i++;
            continue;
        }
        j = 0;
        printf("declare -x ");
        while (env_copy[i][j] && env_copy[i][j] != '=')
            printf("%c", env_copy[i][j++]);
        if (env_copy[i][j] == '=')
        {
            printf("=\"");
            j++;
            while (env_copy[i][j] != '\0')
            {
                printf("%c", env_copy[i][j]);
                j++;
            }
            printf("\"");
        }
        printf("\n");
        i++;
    }
}
