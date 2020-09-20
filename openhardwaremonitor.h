#include "openhardwaremonitor_defs.h"

namespace ohm
{
    // read_msr - receives a handle to openhardwaremonitorlib.sys, the msr id and a pointer to
    // a buffer which receives the msr value
    //
    bool read_msr( HANDLE h_device, unsigned long msr_id, unsigned long long* ptr_msr );

    // write_msr - receives a handle to openhardwaremonitorlib.sys, the msr id and the new msr value
    //
    bool write_msr( HANDLE h_device, unsigned long msr_id, unsigned long long value );
}