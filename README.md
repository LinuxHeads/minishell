# Minishell

## Known Issues

- **Expander does not work on heredoc input**
- **Minor memory leaks**
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

  3. **Edge case with `echo`:**
     ```sh
     echo - "" "  " hello
     ```
  4. **Tilde '~' expansion:**
      ```sh
      echo ~
      cd ~
      ```
