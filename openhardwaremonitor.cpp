#include <windows.h>

#include "openhardwaremonitor.h"

namespace ohm
{
    // read_msr - receives a handle to openhardwaremonitorlib.sys, the msr id and a pointer to
    // a buffer which receives the msr value
    //
    bool read_msr( HANDLE h_device, unsigned long msr_id, unsigned long long* ptr_msr )
    {
        DWORD num_returned_bytes;

        return h_device != nullptr &&
            DeviceIoControl( h_device, READ_MSR_IOCTL, &msr_id, sizeof( msr_id ),
                ptr_msr, 0x8, &num_returned_bytes, nullptr );
    }

    // write_msr - receives a handle to openhardwaremonitorlib.sys, the msr id and the new msr value
    //
    bool write_msr( HANDLE h_device, unsigned long msr_id, unsigned long long value )
    {
        if ( !h_device )
            return false;
        
        // Creates Write MSR Structure
        //
        WRITE_MSR_STRUCT write_msr;
        write_msr.id = msr_id;
        write_msr.value = value;

        DWORD num_returned_bytes;

        return DeviceIoControl( h_device, WRITE_MSR_IOCTL, &write_msr, sizeof( write_msr ), 
            nullptr, NULL, &num_returned_bytes, nullptr );
    }
}