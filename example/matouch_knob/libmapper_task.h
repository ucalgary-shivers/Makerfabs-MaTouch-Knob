#ifndef __LIBMAPPER_TASK_H__
#define __LIBMAPPER_TASK_H__

#include <WiFi.h>
#include <WiFiClient.h>
#include <mapper_cpp.h>
#include <SimpleFOC.h>
#include <mapper/mapper_cpp.h>

//static mapper::Device device("test");

void init_libmapper() {
    // device is already constructed above
}

/*
void update_libmapper() {
    Serial.println(device.ready());

    if (device.ready()) {
        device.poll(100);
    }
}
*/
bool wifi_is_ready() {
    // STA mode ready
    if (WiFi.getMode() == WIFI_STA && WiFi.status() == WL_CONNECTED) {
        return true;
    }

    // AP mode ready
    if (WiFi.getMode() == WIFI_AP && WiFi.softAPIP() != IPAddress(0,0,0,0)) {
        return true;
    }

    return false;
}


void libmapper_run(void* parameter) {
    while (!wifi_is_ready()) {
        Serial.println("Waiting for WiFi stack...");
        vTaskDelay(500);
    }


    mapper::Device dev("testing");

    while (1) {
        Serial.println(dev.ready());
        if (dev.ready()) {
            dev.poll(100);
        }
        vTaskDelay(1);
    }
}

#endif