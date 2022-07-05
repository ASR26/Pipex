/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:09:30 by asolano-          #+#    #+#             */
/*   Updated: 2022/07/05 08:18:26 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 * La función open file requiere el nombre de un archivo y un modo
 * si el modo es 0 (infile) comprobará que el archivo existe, si no existe
 * imprimirá "pipex: <nombre del archivo> no such file or directory" y
 * devolverá 0 (stdin) y lo escribirá en el archivo. Todos estos errores serán
 * escritos con write(2, "texto", length); el 2 es el file descriptor que le
 * estamos dando.
 * En caso de que el archivo exista lo abriremos con el modo readonly
 * Si el modo no es 0 (infile) abre el archivo (si no existe lo crea con 
 * O_CREAT), lo trunca a longitud 0 (O_TRUNC) y lo pone en modo solo escritura
 * (O_WRONLY)
 * Le da permisos de lectura y escritura al usuario (S_IRUSR, S_WRUSR),
 * permisos de lectura y escritura al grupo (S_IRGRP, S_IWGRP)
 * y permisos de lectura a otros (S_IROTH);
 */
int	openfile(char *filename, int mode)
{
	if (mode == INFILE)
	{
		if (access(filename, F_OK))
		{
			write(STDERR, "pipex: ", 7);
			write(STDERR, filename, ft_strchr(filename, 0));
			write(STDERR, ": No such file or directory\n", 28);
			return (STDIN);
		}
		return (open(filename, O_RDONLY));
	}
	else
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
}

/*
 * Esta función se encarga de encontrar encontrar y preparar los paths para más
 * adelante, primero comprueba que exista la variable global PATH que recibirá
 * por el parámetro env, si no lo encuentra devolverá la cadena "cmd" que ha
 * recibido como parámetro.
 * En caso de que si lo encuentre hacemos un duplicado de cada uno de los path
 * en la variable dir y lo preparamos con la función path_join,que está en 
 * los utils.
 * Comprobamos con access si la ruta es accesible, en caso de que lo sea la
 * devolvemos, y en cada rotación del bucle liberamos dir para evitar leaks.
 * Si la ruta no está movemos el path hasta los siguientes ':' para seguir
 * comprobando y devolvemos la cadena cmd que hemos recibido como parámetro
 */
char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_strchr(path, ':') > -1)
	{
		dir = ft_strndup(path, ft_strchr(path, ':'));
		bin = path_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_strchr(path, ':') + 1;
	}
	return (cmd);
}

/*
 * La función exec recibe como parámetros cmd y la variable environment (env)
 * hacemos un split de la variable cmd usando ' ' como referencia, si en el
 * primer elemento de este split no encontramos una '/' asignamos este
 * primer elemento a path, si no usamos la función get_path para encontrar
 * el path y asignarselo a la variable path.
 * Usamos la función execve dandole como parametros el path, el split que hemos
 * hecho y la variable env.
 * Si no se ejecuta imprimirá como error "pipex: <comando>: command not found"
 * y cerrará el programa con un código 127 que es de error
 */

void	exec(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (ft_strchr(args[0], '/') > -1)
		path = args[0];
	else
		path = get_path(args[0], env);
	execve(path, args, env);
	write(STDERR, "pipex: ", 7);
	write(STDERR, cmd, ft_strchr(cmd, 0));
	write(STDERR, ": command not found\n", 20);
	exit(127);
}

/*
 * Esta función recibe como parámetros el string cmd, la variable de entorno
 * environment (env) y el file descriptor de entrada
 * Se crean dos variables, una pid_t (process identification) que es un signed
 * int y creamos un array de int de longitud 2.
 * Hacemos un pipe de ese array, esto crea dos file descriptors en el array,
 * -El primero (pipefd[0]) se usa para leer datos del pipe
 * -El segundo (pipefd[1]) se usa para escribir datos en el pipe
 * Al pid le hacemos un fork, creando dos subprocesos para esta variable
 * Si el pid es mayor que 0 quiere decir que estamos en el proceso padre,
 * en este caso cerramos el pipefd[1] (escritura) para evitar errores,
 * con dup2 copiamos el pipefd[0] (lectura) y le damos como valor '0',
 * y le decimos que espere hasta que otro proceso termine.
 * Si el pid es '0' quiere decir que estamos en el proceso hijo, cerramos el
 * pipefd[0] (lectura) para evitar errores y duplicamos el pipefd[1] (escritura)
 * dándole como valor '1'.
 * Si la variable fdin que le hemos dado como parametro es 0 cerramos el 
 * programa, si no llamamos a la función exec dándole como parámetro la string 
 * cmd y la variable de entorno env
 */
void	redir(char *cmd, char **env, int fdin)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
	else
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT);
		if (fdin == STDIN)
			exit(1);
		else
			exec(cmd, env);
	}
}

/*
 * Esta es la función principal del programa, recibe los tres parámetros que
 * puede recibir el main, argc, argv y env.
 * Declaramos dos int que será nuestros file descriptors de entrada y salida.
 * Si hay una cantidad distinta de 5 argumentos en la ejecución del programa
 * imprimirá "Invalid number of arguments" y se cierra el programa
 * En caso de que si sean 5 argumentos (nombre del programa, archivo de entrada,
 * primer comando, segundo comando, archivo de salida) comienza el programa.
 * usando fdin abrimos el archivo de entrada con valor 0 (lectura)
 * usando fdout abrimos el archivo de salida con valor 1 (escritura)
 * con dup2 creamos una copia de ambos file descriptors con sus correspondientes
 * valores.
 * Ejecutamos la función redir dandole como parámetros el primer comando, la
 * variable de entorno env y el file descriptor de entrada.
 * Ejecutamos la función exec dándole como parámetros la segunda función dada
 * y la variable de entorno env
 */

int	main(int argc, char **argv, char **env)
{
	int	fdin;
	int	fdout;

	if (argc == 5)
	{
		fdin = openfile(argv[1], INFILE);
		fdout = openfile(argv[4], OUTFILE);
		dup2(fdin, STDIN);
		dup2(fdout, STDOUT);
		redir(argv[2], env, fdin);
		exec(argv[3], env);
	}
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	return (1);
}
