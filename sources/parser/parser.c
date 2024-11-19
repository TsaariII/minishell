/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/19 10:17:23 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		parse_input(t_shell *mini);
int		parse_cmd_string(t_cmd *cmd);
int		handle_redirections(t_cmd *cmd, int i);
int		handle_cmd_name(t_cmd *cmd, int i);
int		is_this_built(char *str);
static int	no_args(t_cmd *cmd, int i);
static int	double_redirect(t_cmd *cmd, int i);
static int	single_redirect(t_cmd *cmd, int i);

// Validates input string and parses the content into an array of structs

int	parse_and_validate_input(char *input, t_shell *mini)
{
	if (validate_input_syntax(input))
		return (1);
	if (prepare_command_structs(mini, input))
		return (1);
	if (split_input_by_pipes(input, mini))
		return (1);
	if (parse_input(mini))
		return (1);
	return (0);
}

// Parses information added to array of structs

int	parse_input(t_shell *mini)
{
	int	index;

	index = 0;
	while (mini->cmds[index])
	{
		if (parse_cmd_string(mini->cmds[index]))
			return (1);
		if (expand_or_not(mini, mini->cmds[index]))
			return (1);
		if (is_this_built(mini->cmds[index]->command) != 1)
		{
			if (get_cmd_path(mini, mini->cmds[index]))
				return (1);
		}
		if (resolve_fd(mini->cmds[index]) == -1)
			return (1);
		index++;
	}
	return (0);
}

// Parses the segment string of each struct

int	parse_cmd_string(t_cmd *cmd)
{
	int		i;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	i = handle_redirections(cmd, i);
	if (i == -1)
		return ((1));
	i = handle_cmd_name(cmd, i);
	if (i == -1)
		return (1);
	cmd_found = true;
	if (!cmd->segment[i] || is_redirection(cmd, i))
		i = no_args(cmd, i);
	else
	{
		i = handle_cmd_args(cmd, i);
		if (i == -1)
			return (-1);
	}
	while (cmd->segment[i] && cmd_found && !is_redirection(cmd, i))
	{
		i = handle_cmd_args(cmd, i);
		if (i == -1)
			return (1);
	}
	i = handle_redirections(cmd, i);
	if (i == -1)
		return (1);
	return (0);
}

int	is_this_built(char *str)
{
	if (ft_strcmp(str, "exit") == 0)
		return (1);
	else if (ft_strcmp(str, "cd") == 0)
		return (1);
	else if (ft_strcmp(str, "echo") == 0)
		return (1);
	else if (ft_strcmp(str, "env") == 0)
		return (1);
	else if (ft_strcmp(str, "pwd") == 0)
		return (1);
	else if (ft_strcmp(str, "unset") == 0)
		return (1);
	else if (ft_strcmp(str, "export") == 0)
		return (1);
	return (0);
}

//Loops through segment string to find redirection symbols
// creates a linked list if redirection symbol(s) are found
// each redirect will be its own node and will contain information about redirection type,
// filename, delimiter and pointer to next node

static int	double_redirect(t_cmd *cmd, int i)
{
	if (cmd->segment[i] == '<' && cmd->segment[i + 1] == '<')
	{
		i = handle_heredoc(cmd, i);
		if (i == -1)
			return (-1);
	}
	else if (cmd->segment[i] == '>' && cmd->segment[i + 1] == '>')
	{
		i = handle_append(cmd, i);
		if (i == -1)
			return (-1);
	}
	return (i);
}

static int	single_redirect(t_cmd *cmd, int i)
{
	if (cmd->segment[i] == '<')
	{
		i = handle_redirect_in(cmd, i);
		if (i == -1)
			return (-1);
	}
	else if (cmd->segment[i] == '>')
	{
		i = handle_redirect_out(cmd, i);
		if (i == -1)
			return (-1);
	}
	return (i);
}

int handle_redirections(t_cmd *cmd, int i)
{
	while (cmd->segment[i] && i < (int)ft_strlen(cmd->segment))
	{
		if (is_redirection(cmd, i))
		{
			if (redirll_head_tail(cmd))
				return (-1);
			if ((cmd->segment[i] == '<' && cmd->segment[i + 1] == '<') ||
				(cmd->segment[i] == '>' && cmd->segment[i + 1] == '>'))
			{
				i = double_redirect(cmd, i);
				if (i == -1)
					return (-1);
			}
			else if (cmd->segment[i] == '<' || cmd->segment[i] == '>')
			{
				i = single_redirect(cmd, i);
				if (i == -1)
					return (-1);
			}
			i++;
		}
		else
			break;
	}
	return (i);
}

// Retrieves command name from string and copies it to struct

int	handle_cmd_name(t_cmd *cmd, int i)
{
	char	*cmd_start;
	int		cmd_length;

	cmd_length = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		i++;
	cmd_start = &cmd->segment[i];
 	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
	!is_redirection(cmd, i))
	{
		cmd_length++;
		i++;
	}
	cmd->command = ft_strndup(cmd_start, cmd_length);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	return (i);
}

static int	no_args(t_cmd *cmd, int i)
{
	cmd->args = ft_calloc(2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args)
		return (-1);
	return (i);
}