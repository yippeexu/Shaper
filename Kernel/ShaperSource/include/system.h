
#include <stdint.h>

extern void _sys_enableinterrupts();
extern void _sys_disableinterrupts();
extern void _sys_v86mode(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
extern void _sys_realmode();
extern void _sys_enablepaging(uint32_t pagedir_address);