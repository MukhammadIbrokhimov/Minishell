#include "../../includes/sadaf.h"

void ft_error(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
    ft_putstr_fd(msg, STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

void ft_perror(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
    ft_putstr_fd(msg, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(strerror(errno), STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

int	protected_fork(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_perror("fork");
		exit(1);
	}
	return (pid);
}

void *ft_calloc(size_t nmemb, size_t size)
{
    void *ptr;

    ptr = malloc(nmemb * size);
    if (!ptr)
        return (NULL);

    ft_bzero(ptr, nmemb * size);
    return (ptr);
}

void ft_bzero(void *s, size_t n)
{
    memset(s, 0, n);
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

void	cleanup_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
