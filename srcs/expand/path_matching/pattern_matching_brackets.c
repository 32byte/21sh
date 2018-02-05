/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_brackets.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/07 15:47:45 by vbastion          #+#    #+#             */
/*   Updated: 2018/01/07 15:55:34 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pattern_matching.h"

static void		ft_mb_rng(t_membuf *buf, u_int bin)
{
	if (bin & UPPER_BIN)
		ft_mb_add_unsafe(buf, PM_UPPER_STR);
	if (bin & LOWER_BIN)
		ft_mb_add_unsafe(buf, PM_LOWER_STR);
	if (bin & DIGIT_BIN)
		ft_mb_add_unsafe(buf, PM_DIGIT_STR);
	if (bin & PUNCT_BIN)
		ft_mb_add_unsafe(buf, PM_PUNCT_STR);
	if (bin & SPACE_BIN)
		ft_mb_add_unsafe(buf, PM_SPACE_STR);
	if (bin & XLOWER_BIN)
		ft_mb_add_unsafe(buf, PM_XLOWER_STR);
	if (bin & XUPPER_BIN)
		ft_mb_add_unsafe(buf, PM_XUPPER_STR);
}

static void		handle_rem(t_membuf *buf, char **curr)
{
	if (ft_strncmp(*curr + 1, PM_DIGIT, 5) == 0)
		ft_mb_add_unsafe(buf, PM_DIGIT_STR);
	else if (ft_strncmp(*curr + 1, PM_GRAPH, 5) == 0)
		ft_mb_rng(buf, UPPER_BIN | LOWER_BIN | DIGIT_BIN | PUNCT_BIN);
	else if (ft_strncmp(*curr + 1, PM_LOWER, 5) == 0)
		ft_mb_add_unsafe(buf, PM_LOWER_STR);
	else if (ft_strncmp(*curr + 1, PM_PRINT, 5) == 0)
	{
		ft_mb_rng(buf, UPPER_BIN | LOWER_BIN | DIGIT_BIN | PUNCT_BIN);
		ft_mb_addc(buf, ' ');
	}
	else if (ft_strncmp(*curr + 1, PM_PUNCT, 5) == 0)
		ft_mb_add_unsafe(buf, PM_PUNCT_STR);
	else if (ft_strncmp(*curr + 1, PM_SPACE, 5) == 0)
		ft_mb_add_unsafe(buf, PM_SPACE_STR);
	else if (ft_strncmp(*curr + 1, PM_UPPER, 5) == 0)
		ft_mb_add_unsafe(buf, PM_UPPER_STR);
	else
		ft_mb_add(buf, *curr, 7);
}

static void		classes(t_membuf *buf, char **curr)
{
	if (ft_strncmp(*curr + 1, PM_ALNUM, 5) == 0)
		ft_mb_rng(buf, UPPER_BIN | LOWER_BIN | DIGIT_BIN);
	else if (ft_strncmp(*curr + 1, PM_ALPHA, 5) == 0)
		ft_mb_rng(buf, UPPER_BIN | LOWER_BIN);
	else if (ft_strncmp(*curr + 1, PM_ASCII, 5) == 0)
		ft_mb_pushrange(buf, 0x0, 0xF);
	else if (ft_strncmp(*curr + 1, PM_BLANK, 5) == 0)
		ft_mb_add_unsafe(buf, " \t");
	else if (ft_strncmp(*curr + 1, PM_CNTRL, 5) == 0)
	{
		ft_mb_pushrange(buf, 0x0, 0xF);
		ft_mb_addc(buf, 0x7F);
	}
	else
		handle_rem(buf, curr);
	*curr += 7;
}

static int		spec_classes(t_membuf *buf, char **curr)
{
	if (**curr == ':' && ft_strncmp(*curr + 1, PM_WORD, 4) == 0
		&& (*curr)[5] == ':')
	{
		ft_mb_rng(buf, UPPER_BIN | LOWER_BIN | DIGIT_BIN);
		ft_mb_addc(buf, '_');
		*curr += 6;
		return (1);
	}
	else if (**curr == ':' && ft_strncmp(*curr + 1, PM_XDIGIT, 6) == 0
			&& (*curr)[7] == ':')
	{
		ft_mb_rng(buf, XUPPER_BIN | XLOWER_BIN | DIGIT_BIN);
		*curr += 8;
		return (1);
	}
	return (0);
}

char			*create_range(char *beg, char *end)
{
	t_membuf	buf;

	ft_mb_init(&buf);
	while (beg != end)
	{
		if (beg[0] > 0 && beg[0] < 127 && beg[1] == '-' && ft_isalnum(beg[2])
			&& beg[2] > 0 && beg[2] < 127)
		{
			ft_mb_pushrange(&buf, beg[0], beg[2]);
			beg += 3;
		}
		else if (ft_strwildcmp(beg, PM_CLASS_WILDCARD) == 0)
			classes(&buf, &beg);
		else if (spec_classes(&buf, &beg) != 0)
			break ;
		else
		{
			if (beg[0] == '\\')
				beg++;
			ft_mb_addc(&buf, *beg);
			beg++;
		}
	}
	return (ft_mb_fetch(&buf));
}
