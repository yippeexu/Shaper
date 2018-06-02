
#include <stdint.h>
#include <sysprint.h>
#include <irq.h>
#include <pic.h>
#include <io.h>

static uint64_t timerticks = 0;
static uint64_t stopwatch_ticks = 0;
static uint64_t execute_ticks = 0;
static uint64_t execute_time = 0;
static bool stopwatch_enabled = false;
static bool execute_always = false;
static void (*daily_routine)() = 0;

void pit_handler(x86_regs *r);
void timer_phase(int hz);

void pit_handler(x86_regs *r)
{
	timerticks++;
	
	if (daily_routine) execute_ticks++;
	if (stopwatch_enabled) stopwatch_ticks++;

	if (daily_routine && !(execute_ticks % execute_time)) {
		if (daily_routine && !execute_always) {
			r->ebx |= 0x80000000;
			execute_time = 0;
			execute_ticks = 0;
			daily_routine = 0;
		} else if (daily_routine && execute_always) {
			r->ebx |= 0x80000000;
		}
	}
}

void pit_init()
{
	_irq_unmask(0x00);
	_irq_add_handler(0x00, pit_handler);
	timer_phase(1000);
}

void pit_mask(void (*mask)())
{
	if (mask == daily_routine)
	{
		daily_routine = 0;
	}
}

void pit_unmask(void (*mask)())
{
	daily_routine = mask;
}

void timer_phase(int hz)
{
	int divisor = 1193180 / hz;
	outb(0x43, 0x36);
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
}

void sleep(int milliseconds_timeout)
{
	uint64_t current_timerticks = timerticks;
	while (timerticks < (current_timerticks + milliseconds_timeout));
}

void stopwatch_start()
{
	stopwatch_ticks = 0;
	stopwatch_enabled = true;
}

void stopwatch_reset()
{
	stopwatch_ticks = 0;
}

void stopwatch_stop()
{
	stopwatch_enabled = false;
}

uint32_t stopwatch_get_ms()
{
	return stopwatch_ticks;
}

void execute_after(void (*routine)(), int milliseconds_timeout, bool always)
{
	daily_routine = routine;
	execute_time = milliseconds_timeout;
	execute_always = always;
	execute_ticks = 0;
}