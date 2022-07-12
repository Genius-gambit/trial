#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "minilibx_opengl/mlx.h"

#define W 13  //5 in linux 51
#define D 2 // 6 in linux 53
#define A 0 //4 in linux 50
#define S 1 //3 in linux 52
#define ARROW_L 123 //2 in linux 54
#define ARROW_R 124 //1 in linux 55
#define ESC 53 //1 in linux 49
#define PI 3.14159265358979323846

typedef struct s_player
{
	float	x;
	float	y;
	float	ang;
	float	dx;
	float	dy;
}				t_player;

typedef struct s_mouse
{
	float	angle;
}				t_mouse;

typedef struct s_ray
{
	float	ray_left_x;
	float	ray_left_y;
	float	ray_right_x;
	float	ray_right_y;
	float	angle_left;
	float	angle_right;
	int		m_r_left;
	int		incpt_r_left;
	int		m_r_right;
	int		incpt_r_right;
}				t_ray;

typedef struct	s_vars
{
	void		*mlx;
	void		*win;
	void		*img;
	int			width;
	int			height;
	char		**map;
	t_player	p;
	t_ray		r;
	t_mouse		mouse;
}				t_vars;


void	ray_init(t_vars *vars)
{
	vars->r.angle_left = PI / 3;
	vars->r.angle_right = PI / 3;
	vars->r.ray_left_x = 0;
	vars->r.ray_left_y = 0;
	vars->r.ray_right_x = 0;
	vars->r.ray_right_y = 0;
	vars->r.m_r_left = 0;
	vars->r.m_r_right = 0;
	vars->r.incpt_r_left = 0;
	vars->r.incpt_r_right = 0;
}

void	draw_rays(t_vars *vars)
{
	int	i;
	int	j;

	j = 0;
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mlx_pixel_put(vars->mlx, vars->win, vars->r.ray_left_x + i,
				vars->r.ray_left_y + j, 0x00FF00);
			i++;
		}
		j++;
	}
	j = 0;
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mlx_pixel_put(vars->mlx, vars->win, vars->r.ray_right_x + i,
				vars->r.ray_right_y + j, 0x00FF00);
			i++;
		}
		j++;
	}
}

void	which_quadrant(t_vars *vars)
{
	if (vars->p.ang >= 0 && vars->p.ang < (PI / 2))
		printf("First Quadrant\n");
	else if (vars->p.ang >= (PI / 2) && vars->p.ang < PI)
		printf("Second Quadrant\n");
	else if (vars->p.ang >= PI && vars->p.ang < (3 * (PI / 2)))
		printf("Third Quadrant\n");
	else if (vars->p.ang >= (3 * (PI / 2)) && vars->p.ang < (2 * PI))
		printf("Fourth Quadrant\n");
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	building_rays(t_vars *vars)
{
	vars->r.angle_left = vars->p.ang - (PI / 6);
	if (vars->r.angle_left < 0)
		vars->r.angle_left += 2 * PI;
	vars->r.angle_right = vars->p.ang + (PI / 6);
	if (vars->r.angle_right > 2 * PI)
		vars->r.angle_right -= 2 * PI;
	vars->img = mlx_xpm_file_to_image(vars->mlx, "black_bg.xpm", &vars->width, &vars->height);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->r.ray_left_x, vars->r.ray_left_y);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->r.ray_right_x, vars->r.ray_right_y);
	vars->r.ray_left_x = roundf((cos(vars->r.angle_left) * 100) + vars->p.x);
	vars->r.ray_left_y = roundf((sin(vars->r.angle_left) * 100) + vars->p.y);
	vars->r.ray_right_x = roundf((cos(vars->r.angle_right) * 100) + vars->p.x);
	vars->r.ray_right_y = roundf((sin(vars->r.angle_right) * 100) + vars->p.y);
	draw_rays(vars);
	printf("Ray1X: %f, Ray1Y: %f, Angle Left: %f, Angle Right: %f\n", vars->r.ray_left_x,
		vars->r.ray_left_y, vars->r.angle_left, vars->r.angle_right);
	printf("Angle of Player: %f\n", vars->p.ang);
}

void	draw_player(t_vars *vars)
{
	int	i;
	int	j;

	j = 0;
	building_rays(vars);
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mlx_pixel_put(vars->mlx, vars->win, vars->p.x + i,
				vars->p.y + j, 0x00FF00);
			i++;
		}
		j++;
	}
}

void	raycasting_up(t_vars *vars)
{
	if (((vars->p.x + cos(vars->p.ang) * 15) > 0)
		&& ((vars->p.x + cos(vars->p.ang) * 15) < 1920)
		&& ((vars->p.y + sin(vars->p.ang) * 15) > 0)
		&& ((vars->p.y + sin(vars->p.ang) * 15) < 1240))
	{
		vars->img = mlx_xpm_file_to_image(vars->mlx, "black_bg.xpm", &vars->width, &vars->height);
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->p.x, vars->p.y);
		vars->p.dx = cos(vars->p.ang) * 15;
		vars->p.dy = sin(vars->p.ang) * 15;
		vars->p.x += vars->p.dx;
		vars->p.y += vars->p.dy;
		draw_player(vars);
	}
}

void	raycasting_down(t_vars *vars)
{
	if (((vars->p.x - cos(vars->p.ang) * 15) > 0)
		&& ((vars->p.x - cos(vars->p.ang) * 15) < 1920)
		&& ((vars->p.y - sin(vars->p.ang) * 15) > 0)
		&& ((vars->p.y - sin(vars->p.ang) * 15) < 1240))
	{
		vars->img = mlx_xpm_file_to_image(vars->mlx, "black_bg.xpm", &vars->width, &vars->height);
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->p.x, vars->p.y);
		vars->p.dx = cos(vars->p.ang) * 15;
		vars->p.dy = sin(vars->p.ang) * 15;
		vars->p.x -= vars->p.dx;
		vars->p.y -= vars->p.dy;
		draw_player(vars);
	}
}

void	raycasting_left(t_vars *vars)
{
	if (((vars->p.x - cos((PI / 2) + vars->p.ang) * 15) > 0)
		&& ((vars->p.x - cos((PI / 2) + vars->p.ang) * 15) < 1920)
		&& ((vars->p.y - sin((PI / 2) + vars->p.ang) * 15) > 0)
		&& ((vars->p.y - sin((PI / 2) + vars->p.ang) * 15) < 1240))
	{
		vars->img = mlx_xpm_file_to_image(vars->mlx, "black_bg.xpm", &vars->width, &vars->height);
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->p.x, vars->p.y);
		vars->p.dx = cos((PI / 2) + vars->p.ang) * 15;
		vars->p.dy = sin((PI / 2) + vars->p.ang) * 15;
		vars->p.x -= vars->p.dx;
		vars->p.y -= vars->p.dy;
		draw_player(vars);
	}
}

void	raycasting_right(t_vars *vars)
{
	if (((vars->p.x - (-(cos((PI / 2) + vars->p.ang) * 15)) > 0)
			&& (vars->p.x - (-(cos((PI / 2) + vars->p.ang) * 15)) < 1920))
		&& ((vars->p.y - (-(sin((PI / 2) + vars->p.ang) * 15)) > 0)
			&& (vars->p.y - (-(sin((PI / 2) + vars->p.ang) * 15)) < 1240)))
	{
		vars->img = mlx_xpm_file_to_image(vars->mlx, "black_bg.xpm", &vars->width, &vars->height);
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img, vars->p.x, vars->p.y);
		vars->p.dx = -(cos((PI / 2) + vars->p.ang) * 15);
		vars->p.dy = -(sin((PI / 2) + vars->p.ang) * 15);
		vars->p.x -= vars->p.dx;
		vars->p.y -= vars->p.dy;
		draw_player(vars);
	}
}


void	rotation(int keycode, t_vars *vars)
{
	if (keycode == ARROW_L)
	{
		vars->p.ang -= 0.1;
		if (vars->p.ang < 0)
			vars->p.ang += 2 * PI;
	}
	if (keycode == ARROW_R)
	{
		vars->p.ang += 0.1;
		if (vars->p.ang > 2 * PI)
			vars->p.ang -= 2 * PI;
	}
	draw_player(vars);
}

void	free_all(t_vars *vars)
{
	int	i;

	i = 0;
	while (vars->map[i] != NULL)
	{
		free(vars->map[i]);
		i++;
	}
	free(vars->map);
}

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == W)
		raycasting_up(vars);
	if (keycode == S)
		raycasting_down(vars);
	if (keycode == D)
		raycasting_right(vars);
	if (keycode == A)
		raycasting_left(vars);
	rotation(keycode, vars);
	if (keycode == ESC)
	{
		free_all(vars);
		mlx_destroy_window(vars->mlx, vars->win);
		exit(1);
	}
	return (0);
}

void	init(t_vars *vars)
{
	vars->p.x = 300;
	vars->p.y = 300;
	vars->p.dx = 0;
	vars->p.dy = 0;
	vars->p.ang = 3 * (PI / 2);
	vars->height = 0;
	vars->width = 0;
}

int	mouse_hook(int x, int y, t_vars *vars)
{
	if (x < 960)
		rotation(ARROW_L, vars);
	else if (x > 960)
		rotation(ARROW_R, vars);
	printf("Mouse Coordinates: %d, %d\n", x, y);
	mlx_mouse_move(vars->win, 960, 620);
	return (0);
}

void	map_dup(char **map, t_vars *vars)
{
	int	i;

	i = 0;
	vars->map = (char **)malloc(sizeof(char *) * 9);
	while (map[i] != NULL)
	{
		vars->map[i] = strdup(map[i]);
		free(map[i]);
		i++;
	}
	vars->map[i] = NULL;
	free(map);
}

void	init_window(t_vars *vars)
{
	int	i;
	int	j;
	int	x;
	int	y;
	int	k;
	int	l;

	i = 0;
	j = 0;
	x = 0;
	y = 0;
	k = 0;
	l = 0;
	while (vars->map[i] != NULL)
	{
		j = 0;
		while (vars->map[i][j])
		{
			if (vars->map[i][j] == '1')
			{
				vars->img = mlx_xpm_file_to_image(vars->mlx, "white.xpm", &k, &l);
				mlx_put_image_to_window(vars->mlx, vars->win, vars->img, x, y);
				mlx_destroy_image(vars->mlx, vars->img);
			}
			j++;
			x += 240;
		}
		y += 155;
		x = 0;
		i++;
	}
}


// mouse
int main(void)
{
	t_vars	vars;
	char	**map;

	init(&vars);
	map = (char **)malloc(sizeof(char *) * 9);
	map[0] = strdup("11111111");
	map[1] = strdup("10000001");
	map[2] = strdup("10000001");
	map[3] = strdup("10000001");
	map[4] = strdup("10000001");
	map[5] = strdup("10000001");
	map[6] = strdup("10000001");
	map[7] = strdup("11111111");
	map[8] = NULL;
	ray_init(&vars);
	vars.mlx = mlx_init();
	map_dup(map, &vars);
	vars.win = mlx_new_window(vars.mlx, 1920, 1240, "Cub3D");
	init_window(&vars);
	draw_player(&vars);
	mlx_hook(vars.win, 6, (1L<<6), mouse_hook, &vars);
	mlx_hook(vars.win, 2, (1L<<0), key_hook, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
