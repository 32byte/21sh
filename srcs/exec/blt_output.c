/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_output.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/16 16:53:38 by vbastion          #+#    #+#             */
/*   Updated: 2018/02/18 13:56:42 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int						blt_output(t_proc *p)
{
	t_list				*lst;
	char				*str;

	lst = p->data.out;
	while (lst != NULL)
	{
		str = (char *)lst->content;
		ft_putstr_fd((char *)lst->content + 1, ((char *)lst->content)[0] - '0');
//		write(str[0] - '0', str + 1, ft_strlen(str + 1));
		lst = lst->next;
	}
	return (p->status);
}
