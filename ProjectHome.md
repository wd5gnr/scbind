This is a little program I wrote some years ago that allows you to "compile"
shell scripts. Well, OK, you don't really compile them. The way the program
works is that it creates and compiles a C program with your script scrambled
up inside. When you run the program, it runs the shell of your choice, and
calls it, feeding it input from a named pipe. This allows the program to
feed commands to the shell, while the shell's stdin is still the terminal
(or wherever you think it is).