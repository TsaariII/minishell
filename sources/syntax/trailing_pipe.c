/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:32:24 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 10:24:38 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*join_and_free(char *input, char *additional_input)
{
	char	*updated_input;

	updated_input = ft_strjoin(input, additional_input);
	free(additional_input);
	if (!updated_input)
	{
		perror("malloc");
		free(input);
		return (NULL);
	}
	free(input);
	return (updated_input);
}

/* Handles trailing pipe by getting additional input from user
and joining that to the original input string */

char	*handle_trailing_pipe(char *input)
{
	char	*additional_input;

	additional_input = NULL;
	while (1)
	{
		additional_input = readline(">");
		if (!additional_input)
		{
			perror("readline error");
			free(input);
			return (NULL);
		}
		if (check_non_whitespace(additional_input))
			return (join_and_free(input, additional_input));
		free(additional_input);
	}
}
