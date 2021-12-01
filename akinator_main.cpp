#include "akinator.hpp"
#include <stdio.h>

int main () {

    int mode = 0;
    while (true) {

        printf ("\nChoose the mode:\n1. Standard game\n2. Definition\n3. Comparison\n4. Visualization\n");
        scanf ("%d", &mode);
        if (mode == GAME || mode == DEFINE || mode == COMPARE || mode == VISUALIZE) {

            break;
        }

        printf ("\nWrong input format, try again\n");
        while (getchar () != '\n') ;
    }

    switch (mode) {

        case GAME: {

            ak_game_init ();
            break;
        }

        case DEFINE: {

            ak_def_init ();
            break;
        }

        case COMPARE: {

            ak_comp_init ();
            break;
        }

        case VISUALIZE: {

            ak_vis_init ();
            break;
        }
    }

    return 0;
}
