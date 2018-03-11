/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/11 13:10:43 by vbastion          #+#    #+#             */
/*   Updated: 2018/03/11 17:11:31 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

char					jc_getstatus(t_job *j)
{
	t_ctx				*ctx;

	ctx = get_ctxaddr(NULL);
	if (ctx->bgs != NULL && (t_job *)ctx->bgs->content == j)
		return ('+');
	else if (ctx->bgs->next != NULL
				&& (t_job *)ctx->bgs->next->content == j)
		return ('-');
	else
		return (' ');
}

void					jc_notify(t_job *j, t_ctx *ctx, int i, int all)
{
	if (j->parent->done == 1)
	{
		if (j->parent->status == 0)
			printf("[%d]%-3c%-24s%s\n", i + 1, jc_getstatus(j), "Done",
					j->parent->command);
		else
			printf("[%d]%-3c%s %-3d%16c%s\n", i + 1, jc_getstatus(j), "Exit",
					j->parent->status, jc_getstatus(j), j->parent->command);
		jc_rmbg(ctx, j);
	}
	else if (all && j->stopped)
		printf("[%d]%-3c%-24s%s\n", i + 1, jc_getstatus(j), "Stopped",
				j->parent->command);
	else if (all)
		printf("[%d]%-3c%-24s%s\n", i + 1, jc_getstatus(j), "Running",
				j->parent->command);
}

void					jc_print(t_ctx *ctx, int all, int verbose)
{
	size_t				i;

	i = 0;
	while (i < ctx->bg_cnt)
	{
		if (ctx->bg_jobs[i] != NULL)
			jc_notify(ctx->bg_jobs[i], ctx, (int)i, all);
		i++;
	}
	if (verbose)
	{
		for (i = 0; i < ctx->bg_cnt; i++)
		{
			t_job *j = ctx->bg_jobs[i];
			if (j == NULL)
				continue ;
			printf("[%zu]   %s (%d)\n", i + 1, j->parent->command, j->pgid);
			for (t_proc *p = j->procs; p != NULL; p = p->next)
			{
				printf("\t");
				for (size_t j = 0; p->argv[j] != NULL; j++)
					printf("%s ", p->argv[j]);
				printf("(%d)\n", p->pid);
			}
		}
	}
}
