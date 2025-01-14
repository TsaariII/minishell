NAME = minishell

SRC_DIR = sources/

SRC = $(SRC_DIR)main.c\
	$(SRC_DIR)built_in/cd/cd.c\
	$(SRC_DIR)built_in/cd/cd_utils.c\
	$(SRC_DIR)built_in/cd/cd_utils2.c\
	$(SRC_DIR)built_in/echo.c\
	$(SRC_DIR)built_in/exit.c\
	$(SRC_DIR)built_in/export/export.c\
	$(SRC_DIR)built_in/export/export_update.c\
	$(SRC_DIR)built_in/pwd.c\
	$(SRC_DIR)built_in/unset.c\
	$(SRC_DIR)built_in/env/env.c\
	$(SRC_DIR)built_in/env/env_handling.c\
	$(SRC_DIR)built_in/env/env_ll.c\
	$(SRC_DIR)built_in/env/env_utils.c\
	$(SRC_DIR)commands/cmd_array.c\
	$(SRC_DIR)executor/child_process.c\
	$(SRC_DIR)executor/child_process_utils.c\
	$(SRC_DIR)executor/cmd_path.c\
	$(SRC_DIR)executor/cmd_path_utils.c\
	$(SRC_DIR)executor/cmd_path_utils2.c\
	$(SRC_DIR)executor/fd_handler.c\
	$(SRC_DIR)executor/handle_builtins.c\
	$(SRC_DIR)executor/pipeline.c\
	$(SRC_DIR)executor/pipeline_utils.c\
	$(SRC_DIR)executor/pipeline_utils2.c\
	$(SRC_DIR)parser/expand.c\
	$(SRC_DIR)parser/expand_quoted.c\
	$(SRC_DIR)parser/expand_unquoted.c\
	$(SRC_DIR)parser/expand_no_expand.c\
	$(SRC_DIR)parser/expand_utils.c\
	$(SRC_DIR)parser/expand_utils2.c\
	$(SRC_DIR)parser/expand_utils3.c\
	$(SRC_DIR)parser/expand_utils4.c\
	$(SRC_DIR)parser/handle_cmd_args.c\
	$(SRC_DIR)parser/handle_cmd_args_utils.c\
	$(SRC_DIR)parser/handle_cmd_args_utils2.c\
	$(SRC_DIR)parser/handle_cmd_name.c\
	$(SRC_DIR)parser/handle_redirections.c\
	$(SRC_DIR)parser/handle_redirs_utils.c\
	$(SRC_DIR)parser/heredoc.c\
	$(SRC_DIR)parser/heredoc_expand.c\
	$(SRC_DIR)parser/heredoc_file.c\
	$(SRC_DIR)parser/parser.c\
	$(SRC_DIR)parser/parser_utils.c\
	$(SRC_DIR)parser/split_input.c\
	$(SRC_DIR)redirection/redir_ll.c\
	$(SRC_DIR)redirection/redirector.c\
	$(SRC_DIR)redirection/open_files.c\
	$(SRC_DIR)redirection/get_filename.c\
	$(SRC_DIR)signals/signal_handlers.c\
	$(SRC_DIR)signals/signals_utils.c\
	$(SRC_DIR)syntax/syntax_checker.c\
	$(SRC_DIR)syntax/redirection_syntax.c\
	$(SRC_DIR)syntax/pipe_syntax.c\
	$(SRC_DIR)syntax/trailing_pipe.c\
	$(SRC_DIR)utils/freeing.c\
	$(SRC_DIR)utils/exit_handler.c\
	$(SRC_DIR)utils/cleaners.c

FLAGS = -Wall -Werror -Wextra -g
LDFLAGS = -lreadline
MAKEFLAGS += --no-print-directory

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft
LIBFT_LINK = -L$(LIBFT_DIR) -lft
LIBFT_INC = -I$(LIBFT_DIR)/includes

OBJ_DIR = objects/
OBJS = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))
OBJ_DIRS = $(sort $(dir $(OBJS)))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIRS) $(OBJS)
		@cc $(OBJS) $(LIBFT_LINK) -o $(NAME) $(LDFLAGS) > /dev/null
		@echo "\n\033[0;32mLet's mini!\033[0m\n"

$(OBJ_DIRS):
		@mkdir -p $(OBJ_DIRS)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
		@cc $(FLAGS) $(LIBFT_INC) -c $< -o $@

$(LIBFT):
		@make -C $(LIBFT_DIR)
		@echo "\n\033[0;32mLibft ready\033[0m\n"

clean:
		@rm -rf $(OBJ_DIR)
		@make -C $(LIBFT_DIR) clean

fclean: clean
		@rm -f $(NAME)
		@make -C $(LIBFT_DIR) fclean
		@echo "\n\033[0;31mAll is gone\033[0m\n"

re: fclean all

.PHONY: all clean fclean re libft
