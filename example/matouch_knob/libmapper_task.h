#ifndef __LIBMAPPER_TASK_H__
#define __LIBMAPPER_TASK_H__

#include <WiFi.h>
#include <WiFiClient.h>
#include <mapper_cpp.h>
#include <SimpleFOC.h>
#include <mapper/mapper_cpp.h>
#include "motor_task.h"
#include "display_task.h"
#include "libmapper_state.h"

extern volatile bool g_libmapper_network_ready;

mpr_dev dev = 0;
mpr_sig inputSignal = 0;
mpr_sig outputSignal_angle = 0;
mpr_sig inputSignal_torque = 0;
mpr_sig inputSignal_mode = 0;
float seqNumber = 0;
float receivedValue = 0;

float angleMin = 0.0f;
float angleMax = 3600.0f;

float torqueMin = -5.0f;
float torqueMax = 5.0f;

int modeMin = 0;
int modeMax = 1;

void inputSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
  receivedValue = *((float*)value);
  Serial.print("inputSignalHandler: ");
  Serial.println(receivedValue);
  
}

void torqueSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
    receivedValue = *((float*)value);
    Serial.print("torqueSignalHandler: ");
    Serial.println(receivedValue);

    update_libmapper_float(LibmapperField::torque, receivedValue);

}

void modeSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
    receivedValue = *((int*)value);
    Serial.print("modeSignalHandler: ");
    Serial.println(receivedValue);

    update_libmapper_bool(LibmapperField::IsLibmapperMode, (bool)receivedValue);

}

void define_device(){
    Serial.println("[LM] Creating mapper::Device...");

    dev = mpr_dev_new("ESP32", 0);

    outputSignal_angle = mpr_sig_new(dev, MPR_DIR_OUT, "Angle_Position", 1, MPR_FLT, "V",
                             &angleMin,  &angleMax, 0, 0, 0);
    inputSignal = mpr_sig_new(dev, MPR_DIR_IN, "valueReceived", 1, MPR_FLT, "V",
                            &angleMin,  &angleMax, 0, inputSignalHandler,
                            MPR_SIG_UPDATE);
    
    inputSignal_torque = mpr_sig_new(dev, MPR_DIR_IN, "Torque_value", 1, MPR_FLT, "V",
                            &torqueMin, &torqueMax, 0, torqueSignalHandler,
                            MPR_SIG_UPDATE);    
    inputSignal_mode = mpr_sig_new(dev, MPR_DIR_IN, "IsLibmapperMode", 1,  MPR_INT32, "V",
                            &modeMin, &modeMax, 0, modeSignalHandler,
                            MPR_SIG_UPDATE);
}

void libmapper_run(void* parameter) {

    Serial.println("[LM] libmapper task started");

    publish_libmapper({        
        .isLibmapperMode = false,
        .torque = 0.0f,
        .anglePosition = 0.0f,
        .network_ready = false,
    });
    bool ready = false;

    while (! ready) {
        Serial.println("[LM] Waiting for libmapper network ready...");
        LibmapperState state;

        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdTRUE)
        {
            ready = state.network_ready;
            
            Serial.print("[LM] State: ");
            Serial.println(ready);
        }
        
        vTaskDelay(300);
    }

    Serial.println("[LM] Network ready flag received");

    vTaskDelay(200);

    define_device();

    Serial.println("[LM] mapper::Device created");


    while (1) {
        mpr_dev_poll(dev, 0);
        
        LibmapperState state;

        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdTRUE)
        {
            float img_angle = state.anglePosition;
            
            mpr_sig_set_value(outputSignal_angle , 0, 1, MPR_FLT, &img_angle);
            
            Serial.print("[LM] Sent: ");
            Serial.println(img_angle);
        }
        vTaskDelay(200);
    }
}


#endif