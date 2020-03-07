/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anabaoui <anabaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 13:24:16 by anabaoui          #+#    #+#             */
/*   Updated: 2020/03/06 00:34:51 by anabaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		generate_camera(t_rt *v)
{
	t_vect	up;
	double		fov;

	fov = RAD(v->c->fov);
	up = (t_vect){0, 1, 0};
	v->c->z = ft_vect_sub(v->c->tar, v->c->ori);
	v->c->z.z -= 0.001;
	ft_vect_norm(&v->c->z);
	v->c->x = ft_vect_cross(v->c->z, up);
	ft_vect_norm(&v->c->x);
	v->c->y = ft_vect_cross(v->c->x, v->c->z);
	v->c->height = tan(fov / 2) * 2;
	v->c->width = v->c->height * (IMG_W / IMG_H);
}

void		generate_camera_ray(t_rt *v, t_ray *r, double y, double x, int t)
{
	static double		tab[13][2] = {
		{0, 0},
		{1. / 6., -1. / 6.},
		{-1. / 6., -1. / 6.},
		{-1. / 6., 1. / 6.},
		{1. / 6., 1. / 6.},
		{1. / 3., -1. / 3.},
		{-1. / 3., -1. / 3.},
		{-1. / 3., 1. / 3.},
		{1. / 3., 1. / 3.}
	};
	r->ori = v->c->ori;
	r->dir = ft_vect_add(
		v->c->z,
		ft_vect_add(
			ft_vect_mult_nbr(v->c->x, PX_X((x + tab[t][0] + 0.5)) * v->c->width / 2.),
			ft_vect_mult_nbr(v->c->y, PX_Y((y + tab[t][1] + 0.5)) * v->c->height / 2.)));
	ft_vect_norm(&r->dir);
}

t_vect	ray_trace(t_rt *v, t_ray *ray, t_vect *color, double *c)
{
	double z;
	double n1;
	double n2;

	n1 = 1.;
	n2 = 1.5;
	z = 0;
	if (intersection_checker(v, *ray, &v->point))
	{
		objects_normal(*ray, &v->point);
		if (v->point.obj->id == SPHERE)
		{
			ray->ori = ft_vect_add(v->point.p_inter, ft_vect_mult_nbr(v->point.p_normal, 0.5));
			ray->dir = ft_vect_sub(ray->dir, ft_vect_mult_nbr(ft_vect_mult_nbr(v->point.p_normal, ft_vect_dot(ray->dir, v->point.p_normal)), 2));
			(*c)++;
			ray_trace(v, ray, color, c);
		}
		get_pixel_color(v, color);
	}
	return (*color);
}

void		get_pixel_color(t_rt *v, t_vect *light_color)
{
	t_light		*head;
	double		i;

	v->point.p_light.amb = (t_vect){0, 0, 0};
	v->point.p_light.def = (t_vect){0, 0, 0};
	v->point.p_light.spc = (t_vect){0, 0, 0};
	head = v->l;
	i = 1;
	while (head)
	{
		if (head->pow)
		{
			calculate_pixel_color(v, head, i);
			i++;
		}
		head = head->next;
	}

	*light_color = ft_vect_mult(v->point.p_color, ft_vect_add(
		ft_vect_add(v->point.p_light.def, v->point.p_light.amb),
						v->point.p_light.spc));
}
