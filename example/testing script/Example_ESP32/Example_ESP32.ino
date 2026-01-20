#include <WiFi.h>
#include <mapper.h>

// For disabling power saving
#include "esp_wifi.h"

const char* ssid = "Galaxy";
const char* password = "1q2w3e4r";

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

void setup() {
  Serial.begin(115200);
  Serial.println("setup init");
  Serial.println("wifi init");
  WiFi.begin(ssid, password);
  Serial.println("wifi begin done");

  // Disable WiFi power save (huge latency improvements)
  esp_wifi_set_ps(WIFI_PS_NONE);

  Serial.println("wifi wait for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("wifi connected");

  dev = mpr_dev_new("ESP32", 0);
  outputSignal = mpr_sig_new(dev, MPR_DIR_OUT, "valueToSend", 1, MPR_FLT, "V",
                             &signalMin, &signalMax, 0, 0, 0);
  inputSignal = mpr_sig_new(dev, MPR_DIR_IN, "valueReceived", 1, MPR_FLT, "V",
                            &signalMin, &signalMax, 0, inputSignalHandler,
                            MPR_SIG_UPDATE);
}

void loop() {
  // Update libmapper device
  mpr_dev_poll(dev, 0);

  // Increment number and send
  seqNumber += 0.01f;
  if (seqNumber > signalMax) seqNumber = signalMin;
  mpr_sig_set_value(outputSignal, 0, 1, MPR_FLT, &seqNumber);
  Serial.print("sent: ");
  Serial.println(seqNumber);

  // Print received value
  Serial.print("received: ");
  Serial.println(receivedValue);

  // Wait 100 ms
  delay(100);
}

