/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 20:52:07 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/29 00:57:12 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		free_all_object(t_var *v)
{
	void	*ptr;
	void	*tmp;

	ptr = v->cam;
	ft_memdel((void **)&v->cam);
	ptr = v->light;
	while (ptr)
	{
		tmp = ptr;
		ptr = ((t_light *)ptr)->next;
		free(tmp);
	}
	v->light = NULL;
	ptr = v->obj;
	while (ptr)
	{
		tmp = ptr;
		ptr = ((t_obj *)ptr)->next;
		free(tmp);
	}
	v->obj = NULL;
}

static int	add_object_to_var(t_var *v, void *obj, int type)
{
	if (type == 0 && v->cam)
	{
		ft_putstr("error : double camera in the scene\n");
		return (-1);
	}
	if (type == 0)
		v->cam = obj;
	if (type == 1)
	{
		((t_light *)obj)->next = v->light;
		v->light = obj;
	}
	if (type == 2)
	{
		((t_obj *)obj)->o = ft_vector_add(((t_obj *)obj)->o,
														((t_obj *)obj)->trans);
		((t_obj *)obj)->rot.x *= M_PI / 180;
		((t_obj *)obj)->rot.y *= M_PI / 180;
		((t_obj *)obj)->rot.z *= M_PI / 180;
		((t_obj *)obj)->axis = ft_vector_rotate(((t_obj *)obj)->axis,
														((t_obj *)obj)->rot);
		((t_obj *)obj)->next = v->obj;
		v->obj = obj;
	}
	return (type);
}

static void	*parse_object(char *line, int size, int type)
{
	void	*obj;
	char	*tmp;

	if ((obj = ft_memalloc(size)))
		while (obj && line && *line)
		{
			if ((tmp = ft_strchr(line, ' ')))
			{
				*tmp = '\0';
				if ((tmp = parse_property(obj, line, type)))
					line = ft_skip_chars(tmp, " ", NULL);
				else
				{
					ft_putstr("error : invalid property value\n");
					ft_memdel(&obj);
				}
			}
			else
			{
				ft_putstr("error : invalid property\n");
				ft_memdel(&obj);
			}
		}
	return (obj);
}

static int	get_object(char *line, t_var *var)
{
	char	*tmp;
	void	*obj;
	int		obj_type;

	if ((tmp = ft_strchr(line, ' ')))
	{
		*tmp++ = '\0';
		if (ft_strequ(line, "camera"))
			if ((obj = parse_object(tmp, sizeof(t_cam), 0)))
				return (add_object_to_var(var, obj, 0));
		if (ft_strequ(line, "light"))
			if ((obj = parse_object(tmp, sizeof(t_light), 1)))
				return (add_object_to_var(var, obj, 1));
		obj_type = ft_strmequ(line, (char *[]){"cone", "plane",
													"sphere", "cylinder", 0});
		if (obj_type)
			if ((obj = parse_object(tmp, sizeof(t_obj), 2)))
			{
				((t_obj *)obj)->id = obj_type;
				return (add_object_to_var(var, obj, 2));
			}
	}
	else
		ft_putstr("error : object not found\n");
	return (-1);
}

int			parse_file(char *file, t_var *v)
{
	char	*line;
	int		type[3];
	int		t;
	int		fd;

	fd = open(file, O_RDONLY);
	ft_bzero(type, sizeof(int) * 3);
	while (get_next_line(fd, &line) > 0)
	{
		if (!ft_strnequ(line, "//", 2) && *ft_skip_chars(line, " ", NULL))
		{
			if ((t = get_object(line, v)) == -1)
				return (0);
			type[t]++;
		}
		ft_strdel(&line);
	}
	if (type[0] == 0 || type[1] == 0 || type[2] == 0)
	{
		ft_putstr("incomplete scene\n");
		return (0);
	}
	return (1);
}
