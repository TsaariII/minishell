/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:26:25 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/21 16:35:07 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	this_is_quote(t_cmd *cmd, t_expand *arg)
{
	char	*temp;

	if (!arg->sgl && !arg->dbl && (cmd->segment[arg->i] == '\'' || cmd->segment[arg->i] == '"'))
	{
		temp = arg->value;
		arg->value = ft_strjoin_char(temp, cmd->segment[arg->i]);
		what_quote(cmd->segment, arg);
		free(temp);
	}
	else if ((arg->sgl && cmd->segment[arg->i] == '\'')
		|| (arg->dbl && cmd->segment[arg->i] == '"'))
	{
		temp = arg->value;
		arg->value = ft_strjoin_char(arg->value, cmd->segment[arg->i]);
		what_quote(cmd->segment, arg);
		free(temp);
	}
	return (arg->i);
}

int export_args(t_cmd *cmd, t_expand *arg, int i)
{
	the_arg(arg, i);
	what_quote(cmd->segment, arg);
	while (cmd->segment[arg->i])
	{
		if (ft_isspace(cmd->segment[arg->i]) && !arg->sgl && !arg->dbl)
			break ;
		if (cmd->segment[arg->i] == '\'' || cmd->segment[arg->i] == '"')
			arg->i = this_is_quote(cmd, arg);
		else if (add_char(cmd->segment, arg))
			return (free(arg->value), -1);
	}
	arg->len = ft_strlen(arg->value);
	return (arg->i);
}

int	handle_export_redir(t_cmd *cmd, t_expand *arg, int i, int *arg_index)
{
	i = export_args(cmd, arg, i);
	if (!arg->value || append_to_array(cmd, arg->value, arg_index) == -1)
	{
		free(arg->value);
		ft_free_array(cmd->args);
		return (-1);
	}
	free(arg->value);
	i = skip_whitespace(cmd->segment, i);
	return (i);
}

int	handle_arg(t_cmd *cmd, int i, t_expand *arg, int *arg_index)
{
	if (ft_strcmp(cmd->command, "export") == 0 && only_redirect(cmd->segment, i))
	{
		i = handle_export_redir(cmd, arg, i, arg_index);
		if (i == -1)
			return (-1);
	}
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		i = arg_in_quotes(cmd->segment, i, arg);
	else if (!ft_isspace(cmd->segment[i]) || !is_redirection(cmd, i))
		i = arg_no_quotes(cmd, arg, i);
	if (i == -1)
		return (-1);
	if (!arg->value || append_to_array(cmd, arg->value, arg_index) == -1)
	{
		free(arg->value);
		ft_free_array(cmd->args);
		return (-1);
	}
	free(arg->value);
	i = skip_whitespace(cmd->segment, i);
	return (i);
}

int	handle_cmd_args(t_shell *mini, t_cmd *cmd, int i)
{
	int			arg_index;
	t_expand	arg;

	if (init_args_array(cmd, i) == -1)
		return (-1);
	arg_index = 1;
	i = skip_whitespace(cmd->segment, i);
	the_arg(&arg, i);
	free(arg.value);
	while (cmd->segment[i] && arg_index < cmd->args_count + 1)
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			i = skip_whitespace(cmd->segment, i);
			continue;
		}
		i = handle_arg(cmd, i, &arg, &arg_index);
		if (i == -1)
			return (-1);
	}
	cmd->args[arg_index] = NULL;
	return (i);
}
