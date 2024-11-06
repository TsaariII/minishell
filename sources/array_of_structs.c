/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_of_structs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:09:13 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/06 17:26:32 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			prepare_command_structs(char *line, t_command **cmds);
int			count_pipes(char *line);
t_command	*allocate_cmd_array(int command_count);
void		initialize_command_struct(t_command *cmd);

// count number of pipes in input string

int	count_pipes(char *line)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = 0;
	while (line[i])
	{
		if (line[i] == '|' && !is_in_quotes(line, i))
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

// set up an array of structs and initialize it

int	prepare_command_structs(char *line, t_command **cmds)
{
	int	command_count;
	int	i;

	command_count = count_pipes(line) + 1;
	*cmds = allocate_cmd_array(command_count);
	if (!cmds)
	{
		printf("Failed to allocate memory for command array\n");
		return (1);
	}
	i = 0;
	while (i < command_count)
	{
		initialize_command_struct(&(*cmds)[i]);
		i++;
	}
	printf("Allocated %d command struct(s)\n", command_count);
	return (0);
}

//allocate memory for an array of structs

t_command *allocate_cmd_array(int command_count)
{
	t_command *cmds;

	cmds = ft_calloc(sizeof(t_command), command_count);
	if (!cmds)
		return (NULL);
	return (cmds);
}

// initialize all the structs in the array

void	initialize_command_struct(t_command *cmd)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redirect_in = NULL;
	cmd->redirect_out = NULL;
	cmd->redirect_type = 0;
	cmd->append = NULL;
	cmd->heredoc = false;
	cmd->heredoc_delim = NULL;
	cmd->env_vars = NULL;
	cmd->env_var_count = 0;
	cmd->exit_status = 0;
}