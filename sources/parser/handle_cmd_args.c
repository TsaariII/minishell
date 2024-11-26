/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 11:55:06 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);
int	count_args(t_cmd *cmd, int i);

// Counts how many command arguments the string contains
int	count_args(t_cmd *cmd, int i)
{
	int	args_count;

	args_count = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i++;
			while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
				i++;
			while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) && is_redirection(cmd, i))
				i++;
		}
		else
		{
			args_count++;
			while (cmd->segment[i]&& !ft_isspace(cmd->segment[i]) &&
			!is_redirection(cmd, i))
				i++;
		}
		while (cmd->segment[i]&& ft_isspace(cmd->segment[i]))
			i++;
	}
	return (args_count);

}

// Counts how many command arguments the string contains
int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int		arg_len;
	char	*arg_start;
	int		args_count;
	int		arg_index;
	char	*expanded;

	arg_index = 0;
	args_count = count_args(cmd, i);
	cmd->args = ft_calloc(args_count + 2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[arg_index] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		ft_free_array(cmd->args);
		return (-1);
	}
	arg_index++;
	i = skip_whitespace(cmd->segment, i);
	while (cmd->segment[i] && arg_index < args_count + 1 && !is_redirection(cmd, i))
	{
		if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
			i = arg_in_quotes(cmd->segment, i, &arg_start, &arg_len);
		else
			i = arg_no_quotes(cmd, i, &arg_start, &arg_len);
		expanded = expand_var(mini, ft_strndup(arg_start, arg_len));
		if (!expanded)
		{
			ft_free_array(cmd->args);
			return (-1);
		}
		if (append_to_array(cmd, expanded, ft_strlen(expanded), &arg_index) == -1)
		{
			free(expanded);
			ft_free_array(cmd->args);
			return (-1);
		}
		free(expanded);
		i = skip_whitespace(cmd->segment, i);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}
