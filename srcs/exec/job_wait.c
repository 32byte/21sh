/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_wait.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/29 16:06:04 by vbastion          #+#    #+#             */
/*   Updated: 2018/01/30 18:03:28 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void					job_wait(t_job *j)
{
	int					status;
	pid_t				pid;

	do {
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (pid == -1)
			return ; // maybe in an other way?
	} while (proc_chgstat(j, pid, status) == 0 && job_stopped(j) == 0 && job_completed(j) == 0);
//	while (1)
//	{
//		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
//		if (proc_chgstat(j, pid, status)
//			|| job_stopped(j)
//			|| job_completed(j))
//			break ;
//	}
}

void					job_putbg(t_job *j, int continued)
{
	if (continued != 0)
	{
		if (kill(-j->pgid, SIGCONT) < 0)
			ft_putstr_fd("Kill error on kill zombies", STDERR_FILENO);
	}
}

void					job_putfg(t_job *j, int continued, t_env *env)
{
	tcsetpgrp(env->fd, j->pgid);
	if (continued != 0)
	{
		tcsetattr(env->fd, TCSADRAIN, &j->tmodes);
		if (kill(-j->pgid, SIGCONT) < 0)
			ft_putstr_fd("Kill error on kill zombies", STDERR_FILENO);
	}
	job_wait(j);
	tcsetpgrp(env->fd, env->pgid);
	tcgetattr(env->fd, &j->tmodes);
	tcsetattr(env->fd, TCSADRAIN, &env->tnew);
}
