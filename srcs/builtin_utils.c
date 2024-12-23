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
