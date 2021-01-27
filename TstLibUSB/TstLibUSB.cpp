#include "pch.h"
#include <iostream>
#include "LibUSBClass.h"

int main()
{
	int i, RC, nCount;
	char sB[1000] = "";

	LibUSBClass Luc;
	struct libusb_device_descriptor Desc;
	libusb_device_handle *hDev;

	RC = Luc.Init();
	std::cout << "Luc.Init: " << Luc.Message << "\n";
	if (RC != 0)
		return 0;

	nCount = Luc.GetDeviceList();
	std::cout << "Luc.GetDeviceList: " << nCount << "\n";

	for (i = 0; i < Luc.DevCount; ++i)
	{
		sprintf_s(sB, "");

		uint8_t uDA = 0, uBN = 0;
		uDA = Luc.GetDeviceAddress(i);
		uBN = Luc.GetBusNumber(i);

		RC = Luc.GetDeviceDescriptor(i, &Desc);
		if (RC == 0)
			sprintf(sB + strlen(sB), "(bus %u, device %u): %04X - %04X ", uBN, uDA, Desc.idVendor, Desc.idProduct);
		else
			std::cout << "Luc.GetDeviceDescriptor error: " << RC << ": " << Luc.StrError(RC) << "\n";

		Luc.GetDeviceSpeed(i);
		std::cout << "USB " << (i + 1) << ": " << sB << "speed: " << Luc.Message << "\n";

		RC = Luc.OpenDevice(1, &hDev);
		if (RC == 0)
		{
			if (hDev)
			{
				std::cout << "  - ";
				if (Desc.iManufacturer)
				{
					RC = Luc.GetStringDescriptorAscii(hDev, Desc.iManufacturer, (unsigned char *)sB, 256);
					if (RC > 0) std::cout << "Manufacturer: " << sB << " ";
				}
				if (Desc.idProduct)
				{
					RC = Luc.GetStringDescriptorAscii(hDev, Desc.idProduct, (unsigned char *)sB, 256);
					if (RC > 0) std::cout << "Product: " << sB << " ";
				}
				if (Desc.iSerialNumber)
				{
					RC = Luc.GetStringDescriptorAscii(hDev, Desc.iSerialNumber, (unsigned char *)sB, 256);
					if (RC > 0) std::cout << "Serial Number: " << sB << " ";
				}
				std::cout << "\n";



				Luc.CloseDevice(hDev);
			}
		}
		else
			std::cout << "Luc.OpenDevice error: " << RC << ": " << Luc.StrError(RC) << "\n";
	}

	Luc.FreeDeviceList(1);
    std::cout << "OK\n"; 
}
/*
Luc.Init: OK
Luc.GetDeviceList: 6
USB 1: (bus 1, device 4): 046D - C050 speed: 1.5M
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
USB 2: (bus 1, device 2): 072F - 2224 speed: 12M
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
USB 3: (bus 1, device 1): 076B - 3021 speed: 12M
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
USB 4: (bus 1, device 5): 0C2E - 0901 speed: 12M
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
USB 5: (bus 1, device 3): 1C4F - 0002 speed: 1.5M
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
USB 6: (bus 1, device 0): 8086 - A2AF speed: 5G
Luc.OpenDevice error: -12: Operation not supported or unimplemented on this platform
OK
*/
