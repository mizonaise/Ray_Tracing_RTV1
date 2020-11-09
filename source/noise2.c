/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hezzahir <hamza.ezzahiry@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 05:57:57 by hezzahir          #+#    #+#             */
/*   Updated: 2020/11/09 00:18:31 by hezzahir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	smooth_noise(double x, double y, double z)
{
	t_vect		f;
	t_cordin	p;
	t_cordin	p1;
	double		value;

	value = 0.0;
	f.x = x - (int)x;
	f.y = y - (int)y;
	f.z = z - (int)z;
	p.x = ((int)x + noiseWidth) % noiseWidth;
	p.y = ((int)y + noiseHeight) % noiseHeight;
	p.z = ((int)z + noiseDepth) % noiseDepth;
	p1.x = (p.x + noiseWidth - 1) % noiseWidth;
	p1.y = (p.y + noiseHeight - 1) % noiseHeight;
	p1.z = (p.z + noiseDepth - 1) % noiseDepth;
	value += f.x * f.y * f.z * g_noise[p.z][p.y][p.x];
	value += f.x * (1 - f.y) * f.z * g_noise[p.z][p1.y][p.x];
	value += (1 - f.x) * f.y * f.z * g_noise[p.z][p.y][p1.x];
	value += (1 - f.x) * (1 - f.y) * f.z * g_noise[p.z][p1.y][p1.x];
	value += f.x * f.y * (1 - f.z) * g_noise[p1.z][p.y][p.x];
	value += f.x * (1 - f.y) * (1 - f.z) * g_noise[p1.z][p1.y][p.x];
	value += (1 - f.x) * f.y * (1 - f.z) * g_noise[p1.z][p.y][p1.x];
	value += (1 - f.x) * (1 - f.y) * (1 - f.z) * g_noise[p1.z][p1.y][p1.x];
	return (value);
}

double	turbulence(double x, double y, double z, double size)
{
	double	value;
	double	initial_size;

	value = 0.0;
	initial_size = size;
	while (size >= 1)
	{
		value += smooth_noise(x / size, y / size, z / size) * size;
		size /= 2.0;
	}
	return (128.0 * value / initial_size);
}

void	wood(t_vect pt, t_vect *color, double xy_period)
{
	t_vect	value;
	double	turb_power;
	double	turb_size;
	double	sine_value;
	double	dist_value;

	turb_power = 0.7;
	turb_size = 12465.0;
	value.x = (pt.x - noiseWidth / 2) / (double)noiseWidth;
	value.y = (pt.y - noiseHeight / 2) / (double)noiseHeight;
	value.z = (pt.z - noiseDepth / 2) / (double)noiseDepth;
	dist_value = sqrt(value.x * value.x + value.y * value.y + value.z * value.z)
			+ turb_power * turbulence(pt.x, pt.y, pt.z, turb_size) / 256.0;
	sine_value = 128.0 * fabs(sin(2 * xy_period * dist_value * 3.14159));
	color->x = (uint8_t)(80 + sine_value);
	color->y = (uint8_t)(30 + sine_value);
	color->z = 30;
}
