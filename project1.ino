#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Khai báo LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Cảm biến hồng ngoại
#define sensor_on  23
#define sensor_middle 27
#define sensor_bottom 14

// Buzzer
#define buzzer 5

// LED
#define LED 4

// Biến đếm số lượng ô vuông & thùng
int count = 0;
int box_count = 0;

// Biến để kiểm tra thay đổi số lượng
int lastCount = -1, lastBoxCount = -1;

void setup() {
    Serial.begin(115200);
    lcd.init();
    //lcd.backlight();

    pinMode(sensor_on, INPUT_PULLUP);
    pinMode(sensor_middle, INPUT_PULLUP);
    pinMode(sensor_bottom, INPUT_PULLUP);

    pinMode(buzzer, OUTPUT);
    pinMode(LED, OUTPUT);

    // Hiển thị giá trị ban đầu lên LCD
    lcd.setCursor(0, 0);
    lcd.print("SP:0000");
    lcd.setCursor(0, 1);
    lcd.print("Thung:0000");
}

void loop() {
    demSanPham();
    capNhatHienThi();
    xuLyLED();
    xuLyBuzzer();
}

// Hàm đếm sản phẩm

void demSanPham() {
    static bool Counting = false; // xác định có đang đếm trường hợp dính liền không
    static bool isProductPassing = false; //dùng để kiểm tra sản phẩm có đang đi qua cảm biến dưới không
    static bool seen_middle = false; //ghi nhận nếu sản phẩm đi qua cảm biến giữa hoặc trên
    static bool seen_on = false;
    static unsigned long starttime = 0; //thời điểm bắt đầu sản phẩm đi qua cảm biến dưới
    // Đọc cảm biến
    bool bottom = digitalRead(sensor_bottom) == LOW;
    bool middle = digitalRead(sensor_middle) == LOW;
    bool on = digitalRead(sensor_on) == LOW;
    if (bottom) {
        if (!isProductPassing) {
            isProductPassing = true;
            Counting = true;
            starttime = millis();
            seen_middle = middle; //ghi nhận trạng thái của cảm biến giữa 
            seen_on = on; //ghi nhận trạng thái của cảm biến trên
        } else {
            // Sản phẩm vẫn đang đi qua -> cập nhật nếu middle/on có LOW thoáng qua
            if (middle) {
                seen_middle = true;
            } else {
                seen_middle =  false;
            };
            if (on) {
                seen_on = true;
            } else {
                seen_on = false;
            };
        }
    } else {
        if (isProductPassing) {
            isProductPassing = false;
            if (Counting) {
                unsigned long duration = millis() - starttime;
                // Ưu tiên sử dụng cảm biến nếu đã từng thấy middle/on
                if (seen_middle && seen_on) {
                    count += 3;
                } else if (seen_middle) {
                    count += 2;
                } else {
                    // Không thấy middle/on, dùng duration để ước lượng
                    if (duration > 345 && duration <= 500) {
                        count += 3;
                    } else if (duration > 215 && duration <= 345) {
                        count += 2;
                    } else {
                        count += 1;
                    }
                }
                Counting = false;
                seen_middle = false;
                seen_on = false;
            } else {
                // không đang theo dõi, không làm gì
            }
        } else {
            //Không làm gì vì không có sản phẩm đi qua
        }
    }
    // Cập nhật số lượng thùng
    box_count = count / 2;
}

// Hàm cập nhật hiển thị LCD
void capNhatHienThi() {
    if (count != lastCount || box_count != lastBoxCount) {
        Serial.print("SP: ");
        Serial.println(count);
        Serial.print("Thung: "); 
        Serial.println(box_count);

        lcd.setCursor(0, 0);
        lcd.print("SP:");
        lcd.print(count / 1000);
        lcd.print((count % 1000) / 100);
        lcd.print(((count % 1000) % 100) / 10);
        lcd.print(((count % 1000) % 100) % 10);

        lcd.setCursor(0, 1);
        lcd.print("Thung:");
        lcd.print(box_count / 1000);
        lcd.print((box_count % 1000) / 100);
        lcd.print(((box_count % 1000) % 100) / 10);
        lcd.print(((box_count % 1000) % 100) % 10);

        lastCount = count;
        lastBoxCount = box_count;
    }
    else {
        // Không thay đổi giá trị -> không cập nhật LCD
    }
}

// Hàm điều khiển LED
void xuLyLED() {
    static int lastBoxCountLED = 0;
    if (box_count > lastBoxCountLED) {
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        lastBoxCountLED = box_count;
    }
    else {
        // Không làm gì nếu không có thùng mới
    }
}

// Hàm điều khiển Buzzer
void xuLyBuzzer() {
    static int lastBoxCountBuzzer = 0;
    if (box_count > lastBoxCountBuzzer) {
        tone(buzzer, 2500);
        delay(500);
        noTone(buzzer);
        lastBoxCountBuzzer = box_count;
    }
    else {
        // Không làm gì nếu không có thùng mới
    }
}
