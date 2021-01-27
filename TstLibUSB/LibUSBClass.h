#pragma once

#include "libusb.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef int     (__stdcall *LIBUSB_API_INIT                       )(libusb_context**);
	typedef int     (__stdcall *LIBUSB_API_SET_OPTION                 )(libusb_context*, libusb_option);
	typedef char   *(__stdcall *LIBUSB_API_STRERROR                   )(int);
	typedef ssize_t (__stdcall *LIBUSB_API_GET_DEVICE_LIST            )(libusb_context *, libusb_device ***);
	typedef void    (__stdcall *LIBUSB_API_FREE_DEVICE_LIST           )(libusb_device **, int);
	typedef int     (__stdcall *LIBUSB_API_GET_DEVICE_SPEED           )(libusb_device *);
	typedef int     (__stdcall *LIBUSB_API_GET_DEVICE_DESCRIPTOR      )(libusb_device *, struct libusb_device_descriptor *);
	typedef uint8_t (__stdcall *LIBUSB_API_GET_DEVICE_ADDRESS         )(libusb_device *);
	typedef uint8_t (__stdcall *LIBUSB_API_GET_BUS_NUMBER             )(libusb_device *);
	typedef int     (__stdcall *LIBUSB_API_OPEN                       )(libusb_device *, libusb_device_handle **);
	typedef void    (__stdcall *LIBUSB_API_CLOSE                      )(libusb_device_handle *);
	typedef int     (__stdcall *LIBUSB_API_GET_STRING_DESCRIPTOR_ASCII)(libusb_device_handle *, uint8_t, unsigned char *, int);
	typedef int     (__stdcall *LIBUSB_API_GET_CONFIG_DESCRIPTOR      )(libusb_device *, uint8_t, struct libusb_config_descriptor **);
	typedef void    (__stdcall *LIBUSB_API_FREE_CONFIG_DESCRIPTOR     )(struct libusb_config_descriptor *);


#ifdef __cplusplus
}
#endif

class LibUSBClass
{
private:
	libusb_context *LibContext;

public:
	int DevCount = 0;
	char DllPath[500], Message[500];

	libusb_device **Devs;

	LibUSBClass();
	~LibUSBClass();

	int     Init ();
	int     SetOption (libusb_option Option);
	char   *StrError (int Err);
	int     GetDeviceList ();
	void    FreeDeviceList (int UnrefDevicess);
	int     GetDeviceSpeed (int DevNumber);
	int     GetDeviceDescriptor (int DevNumber, struct libusb_device_descriptor *Descr);
	uint8_t GetDeviceAddress (int DevNumber);
	uint8_t GetBusNumber (int DevNumber);
	int     OpenDevice (int nDevNumber, libusb_device_handle **DevHandle);
	void    CloseDevice (libusb_device_handle *DevHandle);
	int     GetStringDescriptorAscii(libusb_device_handle *DevHandle, uint8_t DescIndex, unsigned char *Data, int Length);
	int     GetConfigDescriptor(int nDevNumber, uint8_t ConfigIndex, struct libusb_config_descriptor **Config);
	void    FreeConfigDescriptor(struct libusb_config_descriptor *Config);
};

