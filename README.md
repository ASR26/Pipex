# Pipex

Pipex is a function which replicates the behaviour of a (pipe)[https://www.techtarget.com/whatis/definition/pipe#:~:text=Generally%2C%20a%20pipe%20is%20a,or%20more%20commands%20or%20programs.]. For this purpose our program will receive 4 arguments at least:

- argv[1]. This will be our input file, where we will have the arguments for the first command executed
- argv[2]. This will be the first command to be executed (before the pipe).
- argv[3]. This will be the second command to be executed (after the pipe).
- argv[4]. This will be the output file, where we will receive the output of our pipe.

For more commands we will compile our bonus function which will receive any ammount of arguments greater than 4, being the first argument the input file and being the last one the output file.
