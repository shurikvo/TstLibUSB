
#include "pch.h"
#include <iostream>
#include "libusb.h"

static void PrintEndpointComp(const struct libusb_ss_endpoint_companion_descriptor *EpComp)
{
	printf("      USB 3.0 Endpoint Companion:\n");
	printf("        bMaxBurst:           %u\n", EpComp->bMaxBurst);
	printf("        bmAttributes:        %02xh\n", EpComp->bmAttributes);
	printf("        wBytesPerInterval:   %u\n", EpComp->wBytesPerInterval);
}

static void PrintEndpoint(const struct libusb_endpoint_descriptor *Endpoint)
{
	int i, ret;

	printf("      Endpoint:\n");
	printf("        bEndpointAddress:    %02xh\n", Endpoint->bEndpointAddress);
	printf("        bmAttributes:        %02xh\n", Endpoint->bmAttributes);
	printf("        wMaxPacketSize:      %u\n", Endpoint->wMaxPacketSize);
	printf("        bInterval:           %u\n", Endpoint->bInterval);
	printf("        bRefresh:            %u\n", Endpoint->bRefresh);
	printf("        bSynchAddress:       %u\n", Endpoint->bSynchAddress);

	for (i = 0; i < Endpoint->extra_length;) {
		if (LIBUSB_DT_SS_ENDPOINT_COMPANION == Endpoint->extra[i + 1]) {
			struct libusb_ss_endpoint_companion_descriptor *EpComp;

			ret = libusb_get_ss_endpoint_companion_descriptor(NULL, Endpoint, &EpComp);
			if (LIBUSB_SUCCESS != ret)
				continue;

			PrintEndpointComp(EpComp);

			libusb_free_ss_endpoint_companion_descriptor(EpComp);
		}

		i += Endpoint->extra[i];
	}
}

static void PrintAltSetting(const struct libusb_interface_descriptor *Interface)
{
	uint8_t i;

	printf("    Interface:\n");
	printf("      bInterfaceNumber:      %u\n", Interface->bInterfaceNumber);
	printf("      bAlternateSetting:     %u\n", Interface->bAlternateSetting);
	printf("      bNumEndpoints:         %u\n", Interface->bNumEndpoints);
	printf("      bInterfaceClass:       %u\n", Interface->bInterfaceClass);
	printf("      bInterfaceSubClass:    %u\n", Interface->bInterfaceSubClass);
	printf("      bInterfaceProtocol:    %u\n", Interface->bInterfaceProtocol);
	printf("      iInterface:            %u\n", Interface->iInterface);

	for (i = 0; i < Interface->bNumEndpoints; i++)
		PrintEndpoint(&Interface->endpoint[i]);
}

static void PrintInterface(const struct libusb_interface *Interface)
{
	for (int i = 0; i < Interface->num_altsetting; i++)
		PrintAltSetting(&Interface->altsetting[i]);
}

static void PrintConfiguration(struct libusb_config_descriptor *Config)
{
	uint8_t i;

	printf("  Configuration:\n");
	printf("    wTotalLength:            %u\n", Config->wTotalLength);
	printf("    bNumInterfaces:          %u\n", Config->bNumInterfaces);
	printf("    bConfigurationValue:     %u\n", Config->bConfigurationValue);
	printf("    iConfiguration:          %u\n", Config->iConfiguration);
	printf("    bmAttributes:            %02xh\n", Config->bmAttributes);
	printf("    MaxPower:                %u\n", Config->MaxPower);

	for (i = 0; i < Config->bNumInterfaces; i++)
		PrintInterface(&Config->interface[i]);
}

static void Print20ExtCap(struct libusb_usb_2_0_extension_descriptor *usb_2_0_ext_cap)
{
	printf("    USB 2.0 Extension Capabilities:\n");
	printf("      bDevCapabilityType:    %u\n", usb_2_0_ext_cap->bDevCapabilityType);
	printf("      bmAttributes:          %08xh\n", usb_2_0_ext_cap->bmAttributes);
}

static void PrintSSUSBCap(struct libusb_ss_usb_device_capability_descriptor *ss_usb_cap)
{
	printf("    USB 3.0 Capabilities:\n");
	printf("      bDevCapabilityType:    %u\n", ss_usb_cap->bDevCapabilityType);
	printf("      bmAttributes:          %02xh\n", ss_usb_cap->bmAttributes);
	printf("      wSpeedSupported:       %u\n", ss_usb_cap->wSpeedSupported);
	printf("      bFunctionalitySupport: %u\n", ss_usb_cap->bFunctionalitySupport);
	printf("      bU1devExitLat:         %u\n", ss_usb_cap->bU1DevExitLat);
	printf("      bU2devExitLat:         %u\n", ss_usb_cap->bU2DevExitLat);
}

static void print_bos(libusb_device_handle *handle)
{
	struct libusb_bos_descriptor *bos;
	uint8_t i;
	int ret;

	ret = libusb_get_bos_descriptor(handle, &bos);
	if (ret < 0)
		return;

	printf("  Binary Object Store (BOS):\n");
	printf("    wTotalLength:            %u\n", bos->wTotalLength);
	printf("    bNumDeviceCaps:          %u\n", bos->bNumDeviceCaps);

	for (i = 0; i < bos->bNumDeviceCaps; i++) {
		struct libusb_bos_dev_capability_descriptor *dev_cap = bos->dev_capability[i];

		if (dev_cap->bDevCapabilityType == LIBUSB_BT_USB_2_0_EXTENSION) {
			struct libusb_usb_2_0_extension_descriptor *usb_2_0_extension;

			ret = libusb_get_usb_2_0_extension_descriptor(NULL, dev_cap, &usb_2_0_extension);
			if (ret < 0)
				return;

			Print20ExtCap(usb_2_0_extension);
			libusb_free_usb_2_0_extension_descriptor(usb_2_0_extension);
		}
		else if (dev_cap->bDevCapabilityType == LIBUSB_BT_SS_USB_DEVICE_CAPABILITY) {
			struct libusb_ss_usb_device_capability_descriptor *ss_dev_cap;

			ret = libusb_get_ss_usb_device_capability_descriptor(NULL, dev_cap, &ss_dev_cap);
			if (ret < 0)
				return;

			PrintSSUSBCap(ss_dev_cap);
			libusb_free_ss_usb_device_capability_descriptor(ss_dev_cap);
		}
	}

	libusb_free_bos_descriptor(bos);
}

static void PrintDeviceInfo(libusb_device *Dev)
{
	struct libusb_device_descriptor Desc;
	libusb_device_handle *hDev = NULL;
	unsigned char sB[1000];
	const char *sSpeed;
	int i, RC;

	switch (libusb_get_device_speed(Dev)) {
	case LIBUSB_SPEED_LOW:		sSpeed = "1.5M"; break;
	case LIBUSB_SPEED_FULL:		sSpeed = "12M"; break;
	case LIBUSB_SPEED_HIGH:		sSpeed = "480M"; break;
	case LIBUSB_SPEED_SUPER:	sSpeed = "5G"; break;
	case LIBUSB_SPEED_SUPER_PLUS:	sSpeed = "10G"; break;
	default:			sSpeed = "Unknown";
	}

	RC = libusb_get_device_descriptor(Dev, &Desc);
	if (RC < 0) {
		std::cerr << "libusb_get_device_descriptor error: " << RC << ": " << libusb_strerror(RC) << "\n";
		return;
	}

	printf("Dev (bus %u, device %u): %04X - %04X speed: %4.4s\n",
		libusb_get_bus_number(Dev), libusb_get_device_address(Dev),
		Desc.idVendor, Desc.idProduct, sSpeed);

	RC = libusb_open(Dev, &hDev);
	if (RC < 0)
	{
		if (RC == -12)
			std::cerr << "Device not recognized\n";
		else
			std::cerr << "libusb_open error: " << RC << ": " << libusb_strerror(RC) << "\n";
		return;

	}

	if (hDev)
	{
		if (Desc.iManufacturer) {
			RC = libusb_get_string_descriptor_ascii(hDev, Desc.iManufacturer, sB, sizeof(sB));
			if (RC > 0)
				std::cout << " - Manufacturer: " << (char *)sB;
		}

		if (Desc.iProduct) {
			RC = libusb_get_string_descriptor_ascii(hDev, Desc.iProduct, sB, sizeof(sB));
			if (RC > 0)
				std::cout << " - Product: " << (char *)sB;
		}

		if (Desc.iSerialNumber) {
			RC = libusb_get_string_descriptor_ascii(hDev, Desc.iSerialNumber, sB, sizeof(sB));
			if (RC > 0)
				std::cout << " - Serial Number: " << (char *)sB;
		}
		std::cout << "\n";
	}

	for (i = 0; i < Desc.bNumConfigurations; i++) {
		struct libusb_config_descriptor *Config;

		RC = libusb_get_config_descriptor(Dev, i, &Config);
		if (LIBUSB_SUCCESS != RC) {
			printf("  Couldn't retrieve descriptors\n");
			continue;
		}

		PrintConfiguration(Config);

		libusb_free_config_descriptor(Config);
	}

	if (hDev && Desc.bcdUSB >= 0x0201)
		print_bos(hDev);

	if (hDev)
		libusb_close(hDev);
}

int main()
{
	const char *sDevName = NULL;
	ssize_t uCount;
	int i, RC;

	libusb_context *LibContext;
	libusb_device **Devs;

	RC = libusb_init(NULL);
	if (RC < 0)
	{
		std::cerr << "libusb_init error: " << RC << "\n";
		return 0;
	}
	
	uCount = libusb_get_device_list(NULL, &Devs);
	if (uCount < 0)
	{
		std::cerr << "libusb_get_device_list error: " << uCount << "\n";
		libusb_exit(NULL);
		return 1;
	}
	std::cout << "Device count: " << uCount << "\n";

	for (i = 0; Devs[i]; i++)
		PrintDeviceInfo(Devs[i]);

	libusb_free_device_list(Devs, 1);
	libusb_exit(NULL);
	std::cout << "OK\n";
}

/*
Device count: 6
Dev (bus 1, device 4): 046D - C050 speed: 1.5M
 - Manufacturer: Logitech - Product: USB-PS/2 Optical Mouse
  Configuration:
	wTotalLength:            34
	bNumInterfaces:          1
	bConfigurationValue:     1
	iConfiguration:          0
	bmAttributes:            a0h
	MaxPower:                49
	Interface:
	  bInterfaceNumber:      0
	  bAlternateSetting:     0
	  bNumEndpoints:         1
	  bInterfaceClass:       3
	  bInterfaceSubClass:    1
	  bInterfaceProtocol:    2
	  iInterface:            0
	  Endpoint:
		bEndpointAddress:    81h
		bmAttributes:        03h
		wMaxPacketSize:      5
		bInterval:           10
		bRefresh:            0
		bSynchAddress:       0
Dev (bus 1, device 2): 072F - 2224 speed:  12M
Device not recognized
Dev (bus 1, device 1): 076B - 3021 speed:  12M
Device not recognized
Dev (bus 1, device 7): 0C2E - 0901 speed:  12M
 - Manufacturer: Honeywell Imaging & Mobility     - Product: HID POS - Serial Number: 16011B35A5
  Configuration:
	wTotalLength:            73
	bNumInterfaces:          2
	bConfigurationValue:     1
	iConfiguration:          3
	bmAttributes:            a0h
	MaxPower:                200
	Interface:
	  bInterfaceNumber:      0
	  bAlternateSetting:     0
	  bNumEndpoints:         2
	  bInterfaceClass:       3
	  bInterfaceSubClass:    1
	  bInterfaceProtocol:    1
	  iInterface:            4
	  Endpoint:
		bEndpointAddress:    84h
		bmAttributes:        03h
		wMaxPacketSize:      64
		bInterval:           8
		bRefresh:            0
		bSynchAddress:       0
	  Endpoint:
		bEndpointAddress:    05h
		bmAttributes:        03h
		wMaxPacketSize:      64
		bInterval:           8
		bRefresh:            0
		bSynchAddress:       0
	Interface:
	  bInterfaceNumber:      1
	  bAlternateSetting:     0
	  bNumEndpoints:         2
	  bInterfaceClass:       3
	  bInterfaceSubClass:    0
	  bInterfaceProtocol:    0
	  iInterface:            5
	  Endpoint:
		bEndpointAddress:    82h
		bmAttributes:        03h
		wMaxPacketSize:      64
		bInterval:           8
		bRefresh:            0
		bSynchAddress:       0
	  Endpoint:
		bEndpointAddress:    07h
		bmAttributes:        03h
		wMaxPacketSize:      64
		bInterval:           1
		bRefresh:            0
		bSynchAddress:       0
Dev (bus 1, device 3): 1C4F - 0002 speed: 1.5M
 - Manufacturer: SIGMACHIP - Product: USB Keyboard
  Configuration:
	wTotalLength:            59
	bNumInterfaces:          2
	bConfigurationValue:     1
	iConfiguration:          0
	bmAttributes:            a0h
	MaxPower:                49
	Interface:
	  bInterfaceNumber:      0
	  bAlternateSetting:     0
	  bNumEndpoints:         1
	  bInterfaceClass:       3
	  bInterfaceSubClass:    1
	  bInterfaceProtocol:    1
	  iInterface:            0
	  Endpoint:
		bEndpointAddress:    81h
		bmAttributes:        03h
		wMaxPacketSize:      8
		bInterval:           10
		bRefresh:            0
		bSynchAddress:       0
	Interface:
	  bInterfaceNumber:      1
	  bAlternateSetting:     0
	  bNumEndpoints:         1
	  bInterfaceClass:       3
	  bInterfaceSubClass:    0
	  bInterfaceProtocol:    0
	  iInterface:            0
	  Endpoint:
		bEndpointAddress:    82h
		bmAttributes:        03h
		wMaxPacketSize:      3
		bInterval:           10
		bRefresh:            0
		bSynchAddress:       0
Dev (bus 1, device 0): 8086 - A2AF speed:   5G
Device not recognized
OK
*/