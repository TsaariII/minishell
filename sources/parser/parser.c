/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/05 14:08:54 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			parse_input(t_shell *mini);
int			parse_cmd_string(t_shell *mini, t_cmd *cmd);
int			handle_redirections(t_cmd *cmd, int i);
int			handle_cmd_name(t_cmd *cmd, int i);
static int	no_args(t_cmd *cmd, int i);
static int	double_redirect(t_cmd *cmd, int i);
static int	single_redirect(t_cmd *cmd, int i);
static bool	is_empty_command(t_cmd *cmd, int i);

// Validates input string and parses the content into an array of structs

int	parse_and_validate_input(char *input, t_shell *mini)
{
	if (validate_input_syntax(&input))
		return (1);
	if (prepare_command_structs(mini, input))
		return (1);
	if (split_input_by_pipes(input, mini))
		return (1);
	if (parse_input(mini))
	{
		clean_cmds(mini->cmds);
		return (1);
	}
	return (0);
}

int	parse_input(t_shell *mini)
{
	int	index;

	index = 0;
	while (mini->cmds[index])
	{
		if (parse_cmd_string(mini, mini->cmds[index]))
		{
			clean_cmds(mini->cmds);
			return (1);
		}
		if (is_this_built(mini->cmds[index]->command) != 1)
		{
			if (get_cmd_path(mini, mini->cmds[index]))
				return (1);
		}
		index++;
	}
	return (0);
}

// Parses the segment string of each struct
static int	no_args(t_cmd *cmd, int i)
{
	cmd->args = ft_calloc(2, sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args)
		return (-1);
	cmd->args[1] = NULL;
	return (i);
}

/*int	parse_cmd_string(t_shell *mini, t_cmd *cmd)
{
	int		i;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	i = handle_redirections(cmd, i);
	if (i == -1)
		return ((1));
	if (is_empty_command(cmd, i))
		return (1);
	i = handle_cmd_name(cmd, i);
	if (i == -1)
		return (1);
	cmd_found = true;
	if (!cmd->segment[i] || is_redirection(cmd, i))
		i = no_args(cmd, i);
	else
	{
		i = handle_cmd_args(mini, cmd, i);
		if (i == -1)
			return (1);
	}
	while (cmd->segment[i] && cmd_found && !is_redirection(cmd, i))
	{
		i = handle_cmd_args(mini, cmd, i);
		if (i == -1)
			return (1);
	}
	i = handle_redirections(cmd, i);
	if (i == -1)
		return (1);
	return (0);
} */

int	parse_cmd_string(t_shell *mini, t_cmd *cmd)
{
	int		i;
	bool	cmd_found;

	i = 0;
	cmd_found = false;
	while (cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(cmd, i);
			if (i == -1)
				return (1);
			if (is_empty_command(cmd, i))
				return (1);
		}
		else if (!cmd_found)
		{
			i = handle_cmd_name(cmd, i);
			if (i == -1)
				return (1);
			cmd_found = true;
		}
		else
		{
			i = handle_cmd_args(mini, cmd, i);
			if (i == -1)
				return (1);
		}
	}
	if (cmd_found && (!cmd->args || !cmd->args[0]))
		i = no_args(cmd, i);
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
	while (i < (int)ft_strlen(cmd->segment) && cmd->segment[i])
	{
		if (is_redirection(cmd, i))
		{
			if (redirll_head_tail(cmd))
				return (-1);
			if ((i + 1 < (int)ft_strlen(cmd->segment))
				&& ((cmd->segment[i] == '<' && cmd->segment[i + 1] == '<')
				|| (cmd->segment[i] == '>' && cmd->segment[i + 1] == '>')))
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
		}
		else
			break;
	}
	return (i);
}

static bool	is_empty_command(t_cmd *cmd, int i)
{
	int	len;

	len = ft_strlen(cmd->segment);
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (i != len - 1)
	{
		if (!cmd->segment[i] || cmd->segment[i] == '|')
		{
			ft_putendl_fd("syntax error: expected a command", 2);
			return (true);
		}
	}
	return (false);
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

