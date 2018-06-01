
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <cpuid.h>

char *features_str[] =
{
    "Supports FPU",
    "Supports VME",
    "Supports DE",
    "Supports PSE",
    "Supports TSC",
    "Supports MSR",
    "Supports PAE",
    "Supports MCE",
    "Supports CX8",
    "Supports APIC",
    "Supports SEP",
    "Supports MTRR",
    "Supports PGE",
    "Supports MCA",
    "Supports CMOV",
    "Supports PAT",
    "Supports PSE36",
    "Supports PSN",
    "Supports CLF",
    "Supports DTES",
    "Supports ACPI",
    "Supports MMX",
    "Supports FXSR",
    "Supports SSE",
    "Supporst SSE2",
    "Supports SS",
    "Supports HTT",
    "Supports TM1",
    "Supports IA64",
    "Supports PBE",
    "Supports SSE3",
    "Supports PCLMUL",
    "Supports DTES64",
    "Supports Monitor",
    "Supports DS CPL",
    "Supports VMX",
    "Supports SMX",
    "Supports SSSE3",
    "Supports CID",
    "Supports FMA",
    "Supports CX16",
    "Supports ETPRD",
    "Supports PDCM",
    "Supports PCIDE",
    "Supports DCA",
    "Supports SSE4",
    "Supports SSE4",
    "Supports x2 APIC", 
    "Supports MOVBE",
    "Supports POPCNT",
    "Supports AES",
    "Supports XSAVE",
    "Supports OSXSAVE",
    "Supports AVX"
};

void cpuid_init()
{
    //bool supported = _cpuid_check();
    
    uint32_t features[2];
    char vendorid[8];
    _cpuid_get_vendorid(vendorid);

    printstr("VendorID String: ");
    printstr(vendorid);
}