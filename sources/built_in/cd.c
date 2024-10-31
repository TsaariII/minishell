/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:23:40 by nzharkev          #+#    #+#             */
/*   Updated: 2024/10/31 17:44:28 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Changes the current directory. Accepts a relative or absolute path as an argument.*/

void	built_cd(char **args)
{
	const char	*path;

	path = args[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd HOME not set\n", 1);
			return ;
		}
	}
	if (chdir(path) != 0)
		perror("cd");
}
