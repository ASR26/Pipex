/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:39 by asolano-          #+#    #+#             */
/*   Updated: 2022/07/05 08:32:53 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 * Esta función se encarga de preparar el path para comprobarlo más adelante.
 * Hacemos un malloc del tamaño de path más el tamaño de bin más 2.
 * Mientras que exista el path vamos copiando el path en joined, y al final le
 * añadimos '/' para que el pathing funcione correctamente.
 * Mientras que exista bin vamos copiando en joined lo que haya en bin (el
 * comando que vamos a buscar), le añadimos un '0' al final y lo devolvemos.
 */

char	*path_join(char *path, char *bin)
{
	char	*joined;
	int		i;
	int		j;

	joined = malloc (sizeof(char) * (ft_strchr(path, 0)
				+ ft_strchr(bin, 0) + 2));
	i = 0;
	j = 0;
	while (path[j])
		joined[i++] = path[j++];
	joined [i++] = '/';
	j = 0;
	while (bin[j])
		joined[i++] = bin[j++];
	joined[i] = 0;
	return (joined);
}

/*
 * Esta función compara dos strings hasta cierto número de caracteres, en caso
 * de que sean diferentes devolverá el valor de diferencia entre los caracteres,
 * si son iguales devolverá 0
 */

int	ft_strncmp(char *str1, char *str2, int n)
{
	while (--n > 0 && *str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*str2 - *str1);
}

/*
 * Esta función cuenta el número de caracteres que hay en un string hasta que
 * coincida con uno dado, en caso de que no exista ese caracter devolverá -1
 */

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

/*
 * Esta función duplica un string hasta una cantidad de caracteres
 */

char	*ft_strndup(char *str, unsigned int n)
{
	char			*dup;
	unsigned int	i;

	i = 0;
	dup = malloc(sizeof(char) * (n + 1));
	while (i < n)
		dup[i++] = *str++;
	dup[n] = 0;
	return (dup);
}

/*
 * Esta función separa un string en un array de strings tomando un caracter
 * específico como delimitador
 */

char	**ft_split(char *str, char c)
{
	char	**tab;
	int		count;
	int		i;
	int		j;

	count = 0;
	j = 0;
	while (str[j])
	{
		if (str[j++] == c)
			count++;
	}
	tab = malloc(sizeof(char *) * (count + 2));
	tab[count + 1] = NULL;
	i = 0;
	while (i < count + 1)
	{
		j = 0;
		while (str[j] && str[j] != c)
			j++;
		tab[i++] = ft_strndup(str, j);
		str = str + j + 1;
	}
	return (tab);
}
