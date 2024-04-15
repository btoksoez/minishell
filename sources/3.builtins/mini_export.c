#include "../../includes/minishell.h"

/* if no args: show all env, (but sorted?)
for each arg:
	export PWD=benian ->PWD="benian"
	export PWD=$USER ->PWD="btoksoez"
	export PWD -> PWD
	export PWD="" -> PWD=""
	export PWD="hi" ->PWD="hi"
	export PWD='hi'->PWD="hi"
	export PWD and PWD exists: keep old PWD
	export PWD="..." and PWD exists: update PWD
	always sorted
	check if equal sign:
		no equal sign:
			if in envp:
				don't add
			add just as word
		if equal sign:
			check if single quotes
				replace single quotes by double quotes and add
			check if dollar and dollar + 1
				if in envp, replace with its value
				else remove envp from string
			put whatever is behind = in double quotes
				add PWD="..."
	sort_env
	*/

void	sort_env(char **envp)
{
	int		i;
	char	*temp;
	int		j;
	int		len;

	if (!envp)
		return ;
	i = 0;
	temp = NULL;
	len = char_arr_len(envp);
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(envp[j], envp[j + 1]) > 0)
			{
				temp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
}

/*
    Checks if a variable exists in the environment as a standalone variable,
    either with an equal sign or without any value assigned.
    Parameters:
        env: Pointer to the array of environment variables.
        var: Variable name to search for in the environment.
*/
int is_inenvp(char **env, char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && (env[i][var_len] == '=' || env[i][var_len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

void	add_env(t_shell *s, char *var)
{
	char	**new_envp;
	int		len;
	int		i;

	if (!var || !s->envp)
		return (error_message("add_env error", NULL));
	len = char_arr_len(s->envp);
	new_envp = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
		return (error_message("add_env malloc", NULL));
	i = 0;
	while (i < len)
	{
		new_envp[i] = s->envp[i];
		i++;
	}
	new_envp[i] = var;
	new_envp [i + 1] = NULL;
	free(s->envp);
	s->envp = new_envp;
}

void	append_env(char **s, char *var, char *value)
{
	char	*current_value;
	char	*append_value;
	char	*new_value;
	char	*new_var;

	current_value = NULL;
	append_value = ft_strdup(ft_strchr(value, '=') + 1);
	if (ft_strchr(*s, '='))
	{
		current_value = ft_strdup(ft_strchr(*s, '=') + 1);
		new_value = ft_strjoin(current_value, append_value);
	}
	else
		new_value = ft_strdup(append_value);
	free(*s);
	new_var = ft_strjoin(var, "=");
	*s = ft_strjoin(new_var, new_value);
	if (current_value != NULL)
		free(current_value);
	free(append_value);
	free(new_value);
	free(var);
	free(new_var);
}

/* if append flag, it should find var*/
void	replace_env(t_shell *s, char *var, char *value, int append)
{
	int		i;
	size_t	var_len;

	if (!is_inenvp(s->envp, var))
		return ;
	i = 0;
	var_len = ft_strlen(var);
	while (s->envp[i] != NULL)
	{
		if (ft_strncmp(s->envp[i], var, var_len) == 0)
		{
			if (append)
				append_env(&s->envp[i], var, value);
			else
			{
				free(s->envp[i]);
				s->envp[i] = value;
			}
		}
		i++;
	}
}

/* not sure if this is needed */
char *add_double_quotes(char *str)
{
	int		i;
	char	*res;

	if (!str)
		return (NULL);
	i = 0;
	res = (char *)malloc(ft_strlen(str) + 3);
	if (!res)
		return (NULL);
	res[0] = '\"';
	while (str[i])
	{
		res[i + 1] = str[i];
		i++;
	}
	res[i + 1] = '\"';
	res[i + 2] = '\0';
	// printf("With double quotes: %s\n", res);
	return (res);
}

char **copy_env(char **envp)
{
	int		len;
	int		i;
	char	**copy;

	len = char_arr_len(envp);
	copy = (char **)malloc((len + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[len] = NULL;
	return (copy);
}
/* return 1 & export error if wrong, else 0 */
int	check_var(char *var)
{
	int	i;

	i = 0;
	if (*var == 0)
		return (export_error("="));
	if (ft_isdigit(var[0]))
		return (export_error(var));
	if (var[0] == '=')
		return (export_error(var));
	while (var[i] && var[i] != '=')
	{
		if (check_valid_identifier(var[i]))
			return (export_error(var));
		i++;
	}
	return (EXIT_SUCCESS);
}

void	check_append(char *var, int *flag_append)
{
	int	len;

	len = ft_strlen(var);
	if (len > 0 && var[len - 1] == '+')
	{
		*flag_append = 1;
		var[len - 1] = '\0';
	}
}

int	mini_export(t_shell *s, t_tree_node *tree)
{
	char	*var;
	char	*value;
	char	**copy;
	char	*new_var;
	int		flag_append;
	int		return_value;

	var = NULL;
	value = NULL;
	return_value = 0;
	flag_append = 0;
	if (!s->envp)
		return (EXIT_FAILURE);
	copy = copy_env(s->envp);
	sort_env(copy);
	//if no arguments, just print env, sorted
	if (!tree->args)
	{
		print_env(copy);
		return (EXIT_SUCCESS);
	}
	//for each arg
	while (tree->args)
	{
		// printf("args arg %s\n", tree->args->arg);
		//get var
		var = ft_strdup_delimiter_char(tree->args->arg, '=');
		//check var
		if (ft_strlen(var) != ft_strlen(tree->args->arg))
			check_append(var, &flag_append);
		if (check_var(var))
		{
			if (tree->args)
				tree->args = tree->args->next;
			return_value = 1;
			continue;
		}
		//check for equal sign
		if (ft_strlen(var) == ft_strlen(tree->args->arg))
		{
			//check if it's in envp, if not then add it, else nothing
			if (!is_inenvp(s->envp, var))
				add_env(s, var);
		}
		else
		{
			//get value from tokens
			if (tree->args->space)
				value = ft_strdup(ft_strchr(tree->args->arg, '=') + 1);
			else
			{
				value = ft_strdup(ft_strchr(tree->args->arg, '=') + 1);
				tree->args = tree->args->next;
				while (tree->args->space == 0)
				{
					value = ft_strjoin(value, tree->args->arg);
					tree->args = tree->args->next;
				}
				if (tree->args)
					value = ft_strjoin(value, tree->args->arg);
			}
			// value = add_double_quotes(value);
			new_var = ft_strjoin(var, "=");
			value = ft_strjoin(new_var, value);
			free(new_var);
			//check if var in s->envp already, if yes replace, else add
			if (is_inenvp(s->envp, var))
				replace_env(s, var, value, flag_append);
			else
				add_env(s, value);
		}
		if (tree->args)
			tree->args = tree->args->next;
	}
	ft_freematrix(copy);
	return (return_value);
}
