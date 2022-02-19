extern "C"
{
#include "razerdevice.h"
#include "razerkbd_driver.h"
#include "razermouse_driver.h"
#include "razermousedock_driver.h"
#include "razermousemat_driver.h"
#include "razerheadphone_driver.h"
}

#include <log.hpp>

RazerDevices devices;

/**
* Get all razer devices
*/
void initDevices() {
    devices = getAllRazerDevices();
    for (int i = 0; i < devices.size; ++i) {
        RazerDevice device = devices.devices[i];
        debug("Razer device :: id = %d, product id = %d", device.internalDeviceId, device.productId);
    }
    debug("get all device done.");
}

RazerDevice getRazerDeviceFor(const int deviceId) {
    for (int counter = 0; counter < devices.size; ++counter) {
        auto productId = devices.devices[counter].productId;
        debug("getRazerDeviceFor :: productId [%d]", productId);
        if (productId == deviceId) {
            return devices.devices[counter];
        }
    }
    error("getRazerDeviceFor :: error, unable to get device for 0x%04x", deviceId);
    return {};
}

void SetModeSpectrum(const int deviceId) {
    RazerDevice device = getRazerDeviceFor(deviceId);
    razer_attr_write_mode_spectrum(device.usbDevice, "1", 1);
}

void SetModeCustom(const int deviceId) {
    auto device = getRazerDeviceFor(deviceId);
    razer_attr_write_mode_custom(device.usbDevice, "0", 1);
}

void SetModeReactive(const int deviceId) {
    auto device = getRazerDeviceFor(deviceId);
    if ((*device.usbDevice) == nullptr) {
        debug("SetModeReactive :: error, getRazerDevice failed, deviceId = %d\n", device.productId);
    }
    razer_attr_write_mode_reactive(device.usbDevice, "1fff", 4);
}

void SetModeWave(const int deviceId) {
    auto device = getRazerDeviceFor(deviceId);
    if ((*device.usbDevice) == nullptr) {
        error("unable to get device for %d", deviceId);
    }

    razer_attr_write_mode_wave(device.usbDevice, "1", 0, 0x1F);
}

void SetCustomColor(const int deviceId) {
    auto len = (3 + 22 * 3) * 6;
    auto buffer = new unsigned char[len];
    int index = 0;
    for (int i = 0; i <= 5; ++i) {
        buffer[index++] = i;
        buffer[index++] = 0;
        buffer[index++] = 21;
        for (int j = 0; j < 22; j++) {
            if (i == 0 && (j == 6 || j == 9)) {
                // Sound
                buffer[index++] = 0xAF;
                buffer[index++] = 0x00;
                buffer[index++] = 0x0F;
            } else if (i == 0 && j >= 18 && j <= 20) {
                // Media Control
                buffer[index++] = 0x0F;
                buffer[index++] = 0xFF;
                buffer[index++] = 0x0F;
            } else if(i == 1 && j >= 1 && j <= 10) {
                // Numbers
                buffer[index++] = 0xFF;
                buffer[index++] = 0xFF;
                buffer[index++] = 0x00;
            } else if ((i == 2 && j == 2) || (i == 3 && j >= 1 && j <= 3)) {
                // WSAD
                buffer[index++] = 0x1F;
                buffer[index++] = 0xFF;
                buffer[index++] = 0x1F;
            } else if ((i == 4 && j == 15) || (i == 5 && j >= 14 && j <= 16)) {
                // UP\DOWN\LEFT\RIGHT
                buffer[index++] = 0xFF;
                buffer[index++] = 0x1F;
                buffer[index++] = 0x1F;
            } else if((i == 1 && j == 13) || (i == 2 && j == 14)) {
                // Delete and Backspace
                buffer[index++] = 0xFF;
                buffer[index++] = 0x00;
                buffer[index++] = 0x00;
            } else if(i == 3 && j == 13) {
                // Enter
                buffer[index++] = 0x00;
                buffer[index++] = 0xFF;
                buffer[index++] = 0x0F;
            } else if(i == 5 && j == 1) {
                // Windows
                buffer[index++] = 0x00;
                buffer[index++] = 0xFF;
                buffer[index++] = 0x2F;
            } else {
                // Other Keys
                buffer[index++] = 0x6F;
                buffer[index++] = 0x6F;
                buffer[index++] = 0x4F;
            }
        }
    }
    auto device = getRazerDeviceFor(deviceId);
    razer_attr_write_matrix_custom_frame(device.usbDevice, buffer, len);
}

int main(int argc, char ** argv) {
    initDevices();
    SetCustomColor(0x026b);
    return 0;
}
