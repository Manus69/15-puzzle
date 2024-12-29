#include "Prog.h"

//gui update
//gui move takes in dir
//move is computed by npuzzle


int main()
{
    Prog prog;

    if (! Prog_init(& prog))
    {
        printf("Init failed.\n");
    }

    while (prog.runs)
    {
        Prog_display(& prog);
        Prog_input(& prog);
        Prog_update(& prog);
    }

    Prog_deinit(& prog);

    return 0;
}