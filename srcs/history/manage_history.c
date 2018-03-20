/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/19 18:40:33 by lportay           #+#    #+#             */
/*   Updated: 2018/03/19 18:50:19 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void	trim_history(t_dlist **histlist, char *s_histsize)
{
	int		histsize;
	t_dlist	*tmp;

	if (s_histsize)
		histsize = ft_atoi(s_histsize);
	else
		histsize = ft_atoi(HISTSIZE);
	tmp = *histlist;
	while (histsize && *histlist)
	{
		*histlist = (*histlist)->next;
		histsize--;
	}
	if (*histlist)
	{
		(*histlist)->prev->next = NULL;
		ft_dlstdel(histlist, &del_histentry);
	}
	*histlist = tmp;
}

void	save_history(char *histfile, t_dlist *histlist, int flags)
{
	char	*tmp;
	int		fd;

	fd = open(histfile ? histfile : HISTFILE,
			O_CREAT | flags | O_RDWR, S_IWUSR | S_IRUSR);
	if (fd == -1)
		return ;
	while (histlist->prev)
	{
		tmp = str_from_dlst(T_HISTENTRY(histlist->data)->line);
		if (tmp && !str_isblank(tmp))
		{
			write(fd, tmp, ft_strlen(tmp));
			write(fd, "\n", 1);
		}
		ft_strdel(&tmp);
		histlist = histlist->prev;
	}
	close(fd);
}

void	init_hist(t_hist *hist, char *histfile)
{
	char	*histentry;
	int		file;

	histentry = NULL;
	file = open(histfile, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
	get_next_line(file, &histentry);
	while (histentry)
	{
		if (ft_strlen(histentry))
			ft_dlstinsert(hist->list, ft_dlstnewaddr(new_histentry(
		dlst_from_str(histentry), hist->index++), sizeof(t_histentry)));
		free(histentry);
		if (get_next_line(file, &histentry) == -1)
			break ;
	}
	close(file);
	hist->first_entry = hist->index;
	trim_history(&hist->list, ft_astr_getval(get_ctxaddr()->locals,
				"HISTSIZE"));
}