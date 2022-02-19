#include <stdlib.h>
#include "log.hpp"
#import <IOKit/serial/IOSerialKeys.h>
#include <CoreFoundation/CoreFoundation.h>
#include "../include/razerdevice.h"

static IONotificationPortRef    gNotifyPort;
static io_iterator_t            gAddedIter;
static CFRunLoopRef             gRunLoop;

bool is_razer_device(IOUSBDeviceInterface **dev)
{
	kern_return_t kr;
	UInt16 vendor;
	UInt16 product;
	UInt16 release;

	kr = (*dev)->GetDeviceVendor(dev, &vendor);
	kr = (*dev)->GetDeviceProduct(dev, &product);
	kr = (*dev)->GetDeviceReleaseNumber(dev, &release);

	return vendor == USB_VENDOR_ID_RAZER;
}

bool is_keyboard(IOUSBDeviceInterface **usb_dev)
{
	UInt16 product = -1;
	(*usb_dev)->GetDeviceProduct(usb_dev, &product);
	debug("prod = 0x%x", product);

	switch (product)
	{
	case USB_DEVICE_ID_RAZER_NOSTROMO:
	case USB_DEVICE_ID_RAZER_ORBWEAVER:
	case USB_DEVICE_ID_RAZER_ORBWEAVER_CHROMA:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_STEALTH:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_STEALTH_EDITION:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_ULTIMATE_2012:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_ULTIMATE_2013:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_ULTIMATE_2016:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_X_ULTIMATE:
	case USB_DEVICE_ID_RAZER_TARTARUS:
	case USB_DEVICE_ID_RAZER_TARTARUS_CHROMA:
	case USB_DEVICE_ID_RAZER_TARTARUS_V2:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_OVERWATCH:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_CHROMA:
	case USB_DEVICE_ID_RAZER_DEATHSTALKER_EXPERT:
	case USB_DEVICE_ID_RAZER_DEATHSTALKER_CHROMA:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_CHROMA_TE:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_X_CHROMA:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_X_CHROMA_TE:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_LITE:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_2019:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_ESSENTIAL:
	case USB_DEVICE_ID_RAZER_ORNATA:
	case USB_DEVICE_ID_RAZER_ORNATA_CHROMA:
	case USB_DEVICE_ID_RAZER_ORNATA_CHROMA_V2:
	case USB_DEVICE_ID_RAZER_HUNTSMAN_ELITE:
	case USB_DEVICE_ID_RAZER_HUNTSMAN_TE:
	case USB_DEVICE_ID_RAZER_HUNTSMAN_TL:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_ELITE:
	case USB_DEVICE_ID_RAZER_HUNTSMAN:
	case USB_DEVICE_ID_RAZER_CYNOSA_CHROMA:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_CHROMA_V2:
	case USB_DEVICE_ID_RAZER_ANANSI:
	case USB_DEVICE_ID_RAZER_CYNOSA_V2:
	case USB_DEVICE_ID_RAZER_CYNOSA_LITE:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_V3:
	case USB_DEVICE_ID_RAZER_BLACKWIDOW_V3_TK:
	case USB_DEVICE_ID_RAZER_HUNTSMAN_MINI:
		return true;
	}

	return false;
}

bool is_mouse(IOUSBDeviceInterface **usb_dev)
{
	UInt16 product = -1;
	(*usb_dev)->GetDeviceProduct(usb_dev, &product);

	switch (product)
	{
	case USB_DEVICE_ID_RAZER_OROCHI_2011:
	case USB_DEVICE_ID_RAZER_DEATHADDER_3_5G:
	case USB_DEVICE_ID_RAZER_ABYSSUS_1800:
	case USB_DEVICE_ID_RAZER_MAMBA_2012_WIRED:
	case USB_DEVICE_ID_RAZER_MAMBA_2012_WIRELESS:
	case USB_DEVICE_ID_RAZER_NAGA_2012:
	case USB_DEVICE_ID_RAZER_IMPERATOR:
	case USB_DEVICE_ID_RAZER_OUROBOROS:
	case USB_DEVICE_ID_RAZER_TAIPAN:
	case USB_DEVICE_ID_RAZER_NAGA_HEX_RED:
	case USB_DEVICE_ID_RAZER_DEATHADDER_2013:
	case USB_DEVICE_ID_RAZER_DEATHADDER_1800:
	case USB_DEVICE_ID_RAZER_OROCHI_2013:
	case USB_DEVICE_ID_RAZER_NAGA_2014:
	case USB_DEVICE_ID_RAZER_NAGA_HEX:
	case USB_DEVICE_ID_RAZER_ABYSSUS:
	case USB_DEVICE_ID_RAZER_DEATHADDER_CHROMA:
	case USB_DEVICE_ID_RAZER_MAMBA_WIRED:
	case USB_DEVICE_ID_RAZER_MAMBA_WIRELESS:
	case USB_DEVICE_ID_RAZER_MAMBA_TE_WIRED:
	case USB_DEVICE_ID_RAZER_OROCHI_CHROMA:
	case USB_DEVICE_ID_RAZER_DIAMONDBACK_CHROMA:
	case USB_DEVICE_ID_RAZER_NAGA_HEX_V2:
	case USB_DEVICE_ID_RAZER_NAGA_CHROMA:
	case USB_DEVICE_ID_RAZER_DEATHADDER_3500:
	case USB_DEVICE_ID_RAZER_LANCEHEAD_WIRED:
	case USB_DEVICE_ID_RAZER_LANCEHEAD_WIRELESS:
	case USB_DEVICE_ID_RAZER_ABYSSUS_V2:
	case USB_DEVICE_ID_RAZER_DEATHADDER_ELITE:
	case USB_DEVICE_ID_RAZER_ABYSSUS_2000:
	case USB_DEVICE_ID_RAZER_LANCEHEAD_TE_WIRED:
	case USB_DEVICE_ID_RAZER_BASILISK:
	case USB_DEVICE_ID_RAZER_BASILISK_ULTIMATE:
	case USB_DEVICE_ID_RAZER_BASILISK_ULTIMATE_RECEIVER:
	case USB_DEVICE_ID_RAZER_NAGA_TRINITY:
	case USB_DEVICE_ID_RAZER_ABYSSUS_ELITE_DVA_EDITION:
	case USB_DEVICE_ID_RAZER_ABYSSUS_ESSENTIAL:
	case USB_DEVICE_ID_RAZER_MAMBA_ELITE:
	case USB_DEVICE_ID_RAZER_DEATHADDER_ESSENTIAL:
	case USB_DEVICE_ID_RAZER_LANCEHEAD_WIRELESS_RECEIVER:
	case USB_DEVICE_ID_RAZER_LANCEHEAD_WIRELESS_WIRED:
	case USB_DEVICE_ID_RAZER_DEATHADDER_ESSENTIAL_WHITE_EDITION:
	case USB_DEVICE_ID_RAZER_MAMBA_WIRELESS_RECEIVER:
	case USB_DEVICE_ID_RAZER_MAMBA_WIRELESS_WIRED:
	case USB_DEVICE_ID_RAZER_VIPER:
	case USB_DEVICE_ID_RAZER_VIPER_8KHZ:
	case USB_DEVICE_ID_RAZER_VIPER_MINI:
	case USB_DEVICE_ID_RAZER_BASILISK_V2:
	case USB_DEVICE_ID_RAZER_VIPER_ULTIMATE_WIRED:
	case USB_DEVICE_ID_RAZER_VIPER_ULTIMATE_WIRELESS:
	case USB_DEVICE_ID_RAZER_DEATHADDER_V2:
	case USB_DEVICE_ID_RAZER_DEATHADDER_V2_PRO_WIRED:
    case USB_DEVICE_ID_RAZER_DEATHADDER_V2_PRO_WIRELESS:
	case USB_DEVICE_ID_RAZER_DEATHADDER_V2_MINI:
		return true;
	}

	return false;
}

bool is_mouse_dock(IOUSBDeviceInterface **usb_dev)
{
	UInt16 product = -1;
	(*usb_dev)->GetDeviceProduct(usb_dev, &product);

	switch (product)
	{
	case USB_DEVICE_ID_RAZER_MOUSE_CHARGING_DOCK:
		return true;
	}

	return false;
}

bool is_mouse_mat(IOUSBDeviceInterface **usb_dev)
{
	UInt16 product = -1;
	(*usb_dev)->GetDeviceProduct(usb_dev, &product);

	switch (product)
	{
	case USB_DEVICE_ID_RAZER_FIREFLY_HYPERFLUX:
	case USB_DEVICE_ID_RAZER_FIREFLY:
	case USB_DEVICE_ID_RAZER_FIREFLY_V2:
	case USB_DEVICE_ID_RAZER_GOLIATHUS_CHROMA:
	case USB_DEVICE_ID_RAZER_GOLIATHUS_CHROMA_EXTENDED:
		return true;
	}

	return false;
}

bool is_egpu(IOUSBDeviceInterface **usb_dev)
{
	UInt16 product = -1;
	(*usb_dev)->GetDeviceProduct(usb_dev, &product);

	switch (product)
	{
	case USB_DEVICE_ID_RAZER_CORE_X_CHROMA:
		return true;
	}

	return false;
}


bool is_headphone(IOUSBDeviceInterface **usb_dev) 
{
    UInt16 product = -1;
    (*usb_dev)->GetDeviceProduct(usb_dev, &product);

    switch (product) 
    {
        case USB_DEVICE_ID_RAZER_KRAKEN_KITTY_EDITION:
        case USB_DEVICE_ID_RAZER_KRAKEN_V2:
        case USB_DEVICE_ID_RAZER_KRAKEN_ULTIMATE:
            return true;
    }

    return false;
}

bool is_accessory(IOUSBDeviceInterface **usb_dev)
{
    UInt16 product = -1;
    (*usb_dev)->GetDeviceProduct(usb_dev, &product);

    switch (product)
    {
        case USB_DEVICE_ID_RAZER_NOMMO_CHROMA:
        case USB_DEVICE_ID_RAZER_NOMMO_PRO:
        case USB_DEVICE_ID_RAZER_CHROMA_MUG:
        case USB_DEVICE_ID_RAZER_CHROMA_BASE:
        case USB_DEVICE_ID_RAZER_CHROMA_HDK:
        case USB_DEVICE_ID_RAZER_MOUSE_BUNGEE_V3_CHROMA:
        case USB_DEVICE_ID_RAZER_BASE_STATION_V2_CHROMA:
            return true;
    }

    return false;
}

IOUSBDeviceInterface **getRazerUSBDeviceInterface(int type)
{
	CFMutableDictionaryRef matchingDict;
	matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
	if (matchingDict == NULL)
	{
		return NULL;
	}

	io_iterator_t iter;
	kern_return_t kReturn =
		IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
	if (kReturn != kIOReturnSuccess)
	{
		return NULL;
	}

	io_service_t usbDevice;
	while ((usbDevice = IOIteratorNext(iter)))
	{
		IOCFPlugInInterface **plugInInterface = NULL;
		SInt32 score;

		kReturn = IOCreatePlugInInterfaceForService(
			usbDevice, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score);

		IOObjectRelease(usbDevice); // Not needed after plugin created
		if ((kReturn != kIOReturnSuccess) || plugInInterface == NULL)
		{
			// printf("Unable to create plugin (0x%08x)\n", kReturn);
			continue;
		}

		IOUSBDeviceInterface **dev = NULL;
		HRESULT hResult = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID *)&dev);

		(*plugInInterface)->Release(plugInInterface); // Not needed after device interface created
		if (hResult || !dev)
		{
			// printf("Couldn’t create a device interface (0x%08x)\n", (int) hResult);
			continue;
		}

		// Filter out non-Razer devices
		if (!is_razer_device(dev))
		{
			(*dev)->Release(dev);
			continue;
		}

		switch (type) 
        {
			case TYPE_KEYBOARD:
			case TYPE_BLADE:
				// Filter out non-keyboards and non-blade laptops
				if (!(is_keyboard(dev) || is_blade_laptop(dev))) 
                {
					(*dev)->Release(dev);
					continue;
				}
				break;
        
			case TYPE_MOUSE:
				// Filter out non-mice
				if (!is_mouse(dev)) 
                {
					(*dev)->Release(dev);
					continue;
				}
				break;
        
			case TYPE_MOUSE_DOCK:
				// Filter out non-mice-mats
				if (!is_mouse_dock(dev)) 
                {
					(*dev)->Release(dev);
					continue;
				}
				break;
        
			case TYPE_MOUSE_MAT:
				// Filter out non-mice-mats
				if (!is_mouse_mat(dev)) 
                {
					(*dev)->Release(dev);
					continue;
				}
				break;

 	        case TYPE_HEADPHONE:
                if (!is_headphone(dev))
                {
                  (*dev)->Release(dev);
                  continue;
                }
                break;

            case TYPE_EGPU:
			    // Filter out non-mice-mats
			    if (!is_egpu(dev))
			    {
				  (*dev)->Release(dev);
				  continue;
			    }
			  break;
            case TYPE_ACCESSORY:
                if (!is_accessory(dev))
                {
                    (*dev)->Release(dev);
                    continue;
                }
                break;
        
		    default:
			    // Unsupported Razer peripheral type
			    (*dev)->Release(dev);
			    continue;
		}

		kReturn = (*dev)->USBDeviceOpen(dev);
		if (kReturn != kIOReturnSuccess)
		{
			printf("Unable to open USB device: %08x\n", kReturn);
			(*dev)->Release(dev);
			continue;
		}

		// Success. We found the Razer USB device.
		// Caller is responsible for closing USB and release device.
		IOObjectRelease(iter);
		return dev;
	}

	IOObjectRelease(iter);
	return NULL;
}

void closeRazerUSBDeviceInterface(IOUSBDeviceInterface **dev)
{
	kern_return_t kr;
	kr = (*dev)->USBDeviceClose(dev);
	kr = (*dev)->Release(dev);
}

void SignalHandler(int sigraised)
{
	fprintf(stderr, "\nInterrupted.\n");

	exit(0);
}

RazerDevices getAllRazerDevices()
{

	CFMutableDictionaryRef  matchingDict;
	CFRunLoopSourceRef      runLoopSource;
	CFNumberRef             numberRef;
	kern_return_t           kr;
	long                    usbVendor = 0x1532;
	long                    usbProduct = 0x026b;
	sig_t                   oldHandler;

	// Set up a signal handler so we can clean up when we're interrupted from the command line
	// Otherwise we stay in our run loop forever.
	oldHandler = signal(SIGINT, SignalHandler);
	if (oldHandler == SIG_ERR) {
		fprintf(stderr, "Could not establish new signal handler.");
	}

	printf("Looking for devices matching vendor ID=0x%lx and product ID=0x%lx.\n", usbVendor, usbProduct);

	// Set up the matching criteria for the devices we're interested in. The matching criteria needs to follow
	// the same rules as kernel drivers: mainly it needs to follow the USB Common Class Specification, pp. 6-7.
	// See also Technical Q&A QA1076 "Tips on USB driver matching on Mac OS X"
	// <http://developer.apple.com/qa/qa2001/qa1076.html>.
	// One exception is that you can use the matching dictionary "as is", i.e. without adding any matching
	// criteria to it and it will match every IOUSBDevice in the system. IOServiceAddMatchingNotification will
	// consume this dictionary reference, so there is no need to release it later on.

	matchingDict = IOServiceMatching(kIOUSBDeviceClassName);    // Interested in instances of class
	// IOUSBDevice and its subclasses
	if (matchingDict == NULL) {
		fprintf(stderr, "IOServiceMatching returned NULL.\n");
//		return NULL;
	}

	// We are interested in all USB devices (as opposed to USB interfaces).  The Common Class Specification
	// tells us that we need to specify the idVendor, idProduct, and bcdDevice fields, or, if we're not interested
	// in particular bcdDevices, just the idVendor and idProduct.  Note that if we were trying to match an
	// IOUSBInterface, we would need to set more values in the matching dictionary (e.g. idVendor, idProduct,
	// bInterfaceNumber and bConfigurationValue.

	// Create a CFNumber for the idVendor and set the value in the dictionary
	numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usbVendor);
	CFDictionarySetValue(matchingDict,
						 CFSTR(kUSBVendorID),
						 numberRef);
	CFRelease(numberRef);

	// Create a CFNumber for the idProduct and set the value in the dictionary
	numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usbProduct);
	CFDictionarySetValue(matchingDict,
						 CFSTR(kUSBProductID),
						 numberRef);
	CFRelease(numberRef);
	numberRef = NULL;

	// Create a notification port and add its run loop event source to our run loop
	// This is how async notifications get set up.

	gNotifyPort = IONotificationPortCreate(kIOMainPortDefault);
	runLoopSource = IONotificationPortGetRunLoopSource(gNotifyPort);

	gRunLoop = CFRunLoopGetCurrent();
	CFRunLoopAddSource(gRunLoop, runLoopSource, kCFRunLoopDefaultMode);

	// Now set up a notification to be called when a device is first matched by I/O Kit.
	kr = IOServiceAddMatchingNotification(gNotifyPort,                  // notifyPort
										  kIOFirstMatchNotification,    // notificationType
										  matchingDict,                 // matching
										  NULL,                  // callback
										  NULL,                         // refCon
										  &gAddedIter                   // notification
	);

    RazerDevices allDevices = { .devices = NULL, .size = 0 };

	// unsigned int port;
	// kern_return_t port_ret = IOMainPort(0, &port);
	// debug("getAllRazerDevices :: ret = %d, port = 0x%04x", port_ret, port);

//    CFMutableDictionaryRef matchingDict;
//    matchingDict = IOServiceMatching(kIOUSBHostDeviceClassName);
//    if (matchingDict == NULL)
//    {
//		error("getAllRazerDevice :: matchingDict == null");
//        return allDevices;
//    }
//
//    io_iterator_t iter;
//	//kIOMasterPortDefault
//    kern_return_t kReturn =
//            IOServiceGetMatchingServices(kIOMainPortDefault, matchingDict, &iter);
//    if (kReturn != kIOReturnSuccess)
//    {
//		error("getAllRazerDevice :: kReturn != kIOReturnSuccess");
//        return allDevices;
//    }

    int array_size = 0;
    int array_index = -1;
    RazerDevice *razerDevices = malloc(array_size * sizeof(RazerDevice));

    io_service_t usbDevice;
    while ((usbDevice = IOIteratorNext(gAddedIter)))
    {
		io_name_t       deviceName;
		CFStringRef     deviceNameAsCFString;
		debug("loop : device = 0x%x", usbDevice);

		// Get the USB device's name.
		kr = IORegistryEntryGetName(usbDevice, deviceName);
		if (KERN_SUCCESS != kr) {
			deviceName[0] = '\0';
		}

		deviceNameAsCFString = CFStringCreateWithCString(kCFAllocatorDefault, deviceName,
														 kCFStringEncodingASCII);

		// Dump our data to stderr just to see what it looks like.
		fprintf(stderr, "DeviceAdded :: deviceName: ");
		CFShow(deviceNameAsCFString);

        IOCFPlugInInterface **plugInInterface = NULL;
        SInt32 score;

        kr = IOCreatePlugInInterfaceForService(
                usbDevice, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score);

        IOObjectRelease(usbDevice); // Not needed after plugin created
        if ((kr != kIOReturnSuccess) || plugInInterface == NULL)
        {
            warn("Unable to create plugin (0x%08x)", kr);
            continue;
        }

        IOUSBDeviceInterface **dev = NULL;
        HRESULT hResult = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID *)&dev);

        (*plugInInterface)->Release(plugInInterface); // Not needed after device interface created
        if (hResult || !dev)
        {
            warn("Couldn’t create a device interface (0x%08x)", (int) hResult);
            continue;
        }

        // Filter out non-Razer devices
        if (!is_razer_device(dev))
        {
			warn("Not Razer Device");
            (*dev)->Release(dev);
            continue;
        }

        if(!is_keyboard(dev)
        && !is_blade_laptop(dev)
        && !is_mouse(dev)
        && !is_mouse_dock(dev)
        && !is_mouse_dock(dev)
        && !is_mouse_mat(dev)
        && !is_headphone(dev)
        && !is_egpu(dev)
        && !is_accessory(dev)
        ) {
			warn("unknown device");
            (*dev)->Release(dev);
            continue;
        }

        kr = (*dev)->USBDeviceOpen(dev);
        if (kr != kIOReturnSuccess)
        {
            printf("Unable to open USB device: %08x\n", kr);
            (*dev)->Release(dev);
            continue;
        }

        // Success. We found the Razer USB device.
        // Caller is responsible for closing USB and release device.
        array_size++;
        razerDevices = realloc(razerDevices, array_size * sizeof(RazerDevice));
        array_index++;

        UInt16 productId;
        (*dev)->GetDeviceProduct(dev, &productId);

        RazerDevice newDevice = { .usbDevice = dev, .internalDeviceId = array_index, .productId = productId };
        razerDevices[array_index] = newDevice;
		debug("loop : device added");
    }

    IOObjectRelease(gAddedIter);
    allDevices.devices = razerDevices;
    allDevices.size = array_size;
	debug("done.");
    return allDevices;
}

void closeAllRazerDevices(RazerDevices devices)
{
    for(int counter=0; counter < devices.size; ++counter) {
        IOUSBDeviceInterface **dev = devices.devices[counter].usbDevice;
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
    }
    free(devices.devices);
    devices.devices = NULL;
}
