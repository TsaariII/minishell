#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

# define TMP_S "/tmp/heredoc"
# define TMP_EXT ".tmp"

extern int g_sig;

//# include </usr/include/linux/signal.h>

typedef enum e_redir_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}	t_redir_type;

typedef struct s_hd
{
	char	*cmd_str;
	char	*heredoc_str;
	char	*base;
	char	*mid;
	char	*full;
}	t_hd;

typedef struct s_vdata
{
	char	*value;
	char	**expan;
	char	*temp;
	char	*name;
}	t_vdata;

typedef struct s_expand
{
	int		sgl;
	int		dbl;
	int		i;
	char	*name;
	char	*value;
	int		start;
	int		len;
}	t_expand;

// linked list for redirects in each command struct
typedef struct s_redir
{
	char			*file;
	char			*delimiter;
	t_redir_type	type;
	bool			expand;
	int				node_ind;
	char			*heredoc_name;
	int				heredoc_index;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	*seg;
	char	*command;
	char	*cmd_path;
	int		cmd_index;
	char	**args;
	int		a_num;
	t_redir	*redir_head;
	t_redir	*redir_tail;
	int		fd_in;
	int		fd_out;
	int		cmd_exit;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	struct s_env	*next;
	char			*value;

}	t_env;

typedef struct s_shell
{
	t_cmd	**cmds;
	t_env	*env;
	int		cmd_count;
	char	**pending;
	int		*pids;
	int		**pipes;
	int		stdin_saved;
	int		stdout_saved;
	int		exit_stat;
} t_shell;

void printer(t_shell *mini);

/*built_in*/
	/*echo.c*/
int		built_echo(t_cmd *cmd);
	/*exit.c*/
int		built_exit(t_shell *mini, t_cmd *cmd);
	/*export.c*/
int		built_export(t_shell *mini, t_cmd *cmd);
	/*pwd.c*/
int		built_pwd(t_shell *mini);
	/*unset.c*/
int		built_unset(t_shell *mini, t_cmd *cmd);

/*built_in/env*/
	/*env.c*/
char	**env_to_array(t_env *env);
int		built_env(t_shell *mini, t_cmd *cmd);

	/*env_handling*/
char	**copy_env(char **envp);
t_env	*list_env(char **envp);
void	to_alphabetical(char **array);

	/*env_ll*/
t_env	*add_node(char *env);
t_env	*create_node(void);
void	ft_env_lstadd_back(t_env **lst, t_env *new);

	/*env_utils.c*/
int		set_value(t_env *node, char *value);
int		fill_node(t_env *node, char *name, char *value);

/*built_in/cd*/
	/*cd.c*/
int		built_cd(t_shell *mini, t_cmd *cmd);
int		update_pwd(t_env *env, char *wd, char **oldpwd, int n);

	/*cd_utils.c*/
int		get_oldpwd(t_env *env, char **pwd);
int		old_pwd(t_shell *mini, t_cmd *cmd);

/*commands*/
	/*cmd_array.c*/
int		count_pipes(char *line);
int		prepare_command_structs(t_shell *mini, char *input);
void	initialize_command_struct(t_cmd *cmd);

/*executor*/
	/*child_process.c*/
int		fork_and_execute(t_shell *mini, t_cmd *cmd, int i);
void	close_unused_fds(t_shell *mini, int i);

	/*child_process.c*/
int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_shell *mini, t_cmd *cmd, int count, int i);
int		dup2_and_close(int old_fd, int new_fd);

	/*fd_handlers.c*/
int		save_fds(t_shell *mini);
int		reset_fds(t_shell *mini);

	/*find_cmd_path.c*/
int		get_cmd_path(t_shell *mini, t_cmd *cmd);

	/*handle_builtins.c*/
int		built_in_exe(t_shell *mini, t_cmd *cmd);
int		is_this_built(char *str);

	/*pipeline.c*/
int		execute_pipeline(t_shell *mini);

	/*pipeline_utils.c*/
int		create_pipes(t_shell *mini);
int		dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd);
void	close_fds_and_pipes(t_shell *mini, int i);
void	wait_children(t_shell *mini);

/*parser*/
	/*expand.c*/
int		handle_expand(t_shell *mini, t_cmd **cmd);

	/*expand_no_expand.c*/
int		no_expanding(t_shell *mini, char *str, t_expand *arg);

	/*expand_quoted.c*/
int		in_quotes(t_shell *mini, char *str, int i, t_expand *arg);
int		we_have_heredoc(t_expand *arg, char *str, int n);

	/*expand_unquoted.c*/
int		no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);

	/*expand_utils.c*/
int		we_have_dollar(t_shell *mini, t_expand *arg, char *str);
int 	oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg);

	/*expand_utils2.c*/
int		tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg);
int		handle_value(t_shell *mini, t_vdata *data);
char	*get_value(t_env *env, char *name);
char	*ft_strjoin_char(char *str, char c);

	/*expand_utils3.c*/
int		init_expansion(t_expand *arg, char **expan);
int		the_arg(t_expand *arg, int i);
void	what_quote(char *str, t_expand *arg);
int		handle_question(t_shell *mini, char *str, char **expan, t_expand *arg);
int		new_result(t_expand *arg, char *temp);

	/*handle_cmd_args.c*/
int		handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);

	/*handle_cmd_args_utils.c*/
int		skip_whitespace(char *str, int i);
int		arg_no_quotes(t_cmd *cmd, t_expand *arg, int i);
int		arg_in_quotes(char *str, int i, t_expand *arg);
int		append_to_array(t_cmd *cmd, char *arg, int *index);

	/*handle_cmd_args_utils2.c*/
int		count_args(t_cmd *cmd, int i);
int		count_if_redirection(t_cmd *cmd, int i);
int		only_redirect(char *str, int i);
int		init_args_array(t_cmd *cmd, int i);

	/*handle_cmd_name.c*/
int		handle_cmd_name(t_cmd *cmd, int i);

	/*handle_redirections.c*/
int		handle_redirections(t_shell *mini, t_cmd *cmd, int i);

	/*handle_redirs_utils.c*/
bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_shell *mini, t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

	/*heredoc.c*/
int		open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd);

	/*heredoc_expand*/
int		heredoc_expander(t_shell *mini, char **line);
int 	check_expand(t_shell *mini, t_cmd *cmd, char **line, int fd);

	/*heredoc_file.c*/
int		generate_hd_file(t_cmd *cmd);
	/*parser.c*/
int	parse_and_validate_input(char **input, t_shell *mini);
int	parse_input(t_shell *mini);
int	parse_cmd_string(t_shell *mini, t_cmd *cmd);
int	cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found);

	/*parser_utils.c*/
int		no_args(t_cmd *cmd, int i);
bool	is_empty_command(t_cmd *cmd, int i);
int		add_char(char *str, t_expand *arg);

	/*split_inputs.c*/
int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *seg);

	/*find_cmd_path.c*/
int		get_cmd_path(t_shell *mini, t_cmd *cmd);

/*redirection*/
	/*get_filename.c*/
int		parse_filename(t_cmd *cmd, int i, char **filename);
//int		get_filename_length(t_cmd *cmd, int i, bool in_quotes);

	/*redir_ll*/
t_redir	*list_redir(void);
t_redir	*redir_add_node(void);
void	redir_lstadd_back(t_redir **lst, t_redir *new);
void	redir_update_tail(t_cmd *cmd);
int		redirll_head_tail(t_cmd *cmd);

	/*open_files.c*/
int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);

	/*redirector.c*/
int		resolve_fd(t_cmd *cmd);

/*syntax*/
	/*pipe_syntax*/
int		check_pipes(char **input, t_shell *mini);

	/*redirection_syntax.c*/
int		check_redirects(char *input, t_shell *mini);

	/*syntax_checker.c*/
int		validate_input_syntax(char **input, t_shell *mini);
int		check_quotes(char *input, int limit);
int		check_non_whitespace(char *str);

	/*trailing_pipe.c*/
char	*handle_trailing_pipe(char *input);

/*utils*/
	/*exit_handler.c*/
void	exit_for_failure(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	exit_for_single_cmd(t_shell *mini, int exit_status);
void	hd_free(t_expand *arg, char *expan);

	/*cleaners.c*/
void	mini_cleaner(t_shell *mini);
void	cleaner_for_failure(t_shell *mini);
void	cleaner_for_success(t_shell *mini);
void	clean_cmds(t_cmd **cmds);

	/*freeing*/
void	ft_free_int_arr(int **array);
void 	ft_free_int_arr_with_size(int **array, int size);
void	error(t_shell * mini, char *str);
void	clean_env(t_env *ll, char **array);
void	clean_redir(t_redir *head);

/*signals.c*/
void	init_sig(void);
void	sig_reseted(void);
void	sig_handler_changer(void);
void	sig_heredoc();
void	sig_handler_hd(int signal);

void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status);
int	check_special_cases(t_cmd *cmd);
int	check_for_directory(t_cmd *cmd);

#endif
