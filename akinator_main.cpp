#include "akinator.hpp"
#include <stdio.h>

int main () {

    while (true) {

        char mode = 0;
        while (true) {

            printf ("\nChoose the mode (or type \"e\" if you want to exit):\n1. Standard game\n2. Definition\n3. Comparison\n4. Visualization\n");
            scanf ("%c", &mode);
            if (mode == GAME || mode == DEFINE || mode == COMPARE || mode == VISUALIZE || mode == EXIT) {

                break;
            }

            printf ("\nWrong input format, try again\n");
            fflush (stdin);
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

            case EXIT: {

                return 0;
            }
        }

        fflush (stdin);
    }
}
