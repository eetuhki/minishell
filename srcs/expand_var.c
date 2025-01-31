#include "../incl/minishell.h"

void init_exp_vars(t_xpnd *xp)
{
	xp->sq_open = false;
	xp->dq_open = false;
	xp->var = NULL;
	xp->value = NULL;
	xp->xp_str = NULL;
}

void extract_var_name(char *input, char *var_name, int size)
{
	int i = 0;
	int j = 0;
	while (input[i] && input[i] != '$')
		i++;
	if (input[i] == '$')
	{
		i++;
		while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		{
			if (j >= size - 1)
				break;
			var_name[j++] = input[i];
			i++;
		}
	}
	var_name[j] = '\0';
}

void	expand_var(t_mini *mini, char *cmd, char **expanded)
{
    t_xpnd	xp;
    char    var[256];
    char    *new_cmd;
    char    *temp;
    char    *input = cmd;
    size_t  new_len = 0;
    size_t  var_len = 0;

    init_exp_vars(&xp);
    new_cmd = ft_strdup("");
    if (!new_cmd)
        return;
    while (*input)
    {
		if (*input == SQUOTE && !xp.dq_open)
			xp.sq_open = !xp.sq_open;
		else if (*input == DQUOTE && !xp.sq_open)
			xp.dq_open = !xp.dq_open;
        else if (*input == '$'  && !xp.sq_open)
        {
            extract_var_name(input, var, sizeof(var));
            input++;
            var_len = ft_strlen(var);
            xp.value = env_get_var(mini, var);
            if (!xp.value)
                xp.value = ""; 
            temp = ft_strjoin(new_cmd, xp.value);
			if (!temp)
			{
				free_ptr(new_cmd);
				return;
			}
            free_ptr(new_cmd);
            new_cmd = temp;
            input += var_len -1; 
        }
        else
        {
            new_len = ft_strlen(new_cmd);
            temp = ft_realloc(new_cmd, new_len + 2);
            if (!temp)
            {
                free_ptr(new_cmd);
                return;
            }
            new_cmd = temp;
            new_cmd[new_len] = *input;
            new_cmd[new_len + 1] = '\0';
        }
        input++;
    }
    *expanded = new_cmd;
}
