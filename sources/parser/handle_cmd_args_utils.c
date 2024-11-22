/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/22 14:53:46 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	arg_in_quotes(char *str, int i, char **start, int *len)
{
	int		s;
	char	quote;
	char	*temp;
	char	*res;

	quote = str[i];
	s = i + 1;
	i++;
	*len = 0;
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == quote)
		{
			if (str[i + 1] == quote)
			{
				i++;
				continue ;
			}
			else
			{
				i++;
				break ;
			}
		}
		temp = ft_strjoin(res, ft_strndup(&str[i], 1));
		free(res);
		res = temp;
		i++;
	}
	*start = res;
	*len = ft_strlen(res);
	return (i);
}

int	arg_no_quotes(t_cmd *cmd, int i, char **start, int *len)
{
	*start = &cmd->segment[i];
	*len = 0;

	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i]) ||
		check_quotes(cmd->segment, i)) && !is_redirection(cmd, i))
	{
		(*len)++;
		i++;
	}
	return (i);
}

int	append_to_array(t_cmd *cmd, char *start, int len, int *index)
{
	cmd->args[*index] = ft_strndup(start, len);
	if (!cmd->args[*index])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		return (-1);
	}
	printf("before args[%d]: %s\n", *index, cmd->args[*index]);
	(*index)++;
	return (0);
}
