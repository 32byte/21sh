/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <vbastion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/14 15:59:40 by vbastion          #+#    #+#             */
/*   Updated: 2018/03/24 15:05:53 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_quotes.h"

void				push_str_til(char **str, t_qbuf *buf, char c)
{
	char			*s;

	s = *str + 1;
	while (*s != c)
	{
		if (*s == '\\' && c == '\"' && *(s + 1) == '\"')
			s++;
		qbuf_addc(buf, *s);
		s++;
	}
	*str = s + 1;
}

int					do_expand_quotes(t_list *elem)
{
	t_qbuf			*buf;
	char			*str;

	buf = qbuf_new(1 << 8);
	str = (char *)elem->content;
	while (*str != '\0')
	{
		if (*str == '\\')
		{
			str++;
			qbuf_addc(buf, *str);
			str++;
		}
		else if (*str == '\'')
			push_str_til(&str, buf, '\'');
		else if (*str == '\"')
			push_str_til(&str, buf, '\"');
		else
			qbuf_addc(buf, *(str++));
	}
	free(elem->content);
	elem->content = (void *)qbuf_del(&buf);
	return (elem->content != NULL);
}

int					expand_quotes(t_list *list)
{
	while (list != NULL)
	{
		if (do_expand_quotes(list) == 0)
		{
			ft_list_clear(&list, &ft_memdel);
			return (-1);
		}
		list = list->next;
	}
	return (1);
}
