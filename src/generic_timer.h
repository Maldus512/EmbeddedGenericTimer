#ifndef __GENERIC_TIMER_H__
#define __GENERIC_TIMER_H__

typedef enum {
    TIMER_ERROR = -1,
    TIMER_STOPPED,
    TIMER_COUNTING,
    TIMER_PAUSED,
    TIMER_REACHED,
} TIMER_STATE;

/* Type for timer handles */
// typedef int generic_timer_t;
typedef struct _timer_state *generic_timer_t;


/*
 * Creates a timer and returns the handle.
 * Returns NULL on failure
 */
generic_timer_t create_timer();

/*
 * Starts the specified timer; requires the current time.
 * Returns -1 on failure.
 */
int start_timer(generic_timer_t timer, unsigned long currenttime);

/*
 * Returns the state of the specified timer; requires the current time.
 * Returns -1 on failure.
 */
TIMER_STATE get_timer_state(generic_timer_t timer, unsigned long currenttime);

/*
 * Returns 1 if the specified timer is finished, 0 otherwise; requires the current time.
 * Returns -1 on failure.
 */
int is_timer_reached(generic_timer_t timer, unsigned long currenttime);

/*
 * Returns the missing time before the end is reached. Requires the current time.
 */
unsigned long get_time_left(generic_timer_t timer, unsigned long currenttime);

/*
 * Pauses the timer, keeping the elapsed time intact. Requires the current time.
 * Returns -1 on failure.
 */
int pause_timer(generic_timer_t timer, unsigned long currenttime);

/*
 * Restarts a paused timer. Requires the current time.
 * Returns -1 on failure.
 */
int restart_timer(generic_timer_t timer, unsigned long currenttime);

/*
 * Stops the specified timer, resetting it to zero.
 * Returns -1 on failure.
 */
int stop_timer(generic_timer_t timer);

/*
 * Resets the timer to the initial value. It also stops it
 * Returns -1 on failure.
 */
int reset_timer(generic_timer_t timer, unsigned long currenttime);

/*
 * Returns time that has passed since start of the timer. Requires current time.
 * Returns -1 on failure.
 */
unsigned long get_elapsed_time(generic_timer_t timer, unsigned long currenttime);

/*
 * Sets the specified timer to a new alarm; it also puts it in stopped state
 * and resets the elapsed time.
 * Returns -1 on failure.
 */
int set_timer(generic_timer_t timer, unsigned long timertime);

/*
 * Deallocates the specified timer.
 */
void destroy_timer(generic_timer_t timer);

/*
 * Deallocates all timers.
 */
void destroy_all_timers();

#endif
