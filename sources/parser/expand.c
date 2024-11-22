/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:58:12 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/22 15:25:51 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_value(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			return (temp->value);
		}
		temp = temp->next;
	}
	return (NULL);
}


static int oh_its_a_dollar(t_shell *mini, char *str, char **expanded, int i, int *s)
{
	char	*temp;
	int		indx;
	char	*value;
	char 	*temp2;
	char	name[100];

	temp2 = ft_strndup(&str[*s], i - *s);
	temp = ft_strjoin(*expanded, temp2);
	free(temp2);
	free(*expanded);
	*expanded = temp;
	i++;
	indx = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		name[indx++] = str[i++];
	name[indx] = '\0';
	if (indx == 0)
		return (i);
	value = get_value(mini->env, name);
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(*expanded);
		*expanded = temp;
	}
	return (i);
}

static int	tildes_home(t_shell *mini, char *str, char **expanded, int i, int s)
{
	char	*temp;
	char	*value;

	temp = ft_strjoin(*expanded, ft_strndup(&str[s], i - s));
	free(*expanded);
	*expanded = temp;
	i++;
	value = get_value(mini->env, "HOME");
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(*expanded);
		*expanded = temp;
	}
	return (i);
}

char	*expand_var(t_shell *mini, char *str)
{
	char	*expanded;
	char	*temp;
	char	*temp2;
	int		i;
	int		s;

	i = 0;
	s = 0;
	expanded = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			i = oh_its_a_dollar(mini, str, &expanded, i, &s);
			s = i;
		}
		else if (str[i] == '~')
		{
			i = tildes_home(mini, str, &expanded, i, s);
			s = i;
		}
		else
			i++;
	}
	temp2 = ft_strndup(&str[s], i - s);
	temp = ft_strjoin(expanded, temp2);
	free(expanded);
	free(temp2);
	free(str);
	return (temp);
}

// static void	add_char(char **str, char c)
// {
// 	char	temp[2];
// 	char	*new_str;

// 	temp[0] = c;
// 	temp[1] = '\0';
// 	new_str = ft_strjoin(*str, temp);
// 	free(*str);
// 	*str = new_str;
// }

// static	char	*its_in_quotes(t_shell *mini, char *str)
// {
// 	char	*expanded;
// 	char	quote;
// 	int		i;


// 	i = 1;
// 	expanded = ft_strdup("");
// 	quote = str[0];
// 	while (str[i])
// 	{
// 		if (str[i] == quote)
// 		{
// 			i++;
// 			break ;
// 		}
// 		else if (str[i] == '$' && quote == '"')
// 			i = oh_its_a_dollar(mini, str, &expanded, i, &i);
// 		else
// 			add_char(&expanded, str[i++]);
// 	}
// 	return (expanded);
// }

// int	expand_or_not(t_shell *mini, t_cmd *cmd)
// {
// 	int		i;
// 	char	*arg;
// 	char	*expanded;

// 	i = 0;
// 	while (cmd->args[i])
// 	{
// 		arg = cmd->args[i];
// 		if (arg[0] == '"' || arg[0] == '\'')
// 		{
// 			expanded = its_in_quotes(mini, arg);
// 			//free(arg);
// 			cmd->args[i] = expanded;
// 		}
// 		else if (cmd->args[i][0] == '$' || cmd->args[i][0] == '~')
// 		{
// 			expanded =  expand_var(mini, arg);
// 			//free(arg);
// 			cmd->args[i] = expanded;
// 		}
// 		printf("arg[%d]: %s\n", i, cmd->args[i]);
// 		i++;
// 	}
// 	return (0);
// }

// int	expand_or_not(t_shell *mini, t_cmd *cmd)
// {
// 	int		i;
// 	char	*temp;

// 	i = 0;
// 	while (cmd->args[i])
// 	{
// 		if (cmd->args[i][0] == '"')
// 		{
// 			temp = ft_strdup(cmd->args[i] + 1);
// 			free(cmd->args[i]);
// 			cmd->args[i] = expand_var(mini, temp);
// 		}
// 		else if (cmd->args[i][0] == '$' || cmd->args[i][0] == '~')
// 		{
// 			cmd->args[i] = expand_var(mini, cmd->args[i]);
// 		}
// 		else if (cmd->args[i][0] == '\'')
// 		{
// 			temp = ft_strtrim(cmd->args[i], "'");
// 			free(cmd->args[i]);
// 			cmd->args[i] = temp;
// 		}
// 		i++;
// 	}
// 	return (0);
// }

