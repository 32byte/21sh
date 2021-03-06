/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/19 14:45:26 by vbastion          #+#    #+#             */
/*   Updated: 2018/04/22 20:21:13 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"
#include <errno.h>

static int				open_pipe(t_proc *p, int *pipes)
{
	if (p->next != NULL)
	{
		if (pipe(pipes) < 0)
			return (-1);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[1]);
		pipes[1] = -1;
		move_fd_up(pipes);
	}
	else
		dup2(get_ctxaddr()->std_fd[1], STDOUT_FILENO);
	return (0);
}

static void				prepare_forking(t_ctx *ctx, t_proc *p)
{
	if (p->asmts != NULL && p->argv[0] == NULL)
		prefork_assign(ctx, p->asmts);
	else if (p->argv[0] != NULL)
		prepare_fork(p, ctx, 1);
}

static int				subshell_waitforchild(pid_t pgid, pid_t last)
{
	pid_t				pid;
	int					status;
	int					ret;

	while ((pid = waitpid(-pgid, &status, 0)) != -1)
	{
		if (pid == last)
			ret = get_exit_code(status, pid);
	}
	if (pid == -1 && errno != ECHILD)
	{
		dump_err(errno == EINTR ? EWAITINTR : EWAITCTX);
		return (1);
	}
	return (ret);
}

static void				subshell_execloop(t_proc *p, int *pipes, pid_t *last,
											pid_t pgid)
{
	t_ctx				*ctx;
	int					fd[7];

	ft_bzero(fd, sizeof(int) * 7);
	ctx = get_ctxaddr();
	restore_fds(ctx);
	if (open_pipe(p, pipes) == -1)
	{
		ft_dprintf(STDERR_FILENO, EFILEDESC_STR);
		exit(-42);
	}
	if (do_redir(p->redirs, fd) == -1)
		p->is_err = 1;
	if ((p->status & JOB_CMP) == 0)
		prepare_forking(ctx, p);
	if ((*last = fork_do(p, pgid, pipes)) == -1)
	{
		ft_dprintf(STDERR_FILENO, EFILEDESC_STR);
		exit(-42);
	}
	clear_lfd(fd);
}

int						exec_pipe(t_job *j)
{
	t_proc				*p;
	int					*pipes;
	pid_t				pgid;
	pid_t				last;

	pgid = getpid();
	if (job_setpgid(pgid, pgid) == -1)
		exit(-42);
	else if (job_setpgrp(pgid) == -1)
		exit(-42);
	p = j->procs;
	pipes = (int[]){-1, -1, -1, -1};
	while (p != NULL)
	{
		subshell_execloop(p, pipes, &last, pgid);
		p = p->next;
	}
	exit(subshell_waitforchild(pgid, last));
	return (0);
}
