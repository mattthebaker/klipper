#include <string.h>
#include <stdint.h>

#include "command.h"
#include "sched.h"
#include "generic/misc.h"
#include "board/internal.h"
#include "board/armcm_timer.h"

typedef struct {
    struct timer time;
    struct task_wake wake;
} hsevar_state_t;

static hsevar_state_t m_state;

static uint_fast8_t
hsevar_event(struct timer *t)
{
    sched_wake_task(&m_state.wake);
    t->waketime += timer_from_us(2000);
    return SF_RESCHEDULE;
}

void
hsevar_init(void)
{
    memset(&m_state, 0, sizeof(m_state));

    m_state.time.func = hsevar_event;
    m_state.time.waketime = timer_read_time() + timer_from_us(100000);
    //sched_wake_task(&m_state.wake);
    sched_add_timer(&m_state.time);
}
DECL_INIT(hsevar_init);

#define DEPTH 256

static uint32_t c_delta[DEPTH];
static uint32_t min = 0-1, max = 0, var, avg;
static uint64_t sum = 0;

void
hsevar_task(void)
{
    static uint32_t i = 0;
    uint32_t clock;

    if (!sched_check_wake(&m_state.wake))
        return;

    clock = timer_read_time();
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;
    clock = timer_read_time() - clock;
    RCC->CR &= ~RCC_CR_HSEON;

    c_delta[i] = clock;
    sum += clock;
    if (clock < min)
        min = clock;
    if (clock > max)
        max = clock;

    if (i++ >= DEPTH-1) {
        uint64_t varsum = 0;
        int32_t vardelta;

        avg = sum / DEPTH;
        for (i = 0; i < DEPTH; i++) {
            vardelta = (int32_t)c_delta[i] - avg;
            vardelta *= vardelta;
            varsum += vardelta;
        }
        var = varsum / DEPTH;
        output("cstartup min=%u max=%u avg=%u var=%u", min, max, avg, var);

        sum = max = i = 0;
        min = 0-1;
    }
}
DECL_TASK(hsevar_task);
