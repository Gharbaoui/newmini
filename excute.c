#include "minishell.h"

int fullexcute(t_completecmd **complete, t_fullvar **variables, t_prstatus *prstatus)
{
    t_pipcommand *pcmd;
    t_completecmd *cmpt;

    cmpt = *complete;
    while (cmpt)
    {
        pcmd = expand_current_command(cmpt, *variables);
        excute_one_cmd(pcmd, variables);
        cmpt = cmpt->next;
    }
}

int excute_one_cmd(t_pipcommand *pcmd, t_fullvar **variables)
{
    int count;
    int **pipes;

    count = get_num_subcmds(pcmd);
    if (count > 1){
        alloc_pipes(&pipes, count);
        exec_muti_pipe(pcmd, pipes, variables, 0);
    }
    
    
}

int exec_muti_pipe(t_pipcommand *pcmd, int **pipe, t_fullvar *variables, int index)
{
    int pid;

    pid = fork();
    if (pid == 0)
    {
        
    }
}

void alloc_pipes(int ***pipes, int count)
{
    int i;

    i = -1;
    *pipes = malloc(sizeof(int *) * (count - 1));
    while (++i < count - 1)
        (*pipes)[i] = malloc(sizeof(int) * 2);
}

int get_num_subcmds(t_pipcommand *pcmd)
{
    int i;

    i = 0;
    while (pcmd)
    {
        i++;
        pcmd = pcmd->next;
    }
    return i;
}
