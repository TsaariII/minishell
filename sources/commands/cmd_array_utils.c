/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/11 13:43:47 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_args(t_cmd *cmd, int i);
int	count_args(t_cmd *cmd, int i);

// Retrieves command arguments from string and copies them to struct

int	handle_cmd_args(t_cmd *cmd, int i)
{
	int		arg_length;
	char	*arg_start;
	char	args_count;
	int		arg_index;

	arg_index = 0;
	args_count = count_args(cmd, i);
	cmd->args = ft_calloc(args_count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i] && arg_index <= args_count)
	{
		arg_start = &cmd->segment[i];
		arg_length = 0;
		while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
		!is_redirection(cmd, i))
		{
			arg_length++;
			i++;
		}
		cmd->args[arg_index] = ft_strndup(arg_start, arg_length);
		if (!cmd->args[arg_index])
		{
			printf("Failed to allocate memory for argument\n");
			return (-1);
		}
		arg_index++;
		while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
			i++;
	}
	cmd->args[arg_index] = NULL;
	printf("index after handle_args: %d\n", i);
	return (i);
}

// Counts how many command arguments the string contains

int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i])
	{
		if (cmd->segment[i] && !is_redirection(cmd, i))
		{
			args_count++;
			while (cmd->segment[i]&& !ft_isspace(cmd->segment[i]) &&
			!is_redirection(cmd, i))
				i++;
		}
		else if (is_redirection(cmd, i))
			break ;
		while (cmd->segment[i]&& ft_isspace(cmd->segment[i]))
			i++;
	}
	printf("Argument count: %d\n", args_count);
	return (args_count);
}
