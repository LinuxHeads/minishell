# minishell

expander does not work on heredoc input
minor mem leaks
this case needs a bit of work:
  cat <minishell.h <<HERE <missing <<DOC | echo oi

this cae too:
doesntexist
$EMPTY
echo $?

this fucker too:
  echo - "" "  " hello
