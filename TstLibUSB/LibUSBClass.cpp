#include "pch.h"
#include "LibUSBClass.h"

HINSTANCE hDllLibUSB = NULL;
LIBUSB_API_INIT hInit = NULL;
LIBUSB_API_SET_OPTION hSetOption = NULL;
LIBUSB_API_STRERROR hStrError = NULL;
LIBUSB_API_GET_DEVICE_LIST hGetDeviceList = NULL;
LIBUSB_API_FREE_DEVICE_LIST hFreeDeviceList = NULL;
LIBUSB_API_GET_DEVICE_SPEED hGetDeviceSpeed = NULL;
LIBUSB_API_GET_DEVICE_DESCRIPTOR hGetDeviceDescriptor = NULL;
LIBUSB_API_GET_DEVICE_ADDRESS hGetDeviceAddress = NULL;
LIBUSB_API_GET_BUS_NUMBER hGetBusNumber = NULL;
LIBUSB_API_OPEN hOpen = NULL;
LIBUSB_API_CLOSE hClose = NULL;
LIBUSB_API_GET_STRING_DESCRIPTOR_ASCII hGetStringDescriptorAscii = NULL;
LIBUSB_API_GET_CONFIG_DESCRIPTOR hGetConfigDescriptor = NULL;
LIBUSB_API_FREE_CONFIG_DESCRIPTOR hFreeConfigDescriptor = NULL;


int LibUSBClass::Init()
{
	int RC;
	char sFunc[100];
	wchar_t wtext[500];
	//-------------------------------------------------------------------------
	mbstowcs(wtext, DllPath, strlen(DllPath) + 1);
	hDllLibUSB = LoadLibrary(wtext);
	if (hDllLibUSB == NULL)
	{
		sprintf_s(Message, "Cannot load library  %s", DllPath);
		return -1;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_init");
	hInit = (LIBUSB_API_INIT)GetProcAddress(hDllLibUSB, sFunc);
	if (hInit == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_set_option");
	hSetOption = (LIBUSB_API_SET_OPTION)GetProcAddress(hDllLibUSB, sFunc);
	if (hSetOption == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_strerror");
	hStrError = (LIBUSB_API_STRERROR)GetProcAddress(hDllLibUSB, sFunc);
	if (hStrError == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_device_list");
	hGetDeviceList = (LIBUSB_API_GET_DEVICE_LIST)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetDeviceList == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_free_device_list");
	hFreeDeviceList = (LIBUSB_API_FREE_DEVICE_LIST)GetProcAddress(hDllLibUSB, sFunc);
	if (hFreeDeviceList == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_device_speed");
	hGetDeviceSpeed = (LIBUSB_API_GET_DEVICE_SPEED)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetDeviceSpeed == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_device_descriptor");
	hGetDeviceDescriptor = (LIBUSB_API_GET_DEVICE_DESCRIPTOR)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetDeviceDescriptor == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_device_address");
	hGetDeviceAddress = (LIBUSB_API_GET_DEVICE_ADDRESS)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetDeviceAddress == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_bus_number");
	hGetBusNumber = (LIBUSB_API_GET_BUS_NUMBER)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetBusNumber == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_open");
	hOpen = (LIBUSB_API_OPEN)GetProcAddress(hDllLibUSB, sFunc);
	if (hOpen == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_close");
	hClose = (LIBUSB_API_CLOSE)GetProcAddress(hDllLibUSB, sFunc);
	if (hClose == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_string_descriptor_ascii");
	hGetStringDescriptorAscii = (LIBUSB_API_GET_STRING_DESCRIPTOR_ASCII)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetStringDescriptorAscii == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_get_config_descriptor");
	hGetConfigDescriptor = (LIBUSB_API_GET_CONFIG_DESCRIPTOR)GetProcAddress(hDllLibUSB, sFunc);
	if (hGetConfigDescriptor == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	sprintf_s(sFunc, "libusb_free_config_descriptor");
	hFreeConfigDescriptor = (LIBUSB_API_FREE_CONFIG_DESCRIPTOR)GetProcAddress(hDllLibUSB, sFunc);
	if (hFreeConfigDescriptor == NULL)
	{
		sprintf_s(Message, "Cannot get proc addres %s", sFunc);
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return -2;
	}
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	RC = (hInit)(&LibContext);
	if (RC != 0)
	{
		sprintf_s(Message, "libusb_init error: %d : %s", RC, StrError(RC));
		FreeLibrary(hDllLibUSB);
		hDllLibUSB = NULL;
		return RC;
	}
	sprintf_s(Message, "OK");
	return 0;
}



int LibUSBClass::OpenDevice(int nDevNumber, libusb_device_handle **DevHandle)
{
	return (hOpen)(*(Devs + nDevNumber), DevHandle);
}

void LibUSBClass::CloseDevice(libusb_device_handle *DevHandle)
{
	(hClose)(DevHandle);
}

int LibUSBClass::GetStringDescriptorAscii(libusb_device_handle *DevHandle, uint8_t DescIndex, unsigned char *Data, int Length)
{
	return (hGetStringDescriptorAscii)(DevHandle, DescIndex, Data, Length);
}

int LibUSBClass::GetConfigDescriptor(int nDevNumber, uint8_t ConfigIndex, struct libusb_config_descriptor **Config)
{
	return (hGetConfigDescriptor)(*(Devs + nDevNumber), ConfigIndex, Config);
}

void LibUSBClass::FreeConfigDescriptor(struct libusb_config_descriptor *Config)
{
	(hFreeConfigDescriptor)(Config);
}

int LibUSBClass::GetDeviceSpeed(int nDevNumber)
{
	int nSpeed;

	nSpeed = (hGetDeviceSpeed)(*(Devs + nDevNumber));

	switch (nSpeed)
	{
	case LIBUSB_SPEED_LOW:		sprintf_s(Message, "1.5M"); break;
	case LIBUSB_SPEED_FULL:		sprintf_s(Message, "12M"); break;
	case LIBUSB_SPEED_HIGH:		sprintf_s(Message, "480M"); break;
	case LIBUSB_SPEED_SUPER:	sprintf_s(Message, "5G"); break;
	case LIBUSB_SPEED_SUPER_PLUS:	sprintf_s(Message, "10G"); break;
	default:			sprintf_s(Message, "Unknown");
	}
	return nSpeed;
}

int LibUSBClass::GetDeviceDescriptor(int nDevNumber, struct libusb_device_descriptor *pDescr)
{
	int RC;

	RC = (hGetDeviceDescriptor)(*(Devs + nDevNumber), pDescr);

	return RC;
}

uint8_t LibUSBClass::GetDeviceAddress(int nDevNumber)
{
	return (hGetDeviceAddress)(*(Devs + nDevNumber));
}

uint8_t LibUSBClass::GetBusNumber(int nDevNumber)
{
	uint8_t uBN = (hGetBusNumber)(*(Devs + nDevNumber));
	return uBN;
}

int LibUSBClass::GetDeviceList()
{
	DevCount = (hGetDeviceList)(LibContext, &Devs);

	return DevCount;
}

void LibUSBClass::FreeDeviceList(int nUnrefDevicess)
{
	(hFreeDeviceList)(Devs, nUnrefDevicess);
}

int LibUSBClass::SetOption(libusb_option Option)
{
	return (hSetOption)(LibContext, Option);
}

char *LibUSBClass::StrError(int Err)
{
	char *s, sR[] = "";

	s = (hStrError)(Err);
	if (s == NULL)
		return sR;
	else
		return s;
}

LibUSBClass::LibUSBClass()
{
	sprintf_s(DllPath, "libusb-1.0.dll");
	sprintf_s(Message, "");
}

LibUSBClass::~LibUSBClass()
{
	FreeLibrary(hDllLibUSB);
	hDllLibUSB = NULL;
}
