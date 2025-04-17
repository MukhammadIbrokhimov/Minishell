#include "../includes/sadaf.h"

int handle_heredoc(char *delimiter, t_shell *shell)
{
    int fd[2];
    int pid;
    int status;
    
    if (pipe(fd) == -1)
    {
        ft_perror("heredoc");
        return (-1);
    }
    
    pid = fork_safely();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        shell->in_heredoc = 1;
        close(fd[0]);
        process_heredoc_input(fd[1], delimiter, shell);
        close(fd[1]);
        exit(0);
    }
    
    close(fd[1]);
    waitpid(pid, &status, 0);
    
    if (WIFSIGNALED(status))
    {
        g_signal_received = 1;
        close(fd[0]);
        return (-1);
    }
    
    return (fd[0]);
}

void process_heredoc_input(int fd, char *delimiter, t_shell *shell)
{
    char *line;
    
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        
        // Remove newline if present
        char *newline = strchr(line, '\n');
        if (newline)
            *newline = '\0';
        
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    
    shell->in_heredoc = 0;
}