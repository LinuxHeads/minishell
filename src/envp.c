#include "../include/minishell.h"

void add_variable(char *envp, t_env *env)
{
  t_env *new;
  char *name;
  char *value;
  int length;

  name = ft_strchr(envp, '=') + 1;
  value = ft_strdup(name);
  length = ft_strchr(envp, '=') - envp;
  name = malloc(sizeof(char) * length);
  name = ft_strncpy(name, envp, length);
  new = malloc(sizeof(t_env));
  new->name = ft_strdup(name);
  new->value = ft_strdup(value);
  new->next = NULL;
  while (env->next)
    env = env->next;
  env->next = new;
}
int env_length(t_env *env)
{
  int i;

  i = 0;
  while (env)
  {
    i++;
    env = env->next;
  }
  return (i);
}

t_env *init_envp(char **envp)
{
  int i;
  t_env *env;

  i = 0;
  env = malloc(sizeof(t_env));
  while (envp[i])
  {
    add_variable(envp[i], env);
    i++;
  }
	// print_envp(env);
  return (env);
}
char **envp_to_str(t_env *env)
{
  char **envp;
  int i;

  i = 0;
  envp = malloc(sizeof(char *) * (env_length(env) + 1));
  while (env)
  {
    envp[i] = ft_strjoin(env->name, "=");
    envp[i] = ft_strjoin(envp[i], env->value);
    i++;
    env = env->next;
  }
  envp[i] = NULL;
  return (envp);
}

void print_envp(t_env *env)
{
  while (env)
  {
    printf("%s=%s\n", env->name, env->value);
    env = env->next;
  }
}
// void ft_setenv(const char *name, const char *value, char **envp)
// {
//   char *p;

//   if (getenv(name) == NULL)
//   {

//   }
// }