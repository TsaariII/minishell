/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/11/26 11:47:58 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	clean_env(t_env *ll, char **array)
{
	t_env *temp;

	if (array)
		ft_free_array(array);
	while (ll != NULL)
	{
		temp = ll;
		free(ll->name);
		free(ll->value);
		ll = ll->next;
		free(temp);
	}
}

static void	clean_redir(t_redir *head)
{
	t_redir *temp;

	if (head)
	{
		while (head != NULL)
		{
			temp = head;
			free(head->file);
			free(head->delimiter);
			head = head->next;
			free(temp);
		}
	}
}

void	clean_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		free(cmds[i]->segment);
		free(cmds[i]->command);
		// Only free if path is not the same as command
		if (cmds[i]->cmd_path && cmds[i]->cmd_path != cmds[i]->command)
			free(cmds[i]->cmd_path);
		ft_free_array(cmds[i]->args);
		if (cmds[i]->redir_head)
			clean_redir(cmds[i]->redir_head);
		free(cmds[i]);
		cmds[i] = NULL;
		i++;
	}
	free(cmds);
	cmds = NULL;
}

void	ft_free_int_arr(int **array)
{
	int	a;

	a = 0;
	if (!array)
		return ;
	while (array[a] != NULL)
	{
		free(array[a]);
		a++;
	}
	free(array);
}

void	cleaner(t_shell *mini)
{
	clean_env(mini->env, mini->pending);
	if (mini->cmds)
		clean_cmds(mini->cmds);
}

void	error(char *str)
{
	ft_putstr_fd(str, 2);
}
