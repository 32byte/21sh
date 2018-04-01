/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_one.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 11:37:17 by vbastion          #+#    #+#             */
/*   Updated: 2018/04/01 17:57:23 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static int				fork_do(t_proc *p)
{
	pid_t				pid;

	if ((pid = fork()) < 0)
	{
		ft_putstr_fd("21sh: fork: could not fork\n", STDERR_FILENO);
		return (1);
	}
	else if (pid == 0)
		proc_exec(p);
	else
		p->pid = pid;
	return (0);
}

static void				restore_fds(t_ctx *ctx)
{
	dup2(ctx->std_fd[0], STDIN_FILENO);
	dup2(ctx->std_fd[1], STDOUT_FILENO);
	dup2(ctx->std_fd[2], STDERR_FILENO);
}

int						job_one(t_job *j, t_ctx *ctx)
{
	t_proc				*p;
	int					ret;

	ret = 0;
	p = j->procs;
	if (ctx->set & BU_SET_ONCMD)
		proc_print(p, ctx);
	if (do_redir(p->redirs) == -1)
	{
		restore_fds(ctx);
		return (1);
	}
	if (p->asmts != NULL
		&& p->argv[0] == NULL)
		prefork_assign(ctx, p->asmts);
	else if (p->argv[0] != NULL)
		prepare_fork(p, ctx, 0);
	if (fork_do(p) == 1)
		ret = 1;
	restore_fds(ctx);
	return (ret);
}
