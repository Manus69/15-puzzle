#include "Prog.h"

int main()
{
    Prog prog;

    if (! Prog_start(& prog)) return Prog_error(& prog, "Init failed");

    while (prog.runs)
    {
        Prog_display(& prog);
        Prog_input(& prog);
        Prog_update(& prog);
    }

    Prog_stop(& prog);

    return 0;
}