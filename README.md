PRODUCT COUNTING SYSTEM
1. Introduction

A counting system using an infrared obstacle sensor to detect and count products on a conveyor belt, handling cases where products are stacked or stuck together. The system provides immediate feedback through an LCD display, LED, and a buzzer.

<img width="700" height="440" alt="Mô hình thực tế" src="https://github.com/user-attachments/assets/8ddf79b7-0ff8-4a0a-a451-4a2e2f038f70" />

 3. System Architecture
- Input: Three infrared obstacle Sensors.
- Control: ESP32 processes signals from the three sensors to increment the total count and calculate boxes (2 products per box).
- Output: 16x2 I2C LCD, LED indicator, and Buzzer for real-time status updates and notifications.
3. Key Features
- Real-time Counting: Accurately counts products as they pass through the detection zone of the infrared obstacle sensors.
- Immediate Feedback: Each completed box of products triggers both the Buzzer and LED simultaneously for confirmation.
- Visual Monitoring: Provides real-time product counts through an intuitive display on a 16x2 I2C LCD.
- Custom PCB: The system is integrated on custom-designed printed circuit boards, featuring separate power and control modules for enhanced operational stability.
