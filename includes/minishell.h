#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>

// struct for input segments, split based on pipes

typedef struct s_command
{
	char	*segment;
	char	*command;
	char	**args;
	int		args_count;
	char	*redirect_in;
	char	*redirect_out;
	int		redirect_type; //??
	char	**env_vars; //??
	int		env_var_count; //??
	char	*append;
	bool	heredoc;
	char	*heredoc_delim;
	char	*heredoc_content;
	int		exit_status;
}	t_command;

// redirect types

typedef enum e_redir_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}	t_redir_type;


typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

// linked list for redirects in each command struct
// Nikolai please create the functions for adding these nodes to a list

typedef struct s_redir
{
	char				*file;
	char				*delimiter;
	t_redir_type		*type;
	struct s_redir		*next;
}	t_redir;

// struct to store general information

typedef struct s_shell
{
	t_env		*env;
	char		**pending;
	t_command	**cmds;
	int			exit_stat;
}	t_shell;

int			validate_input_syntax(char *input);
int			is_in_quotes(char *input, int i);
int			check_consecutive_pipes(char *input);
int			check_pipes(char *input);
int			check_redirects(char *input);
int			validate_redirect(char *input, int *i, char *type);
int			count_pipes(char *input);
t_command	**allocate_cmd_array(int command_count);
int			split_input_by_pipes(char *input, t_shell *sh);
int			parse_input(t_shell *sh);
int			parse_and_validate_input(char *input, t_shell *sh);
int			prepare_command_structs(t_shell *sh, char *input);
void		init_sig(void);
int			split_input_by_pipes(char *input, t_shell *sh);
char		*ft_strndup(const char *src, size_t n);
bool		is_redirection(char *str, int index);
int			handle_redirect_in(char *str, int *i, t_shell *sh, int index);
int			handle_redirect_out(char *str, int *i, t_shell *sh, int index);
int			handle_heredoc(char *str, int *i, t_shell *sh, int index);
int			handle_append(char *str, int *i, t_shell *sh, int index);
int			ft_isspace(char c);

#endif
