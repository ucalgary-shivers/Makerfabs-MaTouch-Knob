# Running Guide — Detent

This guide explains how to apply and config detent using the testing script.

## Steps

1. **Flash the firmware** onto the device.  
2. Open the **Settings** sub‑menu and connect to the ESP Wi‑Fi network.  
3. Navigate to the Wi‑Fi configuration page using the IP address shown in the knob UI (e.g., `AP: 192.168.x.x`).  
4. Enter the Wi‑Fi **SSID** and **password** in the top two text fields and submit.  
5. Confirm that the device successfully connects to the intended Wi‑Fi network.  
6. In **webmapper**, verify that the device named **ESP32** is discovered.  
7. Run `libmapper_device_detend.py` to set the detend signals on the ESP32.  

## Signal Definitions

### **detent_num_positions**
- Defines the total number of detent positions.  
- Especially relevant when the detents range does **not** span the full 360° rotation.  
- When set to `0`, the knob uses the full circumference and automatically determines the number of detents based on `detent_width`.  
- Valid range: **0–360**.

### **detent_position**
- Specifies the initial detent index or starting position.  
- Valid range: **0–360**.

### **detent_width**
- Width of a single detent, measured in degrees.  
- Valid range: **0–360**.

### **detent_strength**
- Maximum voltage applied to the motor via `motor.move()` for each detent.  
- Valid range: **–5 to 5**.  
- Positive values apply force **against** the rotation direction.

### **detent_endstop_strength**
- Maximum voltage applied for endstops, used when the detent range does not cover the full circumference.  
- Valid range: **–5 to 5**.  
- Positive values apply force **against** the rotation direction.

### **detent_snap_point**
- Threshold (as a ratio of `detent_width`) beyond which detent force is no longer applied.  
- Typical detents use a snap point **greater than 1**, meaning the snap occurs slightly past the next detent.  
- Valid range: **0–2**.

---

## Example Configurations

### **Main Menu (4 detents)**
```cpp
{
    detent_num_positions = 0,
    detent_position = 0,
    detent_width = 90,
    detent_strength = 1,
    detent_endstop_strength = 1,
    detent_snap_point = 0.55,
}
```

### **Surface Dial (Upper Submenu)**
```cpp
{
    detent_num_positions = 0,
    detent_position = 0,
    detent_width = 3,
    detent_strength = 1,
    detent_endstop_strength = 1,
    detent_snap_point = 1.1,
}
```

### **Offset Meter (Left Submenu)**
```cpp
{
    detent_num_positions = 100,
    detent_position = 50,
    detent_width = 1,
    detent_strength = 1,
    detent_endstop_strength = 1,
    detent_snap_point = 1.1,
}
```
