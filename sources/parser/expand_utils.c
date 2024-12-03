/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:12:11 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/02 15:41:12 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_value(t_env *env, char *name)
{
	t_env	*temp;
	char	*value;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			value = ft_strdup(temp->value);
			if (!value)
			{
				ft_putendl_fd("malloc fail", 2);
				return ((char *) -1);
			}
			return (value);
		}
		temp = temp->next;
	}
	return (NULL);
}

static int	we_have_value(char *value, char *temp, char **expanded)
{
	if (value)
	{
		temp = ft_strjoin(*expanded, value);
		free(value);
		if (!temp)
		{
			ft_putendl_fd("malloc failed", 2);
			return (-1);
		}
		free(*expanded);
		*expanded = temp;
	}
	return (0);
}

int	handle_value(t_shell *mini, t_vdata *data)
{
	data->value = get_value(mini->env, data->name);
	if (data->value == (char *)-1)
		return (1);
	if (data->value)
	{
		if (we_have_value(data->value, data->temp, data->expanded) == -1)
			return (1);
	}
	return (0);
}

void	init_vdata(t_vdata *data, char **expanded, char *temp, char *name)
{
	data->value = NULL;
	data->expanded = expanded;
	data->temp = temp;
	data->name = name;
}