
void sleep(int milliseconds_timeout);
void execute_after(void (*routine)(), int milliseconds_timeout, bool always);
void stopwatch_start();
void stopwatch_reset();
void stopwatch_stop();
void pit_mask(void (*mask)());
void pit_unmask(void (*mask)());
void pit_init();