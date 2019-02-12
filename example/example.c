#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generic_timer.h"


int main(int argc, char *argv[]) {
    generic_timer_t timer1, timer2;

    if (argc < 2) {
        printf("Usage:\n\t%s <time to wait in seconds>\n", argv[0]);
        exit(1);
    }

    if (atoi(argv[1]) <= 0) {
        printf("Invalid delay\n");
        exit(1);
    }

    timer1 = create_timer();
    timer2 = create_timer();
    set_timer(timer1, atoi(argv[1]));
    set_timer(timer2, atoi(argv[1]));
    start_timer(timer1, time(NULL));

    while (!is_timer_reached(timer1, time(NULL))) {
        if (get_time_left(timer1, time(NULL)) <= atoi(argv[1])/2) {
            printf("Half of first timer\n");
            pause_timer(timer1, time(NULL));
            break;
        }
    }

    start_timer(timer2, time(NULL));
    while (!is_timer_reached(timer2, time(NULL)))
        ;
    printf("Finished second timer\n");

    restart_timer(timer1, time(NULL));
    while (!is_timer_reached(timer1, time(NULL)))
        ;


    printf("Timer has been reached!\n");
    return 0;
}
