# Minishell

## Known Issues

- **Expander does not work on heredoc input**
- **Problematic cases:**

  1. **Heredoc with multiple redirections:**
     ```sh
     cat <minishell.h <<HERE <missing <<DOC | echo oi
     ```

  2. **Nonexistent command and variable expansion:**
     ```sh
     doesntexist
     $EMPTY
     echo $?
     ```
