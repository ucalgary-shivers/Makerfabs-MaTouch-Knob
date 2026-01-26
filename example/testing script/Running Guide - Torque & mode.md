# Running Guide — Torque & Mode

This guide explains how to apply torque and enable **libmapper mode** using the testing script.

## Steps

1. **Flash the firmware** onto the device.  
2. Open the **Settings** sub‑menu and connect to the ESP Wi‑Fi network.  
3. Navigate to the Wi‑Fi configuration page using the IP address shown in the knob UI (e.g., `AP: 192.168.x.x`).  
4. Enter the Wi‑Fi **SSID** and **password** in the top two text fields and submit.  
5. Confirm that the device successfully connects to the intended Wi‑Fi network.  
6. In **webmapper**, verify that the device named **ESP32** is discovered.  
7. Run `libmapper_device_sending.py` to set the **torque** and **is_libmapper** mode signals on the ESP32.  

### Signal Descriptions

- **is_libmapper**  
  Determines whether the knob motor is controlled by the firmware or by libmapper signals.  
  When `is_libmapper = 1`, the torque value from libmapper is applied to the motor.

- **torque**  
  Represents the voltage applied to the motor via `motor.move()`.  
  The valid range is **-5 to 5**, where positive values indicate **clockwise rotation**.

