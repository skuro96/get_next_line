#include "get_next_line.h"

int		free_return(char **s1, char **s2, int ret)
{
	if (s1 && *s1)
	{
		free(*s1);
		*s1 = NULL;
	}
	if (s2 && *s2)
	{
		free(*s2);
		*s2 = NULL;
	}
	return (ret);
}

int		save_buf(char **save, char **buf)
{
	char *tmp;

	if (*save != NULL)
	{
		tmp = *save;
		if (!(*save = ft_strjoin(*save, *buf)))
			return (free_return(buf, &tmp, READ_ERROR));
		free(tmp);
	}
	else if (!(*save = ft_substr(*buf, ft_strlen(*buf))))
		return (free_return(buf, NULL, READ_ERROR));
	return (0);
}

int		cut_endl(char **save, char **line)
{
	int		endl_idx;
	char	*tmp;

	endl_idx = ft_strchr(*save, '\n');
	if (endl_idx < 0)
	{
		if (!(*line = ft_substr(*save, ft_strlen(*save))))
			 (free_return(save, NULL, READ_ERROR));
		return (free_return(save, NULL, READ_EOF));
	}
	if (!(*line = ft_substr(*save, endl_idx)))
		return (free_return(save, NULL, READ_ERROR));
	tmp = *save;
	if (!(*save = ft_substr(*save + endl_idx + 1, ft_strlen(*save + endl_idx + 1))))
		return (free_return(&tmp, line, READ_ERROR));
	return (free_return(&tmp, NULL, READ));
}

int		get_next_line(int fd, char **line)
{
	static char	*save;
	char		*buf;
	int			ret;

	if (!line || fd < 0 || BUFFER_SIZE <= 0)
		return (READ_ERROR);
	if (!(buf = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (free_return(&save, NULL, READ_ERROR));
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (save_buf(&save, &buf) == READ_ERROR)
			return (READ_ERROR);
		if (ft_strchr(save, '\n') != -1)
			break ;
	}
	free(buf);
	if (ret < 0)
		return (free_return(&save, NULL, READ_ERROR));
	return (cut_endl(&save, line));
}
