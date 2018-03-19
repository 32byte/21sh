/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 10:35:33 by lportay           #+#    #+#             */
/*   Updated: 2018/03/16 11:43:43 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//Display some help
// n opt ?

void		d_opt(t_proc *p, t_ctx *ctx)
{
	t_dlist		*tmp;
	unsigned	offset;

	tmp = ctx->hist.list;
	ft_dlstend(&tmp);
	if (!ft_isnumber(p->argv[2]) || ft_atoi(p->argv[2]) < (int)T_HISTENTRY(tmp->data)->index || (offset = ft_atoi(p->argv[2])) >= ctx->hist.index)
		asprintf(&p->data.str, "221sh: history: %s: history position out of range\n", p->argv[2]);
	else
	{
		while (T_HISTENTRY(tmp->data)->index != offset && tmp->prev)
			tmp = tmp->prev;
		if (tmp->prev)
			ft_dlstremove(&tmp, &del_histentry);
	}
}

void		a_opt(t_proc *p, t_ctx *ctx)
{
	t_dlist *tmp;

	tmp = ctx->hist.list->next;
	while (T_HISTENTRY(tmp->data)->index > ctx->hist.first_entry && tmp->next)
		tmp = tmp->next;
	if (T_HISTENTRY(tmp->data)->index < ctx->hist.first_entry)
		tmp = tmp->prev;
	if (p->argv[2])
		save_history(p->argv[2], tmp, O_APPEND);
	else
		save_history(ft_astr_getval(ctx->locals, "HISTFILE"), tmp, O_APPEND);
	ctx->hist.first_entry = ctx->hist.index;
}

//Need Tests

void	r_opt(t_proc *p, t_ctx *ctx)
{
	if (p->argv[2])
		init_hist(&ctx->hist, p->argv[2]);
	else
		init_hist(&ctx->hist, ft_astr_getval(ctx->locals, "HISTFILE"));
}

void	w_opt(t_proc *p, t_ctx *ctx)
{
	if (p->argv[2])
		save_history(p->argv[2], ft_dlstlast(ctx->hist.list), O_APPEND);
	else
		save_history(ft_astr_getval(ctx->locals, "HISTFILE"), ft_dlstlast(ctx->hist.list), O_APPEND);
		
}

int					ft_history(t_proc *p, t_ctx *ctx)
{

	p->type = BU_STR;
	if (p->argv[1] == NULL)
		p->data.str = dump_history(ctx->hist.list->next, ctx->hist.index);
	else if (ft_isnumber(p->argv[1]) && p->argv[2] == NULL)
		p->data.str = dump_history(ctx->hist.list->next, ft_atoi(p->argv[1]));
	else if (!ft_strcmp(p->argv[1], "-c") && p->argv[2] == NULL) 
		ft_dlstdel(&ctx->hist.list->next, &del_histentry);
	else if (!ft_strcmp(p->argv[1], "-d") && p->argv[2])
		d_opt(p, ctx);
	else if (!ft_strcmp(p->argv[1], "-a"))
		a_opt(p, ctx);

	return (0);
}
