/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:35:50 by jose-cad          #+#    #+#             */
/*   Updated: 2025/12/01 12:37:43 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <termios.h>
# include "libft.h"

extern volatile sig_atomic_t	g_signal;

typedef enum e_token_type
{
	TOKEN_ERROR,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_REDIR_ERR,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				was_quoted;
}	t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC,
	NODE_REDIR_ERR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;
	int				*argv_quoted;
	char			*filename;
	int				heredoc_fd;
	int				heredoc_expand;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env_var
{
	char	*key;
	char	*value;
}	t_env_var;

typedef struct s_shell
{
	t_dlist			*env_list;
	t_dlist			*token_list;
	struct s_ast	*ast;
	int				exit_status;
	int				is_child;
	int				saved_fds[3];
}	t_shell;

typedef struct s_buf
{
	char	*str;
	size_t	len;
	size_t	cap;
}	t_buf;

typedef struct s_words
{
	char	**argv;
	int		*quoted;
	int		len;
	int		cap;
}	t_words;

typedef struct s_split
{
	t_buf	buf;
	t_words	*out;
	t_shell	*shell;
	int		has_word;
	int		quoted;
}	t_split;

int			alloc_argv(t_ast *cmd, t_dlist *tokens);
int			apply_redirections(t_ast *node, t_shell *shell);
void		ast_free(t_ast *node);
void		append_literal(char *word, int *i, t_buf *buf);
void		buf_append(t_buf *buf, const char *s, size_t n);
t_ast		*ast_new_node(t_node_type type, t_ast *left, t_ast *right);
int			builtin_cd(char **argv, t_shell *shell);
int			builtin_echo(char **argv);
int			builtin_env(t_shell *shell);
int			builtin_exit(t_ast *node, t_shell *shell);
int			builtin_export(char **argv, t_shell *shell);
int			builtin_pwd(t_shell *shell);
int			builtin_unset(char **argv, t_shell *shell);
void		clean_exit_child(t_shell *shell, int status);
void		cleanup_shell(t_shell *shell);
void		close_heredoc_fds(t_ast *node);
void		close_saved_fds(t_shell *shell);
void		close_std_fds(void);
t_env_var	*create_env_var(char *key_value_pair);
t_token		*create_operator_token(char *line, int *i);
t_ast		*create_redir_node(t_token_type type);
t_token		*create_quoted_token(t_token_type type, char *value,
				int was_quoted);
t_token		*create_token(t_token_type type, char *value);
char		*dup_slice(const char *s, size_t len);
int			env_list_set_var(t_dlist **env_list, char *assignment);
char		**env_list_to_array(t_dlist *env_list);
int			exec_and_or(t_ast *node, t_shell *shell);
int			expand_argv(t_ast *cmd, t_shell *shell);
int			expand_one(char *word, t_words *out, t_shell *shell);
int			exec_builtin(t_ast *node, t_shell *shell);
int			exec_builtin_parent(t_ast *node, t_shell *shell);
int			exec_pipe(t_ast *left, t_ast *right, t_shell *shell);
int			exec_pipeline(t_ast *node, t_shell *shell);
int			exec_simple_cmd(t_ast *node, t_shell *shell);
int			exec_subshell(t_ast *node, t_shell *shell);
int			execute_ast(t_ast *node, t_shell *shell);
int			execute_forked_cmd(t_ast *node, t_shell *shell);
char		*expand_heredoc_line(char *line, t_shell *shell);
char		*expand_variable(char *word, int *i, t_shell *shell);
int			expand_wildcards(char ***argv_ptr, int *quoted);
char		*expand_word(char *word, t_shell *shell);
char		*extract_quoted_word(char *line, int *i);
char		*extract_word(char *line, int *i);
char		*find_command_path(const char *cmd, t_shell *shell);
void		free_char_array(char **arr);
void		free_env_var(void *content);
void		free_words(t_words *words);
void		free_tokens(void *content);
char		**ft_lst_to_array(t_list *list);
char		*ft_strjoin3(char const *s1, char const *s2, char const *s3);
t_ast		*get_command_node(t_ast *node);
char		*get_env_value(char *key, t_shell *shell);
t_list		*get_glob_matches(char *pattern);
int			get_var_name_len(char *str);
int			handle_assignment_only(t_ast *node, t_shell *shell);
int			handle_empty_after_expand(t_ast *node, t_shell *shell,
				t_ast *cmd_node);
int			handle_fork_error(int *pipe_fds, pid_t pid_to_wait);
int			handle_heredocs(t_ast *node, t_shell *shell);
int			handle_redirection_token(t_dlist **tokens, t_ast **cmd_node_ptr);
int			handle_heredoc_interrupt(int *pipe_fds);
int			handle_redirect_only(t_ast *node, t_shell *shell);
void		handle_sigint(int sig);
int			has_redirections(t_ast *node);
void		handle_heredoc_sigint(int sig);
void		init_shell(t_shell *shell, char **envp);
int			is_builtin(t_ast *node);
int			is_directory(const char *path);
int			is_empty_command(char **argv);
int			is_glob_match(const char *pattern, const char *text);
int			is_operator(char *line, int i);
int			is_quote(char c);
t_dlist		*lexer(char *line);
int			open_outfile_append(char *filename);
int			open_outfile_trunc(char *filename);
int			open_errfile_trunc(char *filename);
int			open_heredoc_file(int *fds);
int			parent_wait_and_close(pid_t pid_left, pid_t pid_right,
				int *pipe_fds);
t_ast		*parse_and_or(t_dlist **tokens);
t_ast		*parse_command(t_dlist **tokens);
t_ast		*parse_list(t_dlist **tokens);
t_ast		*parse_pipeline(t_dlist **tokens);
t_ast		*parse_subshell(t_dlist **tokens);
t_ast		*parse_tokens(t_dlist *tokens);
int			print_all_env(t_shell *shell);
void		print_signal_output(int status);
int			process_command(t_ast *node, t_shell *shell, t_ast *cmd_node);
int			process_exports(char **argv, t_shell *shell);
char		*process_filename(char *raw_filename);
int			process_quote_state(char c, int *in_squote, int *in_dquote);
int			process_heredoc_line(char *line, t_ast *node,
				int write_fd, t_shell *shell);
t_token		*process_word_token(char *line, int *i);
char		*read_heredoc_line(void);
char		*resolve_redir_file(t_ast *node, t_shell *shell);
void		restore_std_fds(int sv_in, int sv_out, int sv_err);
void		safe_close(int fd);
int			save_std_fds(int *sv_in, int *sv_out, int *sv_err);
int			set_env_var(t_shell *shell, char *key, char *value);
int			set_heredoc_tty(struct termios *orig_tty);
void		setup_heredoc_signals(void);
void		setup_pipe_input(t_shell *shell, int *pipe_fds);
void		setup_pipe_output(t_shell *shell, int *pipe_fds);
void		setup_signals(void);
void		setup_signals_child(void);
void		setup_signals_wait(void);
int			should_expand_wildcards(char **argv, int *argv_quoted);
t_ast		*skip_redirs(t_ast *node);
void		skip_spaces(char *line, int *i);
int			split_init(t_split *s, t_words *out, t_shell *shell, size_t cap);
int			validate_key(char *assignment);
int			words_push(t_words *words, char *word, int quoted);

#endif
