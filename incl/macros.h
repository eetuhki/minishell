#ifndef MACROS_H
# define MACROS_H

// error status
# define SUCCESS 0
# define FAIL 1
# define CMD_EXEC_FAIL 126
# define CMD_NOT_FOUND 127

// delimiter definitions
# define DELIMITERS " |\t<>\'\""
# define REDIRS "<>"
# define QUOTES "\'\""
# define DQUOTE '\"'
# define SQUOTE '\''
# define WHITE_SPACE " \t\n\v\r\f"

// file descriptors
# define STDIN 0
# define STDOUT 1
# define STDERR 2

// fd index
# define READ 0
# define WRITE 1

// heredoc tmp
# define TMP_PATH "srcs/heredoc"
# define TMP_EXT ".tmp"

#endif