/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:42:04 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/04 16:03:15 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_redir	*redir_add_node(void)
{
	t_redir	*node;

	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->delimiter = NULL;
	node->file = NULL;
	node->type = 0;
	node->expand = false;
	node->next = NULL;
	return (node);
}

t_redir	*list_redir(void)
{
	t_redir	*ll;
	t_redir	*new;

	ll = NULL;
	new = redir_add_node();
	redir_lstadd_back(&ll, new);
	return (ll);
}

void	redir_update_tail(t_cmd *cmd)
{
	t_redir	*new;
	t_redir	*temp;

	temp = NULL;
	new = redir_add_node();
	redir_lstadd_back(&cmd->redir_head, new);
	temp = cmd->redir_head;
	while (temp->next)
		temp = temp->next;
	cmd->redir_tail = temp;
}

int	redirll_head_tail(t_cmd *cmd)
{
	if (!cmd->redir_head)
	{
		cmd->redir_head = list_redir();
		if (!cmd->redir_head)
		{
			printf("Failed to initialize redirection list\n");
			return (-1);
		}
		cmd->redir_tail = cmd->redir_head;
	}
	else
	{
		redir_update_tail(cmd);
		if (!cmd->redir_tail)
		{
			printf("Failed to allocate memory for new redirection node\n");
			return (-1);
		}
	}
	return (0);
}
