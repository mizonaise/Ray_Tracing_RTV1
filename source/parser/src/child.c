/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hastid <hastid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 04:18:26 by hastid            #+#    #+#             */
/*   Updated: 2020/11/13 09:43:37 by hastid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	add_filters(t_rt *r, char *v)
{
	if (!IS_SET(r->s.opt, FIL_SET))
	{
		if (!ft_strcmp(v, "NONE"))
			r->s.fil = NONE;
		else if (!ft_strcmp(v, "SEPIA"))
			r->s.fil = SEPIA;
		else if (!ft_strcmp(v, "STEREO"))
			r->s.fil = STEREO;
		else if (!ft_strcmp(v, "NEGATIVE"))
			r->s.fil = NEGATIVE;
		else if (!ft_strcmp(v, "BLACK_WHITE"))
			r->s.fil = BLACK_WHITE;
		else
			return (ERROR);
		r->s.opt |= FIL_SET;
		return (SUCCESS);
	}
	return (ERROR);
}

int	add_child(t_rt *r, char **tab)
{
	if (!tab[1])
		return (free_tab(tab, ERROR));
	if (r->id == CAMERA)
		return (parse_camera(r, tab));
	else if (r->id == SCENE)
		return (parse_scene(r, tab));
	else if (r->id == LIGHT)
		return (parse_lights(r, tab));
	else
		return (parse_objects(r, tab));
	return (free_tab(tab, ERROR));
}
