//
//  razercommon.c
//  RazerBlade
//
//  Based on Linux razer-driver kernel drivers: https://github.com/terrycain/razer-drivers/blob/master/driver/razercommon.h
//
#include <stdio.h>
#include <string.h>

#include "../include/razercommon.h"

/**
 * Send USB control report to the keyboard
 * USUALLY index = 0x02
 * FIREFLY is 0
 */
IOReturn razer_send_control_msg(IOUSBDeviceInterface **dev, void const *data, uint report_index) {
    IOUSBDevRequest request;
    
    request.bRequest = HID_REQ_SET_REPORT; // 0x09
    request.bmRequestType = USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_OUT;
    request.wValue = 0x300;
    request.wIndex = report_index;
    request.wLength = RAZER_USB_REPORT_LEN;
    request.pData = (void*)data;
    
    return (*dev)->DeviceRequest(dev, &request);
}


/**
 * Get a response from the razer device
 *
 * Makes a request like normal, this must change a variable in the device as then we
 * tell it give us data and it gives us a report.
 *
 * Supported Devices:
 *   Razer Chroma
 *   Razer Mamba
 *   Razer BlackWidow Ultimate 2013*
 *   Razer Firefly*
 *
 * Request report is the report sent to the device specifing what response we want
 * Response report will get populated with a response
 *
 * Returns kIOReturnSuccess when successful
 */
IOReturn razer_get_usb_response(IOUSBDeviceInterface **dev, uint report_index, struct razer_report* request_report, uint response_index, struct razer_report* response_report, int wait_us) {
    IOReturn retval;
    char buffer[sizeof(struct razer_report)];
    
    // Send the request to the device.
    // TODO look to see if index needs to be different for the request and the response
    retval = razer_send_control_msg(dev, request_report, report_index);
    
    if(retval != kIOReturnSuccess) {
        printf("razer_send_control_msg failed, ret = 0x%x\n", retval);
        
        return retval;
    }

    usleep(wait_us);
    
    IOUSBDevRequest request;
    
    request.bRequest = HID_REQ_GET_REPORT;
    request.bmRequestType = USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_IN;
    request.wValue = 0x300;
    request.wIndex = report_index;
    request.wLength = RAZER_USB_REPORT_LEN;
    request.pData = buffer;
    
    retval = (*dev)->DeviceRequest(dev, &request);
    
    if(retval != kIOReturnSuccess) {
        printf("razer_get_usb_response failed\n");
        
        return retval;
    }
    
    memcpy(response_report, buffer, sizeof(struct razer_report));
    
    
    return retval;
}


/**
 * Get initialised razer report
 */
struct razer_report get_razer_report(unsigned char command_class, unsigned char command_id, unsigned char data_size) {
    struct razer_report new_report = get_empty_razer_report();
    
    new_report.status = 0x00;
    new_report.transaction_id.id = 0xFF;
    new_report.remaining_packets = 0x00;
    new_report.protocol_type = 0x00;
    new_report.command_class = command_class;
    new_report.command_id.id = command_id;
    new_report.data_size = data_size;
    
    return new_report;
}


/**
 * Get empty razer report
 */
struct razer_report get_empty_razer_report(void) {
    struct razer_report new_report = {0};
    memset(&new_report, 0, sizeof(struct razer_report));

    return new_report;
}


/**
 * Calculate the checksum for the usb message
 *
 * Checksum byte is stored in the 2nd last byte in the messages payload.
 * The checksum is generated by XORing all the bytes in the report starting
 * at byte number 2 (0 based) and ending at byte 88.
 */
unsigned char razer_calculate_crc(struct razer_report *report) {
    /*second to last byte of report is a simple checksum*/
    /*just xor all bytes up with overflow and you are done*/
    unsigned char crc = 0;
    unsigned char *_report = (unsigned char*)report;
    
    unsigned int i;
    for(i = 2; i < 88; i++)
    {
        crc ^= _report[i];
    }
    
    return crc;
}


/**
 * Clamp a value to a min,max
 */
unsigned char clamp_u8(unsigned char value, unsigned char min, unsigned char max) {
    if(value > max)
        return max;
    if(value < min)
        return min;
    return value;
}

unsigned short clamp_u16(unsigned short value, unsigned short min, unsigned short max) {
    if(value > max)
        return max;
    if(value < min)
        return min;
    return value;
}

IOReturn razer_send_control_msg_old_device(IOUSBDeviceInterface **dev, void const *data, uint report_value, uint report_index, uint report_size)
{
    IOUSBDevRequest request;

    request.bRequest = HID_REQ_SET_REPORT; // 0x09
    request.bmRequestType = USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_OUT; // 0x21
    request.wValue = report_value;
    request.wIndex = report_index;
    request.wLength = report_size;
    request.pData = (void*)data;

    return (*dev)->DeviceRequest(dev, &request);
}
