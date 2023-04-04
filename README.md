# Pipex

Pipex is a function which replicates the behaviour of a [pipe](https://www.techtarget.com/whatis/definition/pipe#:~:text=Generally%2C%20a%20pipe%20is%20a,or%20more%20commands%20or%20programs). For this purpose our program will receive 4 arguments at least:

- argv[1]. This will be our input file, where we will have the arguments for the first command executed
- argv[2]. This will be the first command to be executed (before the pipe).
- argv[3]. This will be the second command to be executed (after the pipe).
- argv[4]. This will be the output file, where we will receive the output of our pipe.

For more commands we will compile our bonus function which will receive any ammount of arguments greater than 4, being the first argument the input file and being the last one the output file.

## Workflow

For our workflow we will use the default pipe (not bonus) since the bonus just adds a loop and repeats the process, but we will comment if something changes for bonus.

As said before our program must receive at least arguments so we will check if we have them (4 or more for bonus). In case we do not have them we will print an error message and return 1. In case we have them we will open both files (input and output) using our function `openfile`, which will give them a read or write permission to each one.

If the mode is "INFILE" (0) we will check if our file is accesible using [access](https://linux.die.net/man/2/access), if it is we will return its file descriptor with read permission using [open](https://man7.org/linux/man-pages/man2/open.2.html) function. If its not accessible it will print an error and return 0.

If the mode is "OUTFILE" we will create our outfile deleting it in case it already exist and will give it the permissions `-rw-rw-r--`.

After both are open we will use `dup2` function to create a copy of both [file descriptors](https://www.computerhope.com/jargon/f/file-descriptor.htm#:~:text=A%20file%20descriptor%20is%20a,Grants%20access.) and assign them the value 0 (STDIN) and 1 (STDOUT) so we can use them instead of the original file descriptor.

Once we have done this we will call `redir` function giving it the first command executed, the [environment variables[(https://www.twilio.com/blog/2017/01/how-to-set-environment-variables.html) and the input file descriptor.

In `redir` we will have a [pid_t](https://www.includehelp.com/c/process-identification-pid_t-data-type.aspx) variable and an int array with size of 2. We will use the function [pipe](https://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html#:~:text=The%20pipe%20function%20creates%20a,descriptor%201%20is%20standard%20output.) so our 2 int array is now a pipe, being `array[0]` our reading part of the pipe and `array[1]` our writing part of the pipe. We will use the function `fork` to create a child process and assign the process value to `pid`.

If `pid` is not 0 we will close our writing file descriptor, duplicate our reading file descriptor and wait for the child process to end. If `pid` is our child process we will close the reading file descriptor, duplicate our writing file descriptor and check our `fdin` value:
- If it is 0 we will exit the process returning 1.
- If it is not 0 we will call to `exec` function giving it our command and our environment variables as parameters.

In `exec` function we will split our command so we have separated words, if our first word (which must be the command) has `/` we will set this as our path; in case it does not have it we will check for a path in our environment variables using `get_paht` function.

Once we have our path we will call to the `execve` function. After this we will print an error message since a successful execve will end the process.

Our `get_path` function will search for the environment variables checking for the one starting with "PATH=". After this it will search for `:` since it is the delimitator for each path and will try to access to the command given joined to that path, it will try for each path until one of them works or until there is not anymore to try, in this case it will only return the command. After getting the correct path it will return the proper command with its full path.
