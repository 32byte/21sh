/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_exec_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 20:48:08 by lportay           #+#    #+#             */
/*   Updated: 2018/04/22 18:14:47 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"
#include <errno.h>

int						get_exit_code(int status, pid_t pid)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status) | JOB_CMP);
	else if (WIFSIGNALED(status))
	{
		ft_dprintf(STDERR_FILENO, "%d received signal %d\n",
					pid, WTERMSIG(status));
		return ((WTERMSIG(status) + 128) | JOB_CMP);
	}
	return (-1);
}

static void				l_wait_for_job(t_job *j)
{
	pid_t				pid;
	int					status;

	while ((pid = waitpid(-j->pgid, &status, 0)) != -1)
	{
		if (pid == j->pgid)
			j->status = get_exit_code(status, pid) & 0xFF;
	}
	if (pid == -1 && errno != ECHILD)
	{
		dump_err(errno == EINTR ? EWAITINTR : EWAITCTX);
		j->status = (-42 & 0xFF);
	}
	j->status = JOB_CMP | (j->status & 0xFF);
}

static int				wait_err(t_job *j)
{
	int					status;

	waitpid(j->procs->pid, &status, WUNTRACED);
	j->status = JOB_CMP | (-42 & 0xFF);
	return (1);
}

int						job_exec_loop(t_job **job, t_ctx *ctx)
{
	t_job				*j;
	t_job				*n;
	int					ret;

	j = *job;
	if ((ret = ((j->procs->next != NULL) ?
					job_pipe(j) : job_one(j, ctx))) == 1)
		return (1);
	else if (ret == -1)
		return (wait_err(j));
	l_wait_for_job(j);
	if ((n = job_getnextexec(j)) == NULL)
		j->parent->status = (JOB_DON | (j->status & 0xFF));
	else
		j = n;
	*job = j;
	return (0);
}
