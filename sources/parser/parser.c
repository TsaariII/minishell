/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:26:26 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 16:05:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_and_validate_input(char **input, t_shell *mini);
int	parse_input(t_shell *mini);
int	parse_cmd_string(t_shell *mini, t_cmd *cmd);
int	cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found);

/**
 * parse_and_validate_input - Validates and parses the user's input
 * into commands.
 *
 * @input: Pointer to the input string to parse.
 * @mini: Pointer to the shell structure containing command and execution data.
 *
 * This function is responsible for ensuring the input syntax is correct,
 * initializing command structures, splitting the input string by pipe symbols,
 * and parsing each segment into command structures. If any step fails,
 * the function cleans up resources (such as unlinking heredocs) and returns 1.
 * On successful parsing and validation, the function returns 0.
 */
int	parse_and_validate_input(char **input, t_shell *mini)
{
	if (validate_input_syntax(input, mini))
		return (1);
	if (prepare_command_structs(mini, *input))
		return (1);
	if (split_input_by_pipes(*input, mini))
		return (1);
	if (parse_input(mini))
		return (1);
	return (0);
}

/**
 * parse_input - Parses individual command segments into the shell structure.
 *
 * @mini: Pointer to the shell structure containing command and execution data.
 *
 * This function iterates through each command segment generated during input
 * splitting, assigns a unique index to each command, and parses the segment
 * into a command structure. If parsing fails for any command segment, the
 * function cleans up the resources, unlinks heredocs, and sets the shell's
 * exit status to an error state.
 *
 * Returns:
 * - 0 on successful parsing of all commands.
 * - 1 if parsing fails for any command segment.
 */
int	parse_input(t_shell *mini)
{
	int	index;

	index = 0;
	while (mini->cmds[index])
	{
		mini->cmds[index]->cmd_index = index;
		if (parse_cmd_string(mini, mini->cmds[index]))
		{
			mini->exit_stat = 1;
			unlink_all_heredocs(mini);
			clean_cmds(mini->cmds);
			return (1);
		}
		index++;
	}
	return (0);
}

/**
 * parse_cmd_string - Parses a single command string and handles expansion.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure for the command being parsed.
 *
 * Handles variable expansion and iterates through the command string to
 * process redirections, command names, and arguments. Returns 1 on failure or
 * 0 on success.
 */
int	parse_cmd_string(t_shell *mini, t_cmd *cmd)
{
	int	i;
	int	cmd_found;

	i = 0;
	cmd_found = 0;
	if (handle_expand(mini, &cmd))
		return (1);
	i = cmd_string_while(mini, cmd, i, &cmd_found);
	if (i == -1)
		return (1);
	if (cmd_found && (!cmd->args || !cmd->args[0]))
		i = no_args(cmd, i);
	return (0);
}

/**
 * cmd_string_while - Iterates through a command string and processes elements.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the command structure.
 * @i: Current position in the command string.
 * @cmd_found: Pointer to an integer indicating if a command has been found.
 *
 * Processes redirections, command names, and arguments in the command string.
 * Skips whitespace and updates `cmd_found` when a command is identified.
 * Returns the updated position in the string, or -1 on failure.
 */
int	cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found)
{
	while (cmd->seg[i])
	{
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			i = skip_whitespace(cmd->seg, i);
		}
		else if (*cmd_found == 0)
		{
			i = handle_cmd_name(cmd, i);
			if (i == -1)
				return (-1);
			*cmd_found = 1;
		}
		else
		{
			i = handle_cmd_args(mini, cmd, i);
			if (i == -1)
				return (-1);
		}
	}
	return (i);
}
