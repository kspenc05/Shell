CIS3110 Assignment 1
Date: 12/02/2016 
Author: Kent Spence (0872780)



References: code that I used in my system that I did not write

    where:: https://forums.gentoo.org/viewtopic-t-704279.html
    where in code:: used in shellFunctions.c on line 1
    
    Author: the user known as "UberLord" (No idea of the author's real name)

    Found this when my compiler would complain about kill() being an implicit declaration

    Without it the compiler will complain about
    an implicit declaration for kill(), although, as shown on the website:
    
    in the manpages for kill(), one must define OPEN Source as a macro in order to use
    kill() (there could be an easier way to do this, but my code does not function 
    without this line).

    At most this is 1% of my entire code, it's just a weird line to see, that is not
    my own code



RUNNING AND COMPILING:

    To compile: open up a terminal, move to the directory where shell.c, shellFunctions.c and Makefile are all located
    (they should be all located in the same directory)
    and type 'make'.

    To run: type './cis3110sh' into that same directory.



WHAT HAS BEEN IMPLEMENTED:

    the functions in part 1,
    most of the functions in part 2,
    and "cd" of part 3.



WHAT HAS NOT BEEN IMPLEMENTED:

    in part2, piping WAS NOT implemented.
    anything in part 3 except "cd", WAS NOT implemented.
    
    