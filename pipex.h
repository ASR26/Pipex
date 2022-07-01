/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:07:59 by asolano-          #+#    #+#             */
/*   Updated: 2022/07/01 13:05:02 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define INFILE 0
# define OUTFILE 1

char	*path_join(char *path, char *bin);
int		ft_strncmp(char *str1, char *str2, int n);
int		ft_strchr(char *str, char c);
char	*ft_strndup(char *str, unsigned int n);
char	**ft_split(char * str, char sep);

#endif
