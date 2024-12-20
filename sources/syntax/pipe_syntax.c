/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:06:20 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/20 11:03:35 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_pipes(char **input, t_shell *mini);
static int	check_consecutive_pipes(char *input, t_shell *mini);
static int	check_pipe_error(char *input, int i, t_shell *mini);
static int	check_trailing_pipe(char **input, t_shell *mini);

/* Checks syntax for pipes, i.e. that it is not a the start or that
there are no consecutive pipes. Also handles a trailing pipe */

int	check_pipes(char **input, t_shell *mini)
{
	int	i;

	i = 0;
	while ((*input)[i] == ' ')
		i++;
	if ((*input)[i] == '|' && !check_quotes(*input, i))
	{
		ft_putendl_fd("syntax error near unexpected token ", 2);
		mini->exit_stat = 2;
		return (1);
	}
	if (check_consecutive_pipes(*input, mini))
		return (1);
	if (check_trailing_pipe(input, mini))
		return (1);
	return (0);
}

// checks if there are consecutive pipes without text in between

static int	check_consecutive_pipes(char *input, t_shell *mini)
{
	int	i;
	int	pipe_found;

	i = 0;
	pipe_found = 0;
	while (input[i])
	{
		if (input[i] == '|' && !check_quotes(input, i))
		{
			if (check_pipe_error(input, i, mini))
				return (1);
			pipe_found = 1;
		}
		else if (input[i] == ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}

static int	check_pipe_error(char *input, int i, t_shell *mini)
{
	int	j;

	j = i + 1;
	while (input[j] == ' ')
		j++;
	if (input[j] == '|' && !check_quotes(input, j))
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putendl_fd(&input[i], 2);
		mini->exit_stat = 2;
		return (1);
	}
	return (0);
}

// Checks for trailing pipe

static int	check_trailing_pipe(char **input, t_shell *mini)
{
	int		i;
	char	*updated_input;

	i = ft_strlen(*input) - 1;
	while (i >= 0 && (*input)[i] == ' ')
		i--;
	if (i >= 0 && (*input)[i] == '|' && !check_quotes(*input, i))
	{
		updated_input = handle_trailing_pipe(*input);
		if (!updated_input)
		{
			ft_putendl_fd("syntax error: unexpected end of input", 2);
			mini->exit_stat = 2;
			*input = NULL;
			return (1);
		}
		*input = updated_input;
	}
	return (0);
}

/*static char	*handle_trailing_pipe(char *input)
{
	char	*additional_input;
	char	*updated_input;

	additional_input = NULL;
	updated_input = NULL;
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
		{
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
		free(additional_input);
	}
} */
