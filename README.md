

# 🌡️ 8051 Temperature Monitoring System

This project is a complete embedded temperature monitoring system built using the 8051 microcontroller. It reads temperature using the LM35 sensor with ADC0804, displays information on an LCD, and communicates with a PC via UART. It also includes LED indicators, buzzer alerts, button-based mode selection, and stores historical data.

---

## 🔧 Features

- ✅ **Live Temperature Display** on 16x2 LCD
- ✅ **Three Modes of Operation** (Temperature, ADC Value, Max/Min)
- ✅ **LED & Buzzer Alerts** based on temperature levels
- ✅ **Serial Communication** via UART (for real-time PC output)
- ✅ **Temperature History Logging** (up to 24 recent values)
- ✅ **Debounced Button Input** for smooth mode switching

---

## 📌 Hardware Used

- AT89C51 Microcontroller (8051 family)
- LM35 Temperature Sensor
- ADC0804 Analog to Digital Converter
- 16x2 LCD Display
- UART for Serial Communication
- Push Buttons for mode selection
- LEDs (Red, Green, Blue) + Buzzer for status indication

---

## 📂 Mode Descriptions

| Mode | Triggered by                  | Display on LCD             | UART Output         |
|------|-------------------------------|-----------------------------|---------------------|
| 1    | Temp switch pressed           | Current temperature in °C   | Temp, Max, Min      |
| 2    | ADC switch pressed            | Raw ADC value               | -                   |
| 3    | MaxMin switch pressed         | Max and Min temperature     | Full history log    |
| 0    | No switch pressed             | "No button pressed"         | -                   |

---

## 📈 LED + Buzzer Indication

| Temperature Range | LED Color | Buzzer |
|-------------------|-----------|--------|
| > 35°C            | 🔴 Red    | ON     |
| 25°C – 35°C       | 🟢 Green  | OFF    |
| < 25°C            | 🔵 Blue   | OFF    |

---

## 🖥️ UART Serial Output

- Current, Max, and Min temperature in °C
- Temperature history (when Mode 3 is selected)
- Data sent in human-readable format for easy serial monitoring

---

## 📌 Code Highlights

- Memory-efficient and clean C code using Keil C for 8051
- All functionality inside `main()` loop with efficient flag checking
- Temperature change threshold avoids unnecessary UART flooding
- Supports 24-slot circular buffer for historical temperature logs

---

## 📷 Demo / Output Screenshots

> (You can add photos of your circuit setup or serial output here)

---

## 🛠️ How to Run

1. Flash the `.hex` file to your AT89C51 microcontroller
2. Connect LM35 to ADC0804 properly (analog in)
3. Wire up LCD (16x2), LEDs, buzzer, and buttons as per code
4. Open serial monitor at 9600 baud rate to observe output
5. Press switches to change modes

---

## 🤝 Contributions & Licensing

Feel free to fork this project or suggest improvements. This code is for educational purposes and open-source hobby use. Attribution appreciated!

---



