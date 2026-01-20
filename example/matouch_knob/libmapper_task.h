#ifndef __LIBMAPPER_TASK_H__
#define __LIBMAPPER_TASK_H__

#include <WiFi.h>
#include <WiFiClient.h>
#include <mapper_cpp.h>
#include <SimpleFOC.h>
#include <mapper/mapper_cpp.h>
#include "motor_task.h"
#include "display_task.h"

extern volatile bool g_libmapper_network_ready;

mpr_dev dev = 0;
mpr_sig inputSignal = 0;
mpr_sig outputSignal = 0;
float seqNumber = 0;
float receivedValue = 0;
float signalMin = 0.0f;
float signalMax = 3600.0f;

void inputSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
  receivedValue = *((float*)value);
  Serial.print("inputSignalHandler: ");
  Serial.println(receivedValue);
  
}

void libmapper_run(void* parameter) {

    Serial.println("[LM] libmapper task started");


    while (! g_libmapper_network_ready) {
        Serial.println("[LM] Waiting for libmapper network ready...");
        vTaskDelay(300);
    }

    Serial.println("[LM] Network ready flag received");

    vTaskDelay(200);

    Serial.println("[LM] Creating mapper::Device...");
    float signalMin = 0.0f;
    float signalMax = 3600.0f;

    dev = mpr_dev_new("ESP32", 0);
    outputSignal = mpr_sig_new(dev, MPR_DIR_OUT, "valueToSend", 1, MPR_FLT, "V",
                             &signalMin, &signalMax, 0, 0, 0);
    inputSignal = mpr_sig_new(dev, MPR_DIR_IN, "valueReceived", 1, MPR_FLT, "V",
                            &signalMin, &signalMax, 0, inputSignalHandler,
                            MPR_SIG_UPDATE);
    Serial.println("[LM] mapper::Device created");


    while (1) {
        mpr_dev_poll(dev, 0);


  // Print received value
        Serial.print("[LM] received: ");
        Serial.println(receivedValue);

        
        KnobState state;

        if (xQueueReceive(knob_state_queue_, &state, 0) == pdTRUE)
        {
            pthread_mutex_lock(&lvgl_mutex);
            float adjusted_sub_position, raw_angle, adjusted_angle, img_angle;

            adjusted_sub_position = state.sub_position_unit * state.config.position_width_radians;
            raw_angle = state.current_position * state.config.position_width_radians;
            adjusted_angle = -(raw_angle + adjusted_sub_position);
            if (adjusted_angle > 0)
                img_angle = (uint16_t)(adjusted_angle * 573) % 3600;
            else
                img_angle = 3600 - (uint16_t)(abs(adjusted_angle) * 573) % 3600;

            
            mpr_sig_set_value(outputSignal, 0, 1, MPR_FLT, &img_angle);
            
            Serial.print("[LM] Sent: ");
            Serial.println(img_angle);

            pthread_mutex_unlock(&lvgl_mutex);

        }
        vTaskDelay(200);
    }
}


#endif