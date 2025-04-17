#include "../../includes/sadaf.h"

void runcmd(t_cmd *cmd, t_shell *shell)
{
    if (!cmd)
        exit(0);
    
    if (cmd->type == EXEC)
        execute_command((t_execcmd *)cmd, shell);
    else if (cmd->type == REDIR)
        handle_redirections((t_redircmd *)cmd, shell);
    else if (cmd->type == PIPE)
        handle_pipe((t_pipecmd *)cmd, shell);
    else if (cmd->type == LIST)
        handle_list((t_listcmd *)cmd, shell);
    else if (cmd->type == BACK)
        handle_background((t_backcmd *)cmd, shell);
    else
    {
        ft_error("Unknown command type");
        exit(1);
    }
    exit(shell->exit_status);
}

void execute_command(t_execcmd *ecmd, t_shell *shell)
{
    char *path;
    char **env_array;
    int exit_code;
    
    if (!ecmd->argv[0])
        exit(0);
    
    expand_variables(ecmd, shell);
    
    if (is_builtin(ecmd->argv[0]))
    {
        exit_code = exec_builtin(ecmd, shell);
        exit(exit_code);
    }
    
    path = find_command_path(ecmd->argv[0], shell);
    if (!path)
    {
        ft_putstr_fd("sadaf: command not found: ", 2);
        ft_putstr_fd(ecmd->argv[0], 2);
        ft_putstr_fd("\n", 2);
        exit(127);
    }
    
    env_array = env_to_array(shell->env_list);
    if (execve(path, ecmd->argv, env_array) < 0)
    {
        free(path);
        cleanup_tokens(env_array);
        ft_perror("execve");
        exit(1);
    }
    free(path);
    cleanup_tokens(env_array);
}

// void handle_redirections(t_redircmd *rcmd, t_shell *shell)
// {
//     int fd;
    
//     close(rcmd->fd);
    
//     if (rcmd->mode & O_RDONLY)
//         fd = open(rcmd->file, rcmd->mode);
//     else
//         fd = open(rcmd->file, rcmd->mode, 0644);
    
//     if (fd < 0)
//     {
//         ft_putstr_fd("sadaf: ", 2);
//         ft_putstr_fd(rcmd->file, 2);
//         ft_putstr_fd(": ", 2);
//         ft_perror("");
//         exit(1);
//     }
    
//     runcmd(rcmd->cmd, shell);
// }

void handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
    int fd;
    
    if (rcmd->file)
    {
        // Regular file redirection
        close(rcmd->fd);
        
        if (rcmd->mode & O_RDONLY)
            fd = open(rcmd->file, rcmd->mode);
        else
            fd = open(rcmd->file, rcmd->mode, 0644);
        
        if (fd < 0)
        {
            ft_putstr_fd("sadaf: ", 2);
            ft_putstr_fd(rcmd->file, 2);
            ft_putstr_fd(": ", 2);
            ft_perror("");
            exit(1);
        }
        
        if (dup2(fd, rcmd->fd) == -1)
        {
            ft_perror("sadaf: dup2");
            exit(1);
        }
        
        close(fd);
    }
    else
    {
        // This is a heredoc redirection
        // The file descriptor is already set
        if (dup2(rcmd->fd, STDIN_FILENO) == -1)
        {
            ft_perror("sadaf: dup2");
            exit(1);
        }
        close(rcmd->fd);
    }
    
    runcmd(rcmd->cmd, shell);
}

void handle_pipe(t_pipecmd *pcmd, t_shell *shell)
{
    int p[2];
    int pid1;
    int pid2;
    
    if (pipe(p) < 0)
        ft_error("pipe");
    
    pid1 = fork_safely();
    if (pid1 == 0)
    {
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        runcmd(pcmd->left, shell);
    }
    
    pid2 = fork_safely();
    if (pid2 == 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        runcmd(pcmd->right, shell);
    }
    
    close(p[0]);
    close(p[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    wait(NULL);
}

void handle_list(t_listcmd *lcmd, t_shell *shell)
{
    int pid;
    int status;
    
    pid = fork_safely();
    if (pid == 0)
        runcmd(lcmd->left, shell);
    
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    
    runcmd(lcmd->right, shell);
}

void handle_background(t_backcmd *bcmd, t_shell *shell)
{
    int pid;
    
    pid = fork_safely();
    if (pid == 0)
        runcmd(bcmd->cmd, shell);
}