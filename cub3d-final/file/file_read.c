/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_read.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anassif <anassif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 01:23:44 by anassif           #+#    #+#             */
/*   Updated: 2020/11/20 19:38:16 by anassif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		player_dir(char cord)
{
	if (cord == 'N')
		return (270);
	else if (cord == 'W')
		return (180);
	else if (cord == 'S')
		return (90);
	else if (cord == 'E')
		return (0);
	return (-1);
}

void	read_resolution(char *line, t_game *game)
{
	int	i;
	int j;

	i = 1;
	j = 0;
	while (line[++j])
	{
		if (!(ft_isdigit(line[j])) && (line[j] != ' ' && line[j] != '\t'))
			log_global_error("Resolution is to small or incorrect", game);
	}
	game->res_x = ft_atoi(&line[i]);
	while (line[i] == ' ')
		i++;
	while (ft_isdigit(line[i]))
		i++;
	game->res_y = ft_atoi(&line[i]);
}

int		check_floor_ceilling(char **split)
{
	int		i;
	char	*number;

	i = 0;
	while (split[i])
	{
		number = ft_strtrim(split[i], " ");
		if (!ft_isallnumber(number))
			return (-1);
		free(number);
		i++;
	}
	if (i != 3)
		return (-1);
	return (1);
}

int		read_floor_ceiling(char *line, t_game *game)
{
	char	**split;
	int		r;
	int		g;
	int		b;

	split = ft_split(line, ',');
	if (check_floor_ceilling(split) == -1)
		return (-1);
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	if (r > 255 || g > 255 || b > 255)
		log_global_error("Unvalid floor color or missing next param", game);
	if (r < 0 || g < 0 || b < 0)
		return (-1);
	free_split(split);
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void	read_file_game(char *file, char *param2, t_game *game)
{
	int		fd;
	int		i;
	char	*s;

	s = ft_strdup("--save");
	i = 0;
	if (!(ft_strnstr(file, ".cub", ft_strlen(file))))
		log_global_error("extension is unvalid or wrong", game);
	if (ft_strncmp(param2, s, 6) == 0 && ft_strlen(param2) == 6)
		game->save = 1;
	free(s);
	s = NULL;
	if (!(fd = open(file, O_RDONLY)))
		error_global_close_fd(fd, game);
	if (parse_file_game(fd, NULL, game, file) < 0)
		error_global_close_fd(fd, game);
	check_game(game, &game->map);
	close(fd);
}