/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_hash.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 19:27:34 by vbastion          #+#    #+#             */
/*   Updated: 2018/02/18 14:04:10 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void		max(size_t *a, size_t b)
{
	if (*a < b)
		*a = b;
}

static void		hashprint(char *key, void *value, void *data)
{
	char		*v;
	size_t		*l;
	char		*str;
	t_qbuf		*buf;
	
	v = (char *)value;
	l = (size_t *)((void **)data)[0];
	buf = (t_qbuf *)((void **)data)[1];
	asprintf(&str, "%-*s    %-*s\n", (int)l[0], key, (int)l[1], v);
	qbuf_add(buf, str);
	ft_strdel(&str);
}

static void		largest(char *key, void *value, void *data)
{
	char		*v;
	size_t		*l;
	size_t		t[2];

	v = (char *)value;
	l = (size_t *)data;
	t[0] = ft_strlen(key);
	t[1] = ft_strlen(v);
	max(l, t[0]);
	max(l + 1, t[1]);
}

static int		print_hash(t_proc *p, t_hash_dict *dict)
{
	size_t		len[2];
	t_qbuf		*buf;
	void		*arg;
	char		*str;

	len[0] = 4;
	len[1] = 5;
	hash_foreach_data(dict, &largest, (void *)len);
	buf = qbuf_new(1 << 8);
	max(len, 4);
	max(len + 1, 5);
	asprintf(&str, "1%-*s    %-*s\n", (int)len[0], "bin:", (int)len[1],
				"path:");
	qbuf_add(buf, str);
	ft_strdel(&str);
	arg = (void *)((void *[]){(void *)len, (void *)buf});
	hash_foreach_data(dict, &hashprint, arg);
	p->data.out = list_create(qbuf_del(&buf));
	return (0);
}

int				ft_hash(t_proc *p, t_ctx *ctx)
{
	p->type = BUILTIN;
	if (ctx->hash->count == 0)
		return (0);
	return (print_hash(p, ctx->hash));
}
