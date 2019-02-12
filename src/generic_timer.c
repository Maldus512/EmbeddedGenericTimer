#include "generic_timer.h"

#define MAX_TIMERS 5

#ifndef NULL
#define NULL 0
#endif

struct _timer_state {
    unsigned long time_left;
    unsigned long starting_time;
    unsigned long total_time;
    unsigned long elapsed_time;
    TIMER_STATE   state;
    char          f_allocated;
};

static struct _timer_state _t[MAX_TIMERS] = {{
    .time_left     = 0,
    .starting_time = 0,
    .total_time    = 0,
    .elapsed_time  = 0,
    .state         = TIMER_STOPPED,
    .f_allocated   = 0,
}};

int VALID_TIMER(generic_timer_t timer) {
    int i, found = 0;
    for (i = 0; i < MAX_TIMERS; i++) {
        if (timer == &_t[i]) {
            found = 1;
            break;
        }
    }
    return found && timer->f_allocated;
}

void _update_timer(unsigned long currenttime, struct _timer_state *t) {
    unsigned long timeleft, unaccountedtime;
    switch (t->state) {
        case TIMER_COUNTING:
            timeleft        = t->total_time - t->elapsed_time;
            unaccountedtime = currenttime - t->starting_time;
            if (t->time_left == 0 || timeleft <= unaccountedtime) {
                t->time_left    = 0;
                t->state        = TIMER_REACHED;
                t->elapsed_time = t->total_time;
            } else {
                t->time_left = timeleft - unaccountedtime;
            }
            break;

        case TIMER_PAUSED:
            t->starting_time = currenttime;
            break;

        case TIMER_ERROR:
        case TIMER_REACHED:
        case TIMER_STOPPED:
            break;
    }
}

int start_timer(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    switch (timer->state) {
        case TIMER_STOPPED:
            timer->starting_time = currenttime;
            timer->elapsed_time  = 0;
            timer->state         = TIMER_COUNTING;
            return 0;
            break;
        default:
            return -1;
            break;
    }
}

void destroy_timer(generic_timer_t timer) {
    if (!VALID_TIMER(timer))
        return;

    timer->f_allocated = 0;
}

void destroy_all_timers() {
    int i;
    for (i = 0; i < MAX_TIMERS; i++) {
        _t[i].f_allocated = 0;
    }
}

generic_timer_t create_timer(unsigned long timertime) {
    int i;
    for (i = 0; i < MAX_TIMERS; i++) {
        if (!_t[i].f_allocated) {
            _t[i].f_allocated  = 1;
            _t[i].time_left    = timertime;
            _t[i].elapsed_time = 0;
            _t[i].state        = TIMER_STOPPED;
            _t[i].total_time   = timertime;
            return &_t[i];
        }
    }
    return NULL;
}

int set_timer(generic_timer_t timer, unsigned long timertime) {
    if (!VALID_TIMER(timer))
        return -1;

    timer->time_left    = timertime;
    timer->elapsed_time = 0;
    timer->state        = TIMER_STOPPED;
    timer->total_time   = timertime;
    return 0;
}

int stop_timer(generic_timer_t timer) {
    if (!VALID_TIMER(timer))
        return -1;

    timer->time_left     = 0;
    timer->starting_time = 0;
    timer->state         = TIMER_STOPPED;
    timer->total_time    = 0;
    return 0;
}

int reset_timer(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    timer->time_left     = timer->total_time;
    timer->starting_time = currenttime;
    timer->state         = TIMER_STOPPED;
    timer->elapsed_time  = 0;
    return 0;
}

unsigned long get_time_left(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return 0;

    _update_timer(currenttime, timer);
    return timer->time_left;
}

unsigned long get_elapsed_time(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return 0;

    _update_timer(currenttime, timer);
    if (timer->state == TIMER_COUNTING)
        return timer->elapsed_time + (currenttime - timer->starting_time);
    else
        return timer->elapsed_time;
}

int pause_timer(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    switch (timer->state) {
        case TIMER_STOPPED:
            timer->state = TIMER_PAUSED;
            return 0;
            break;
        case TIMER_COUNTING:
            _update_timer(currenttime, timer);
            timer->state = TIMER_PAUSED;
            timer->elapsed_time += currenttime - timer->starting_time;
            return 0;
            break;
        default:
            return -1;
            break;
    }
}

int restart_timer(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    _update_timer(currenttime, timer);
    timer->state = TIMER_COUNTING;
    return 0;
}

TIMER_STATE get_timer_state(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    _update_timer(currenttime, timer);
    return timer->state;
}

int is_timer_reached(generic_timer_t timer, unsigned long currenttime) {
    if (!VALID_TIMER(timer))
        return -1;

    _update_timer(currenttime, timer);
    return timer->state == TIMER_REACHED;
}
