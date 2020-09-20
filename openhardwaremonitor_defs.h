#define READ_MSR_IOCTL 0x9C402084
#define WRITE_MSR_IOCTL 0x9C402088

#pragma pack(1)
struct WRITE_MSR_STRUCT
{
	unsigned long IdMSR;
	unsigned __int64 ValueMSR;
};
