/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:53:40 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/18 17:31:33 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_in_exe(t_shell *mini, t_cmd *cmd);
int	is_this_builtin_cmd(t_cmd *cmd);

int	built_in_exe(t_shell *mini, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (ft_strcmp(cmd->command, "exit") == 0)
			built_exit(mini, cmd);
		else if (ft_strcmp(cmd->command, "cd") == 0)
			return (built_cd(mini, cmd));
		else if (ft_strcmp(mini->cmds[i]->command, "echo") == 0)
			return (built_echo(cmd));
		else if (ft_strcmp(mini->cmds[i]->command, "env") == 0)
		 	return (built_env(mini));
		else if (ft_strcmp(mini->cmds[i]->command, "pwd") == 0 && !cmd->args[1])
			return (built_pwd(mini));
		else if (ft_strcmp(mini->cmds[i]->command, "unset") == 0)
			return (built_unset(mini, cmd));
		else if (ft_strcmp(mini->cmds[i]->command, "export") == 0)
			return (built_export(mini, cmd));
		i++;
	}
	return (0);
}

int	is_this_builtin_cmd(t_cmd *cmd)
{
	if (ft_strcmp(cmd->command, "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "pwd"))
		return (1);
	else if (ft_strcmp(cmd->command, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->command, "export") == 0)
		return (1);
	return (0);
}