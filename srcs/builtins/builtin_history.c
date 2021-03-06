/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 10:35:33 by lportay           #+#    #+#             */
/*   Updated: 2018/04/23 14:29:48 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void		d_opt(t_proc *p, t_ctx *ctx)
{
	t_dlist		*tmp;
	unsigned	offset;

	tmp = ctx->hist.list;
	ft_dlstend(&tmp);
	if (!ft_isnumber(p->argv[2]) ||
	ft_atoi(p->argv[2]) < (int)T_HISTENTRY(tmp->data)->index ||
	(offset = ft_atoi(p->argv[2])) >= ctx->hist.index)
		ft_dprintf(STDERR_FILENO, BU_HI_EEORNG, p->argv[2]);
	else
	{
		while (T_HISTENTRY(tmp->data)->index != offset && tmp->prev)
			tmp = tmp->prev;
		if (tmp->prev)
			ft_dlstremove(&tmp, &del_histentry);
	}
}

static void		a_opt(t_proc *p, t_ctx *ctx)
{
	t_dlist *tmp;

	tmp = ctx->hist.list->next;
	while (T_HISTENTRY(tmp->data)->index > ctx->hist.first_entry && tmp->next)
		tmp = tmp->next;
	if (T_HISTENTRY(tmp->data)->index < ctx->hist.first_entry)
		tmp = tmp->prev;
	if (p->argv[2])
		save_history(p->argv[2], tmp, O_APPEND, 0);
	else
		save_history(ft_astr_getval(ctx->locals, "HISTFILE"), tmp, O_APPEND, 0);
	ctx->hist.first_entry = ctx->hist.index;
}

static int		r_opt(t_proc *p, t_ctx *ctx)
{
	if (p->argv[2])
		return (init_hist(&ctx->hist, p->argv[2], O_RDONLY));
	else
		return (init_hist(&ctx->hist, ft_astr_getval(ctx->locals, "HISTFILE"),
								O_RDONLY));
}

static void		w_opt(t_proc *p, t_ctx *ctx)
{
	if (p->argv[2])
		save_history(p->argv[2], ft_dlstlast(ctx->hist.list), O_APPEND, 0);
	else
		save_history(ft_astr_getval(ctx->locals, "HISTFILE"),
				ft_dlstlast(ctx->hist.list), O_APPEND, 0);
}

int				ft_history(t_proc *p, t_ctx *ctx)
{
	p->type = BUILTIN;
	if (p->argv[1] == NULL || (ft_isnumber(p->argv[1]) && p->argv[2] == NULL))
		print_history(ctx->hist.list->next, ctx->hist.index, p->argv[1]);
	else if (!ft_strcmp(p->argv[1], "-c") && p->argv[2] == NULL)
		ft_dlstdel(&ctx->hist.list->next, &del_histentry);
	else if (!ft_strcmp(p->argv[1], "-d"))
	{
		if (p->argv[2])
			d_opt(p, ctx);
		else
		{
			ft_dprintf(STDERR_FILENO, "%s%s", BU_HI_ENUMARG, BU_HI_USAGE);
			return (1);
		}
	}
	else if (!ft_strcmp(p->argv[1], "-a"))
		a_opt(p, ctx);
	else if (!ft_strcmp(p->argv[1], "-r"))
		return (r_opt(p, ctx));
	else if (!ft_strcmp(p->argv[1], "-w"))
		w_opt(p, ctx);
	else
		return (usage_history(p->argv[1]));
	return (0);
}
