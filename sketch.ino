#include <Adafruit_GFX.h>          // مكتبة الرسومات الأساسية
#include <Adafruit_ILI9341.h>      // مكتبة شاشة ILI9341
#include <SPI.h>                   // مكتبة بروتوكول SPI
#include <DHT.h>

// تعريف دبابيس الاتصال مع الشاشة
#define TFT_CS   5   // Chip Select
#define TFT_RST  4   // Reset
#define TFT_DC   2   // Data/Command

// إنشاء كائن الشاشة باستخدام الدبابيس المعرفة
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// إعدادات الجدول
int rowNumber = 0;
const int startY = 40;
const int rowHeight = 20;
const int maxRows = 10;

// LED للتحذير
#define LED_RED 12
#define LED_GREEN 14



void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);  

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
// إطفاء الـ LEDs عند البداية
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);


  // شاشة بيضاء
  tft.fillScreen(ILI9341_WHITE);

  // عنوان ورؤوس الجدول
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.println("📋 Manual Temp Input");

  tft.setCursor(0, 20);
  tft.println("No.  Temp(C)  Status");

}

void loop() {
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    input.trim();


    // إذا كتب المستخدم "clear"
    if (input.equalsIgnoreCase("clear")) {
      rowNumber = 0;
      tft.fillScreen(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_BLACK);
      tft.setCursor(0, 0);
      tft.println("📋 Manual Temp Input");
      tft.setCursor(0, 20);
      tft.println("No.  Temp(C)  Status");
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);

      return;
    }

    // تحويل النص إلى رقم
    float temp = input.toFloat();

    // إعادة الجدول إذا امتلأ
    if (rowNumber >= maxRows) {
      rowNumber = 0;
      tft.fillScreen(ILI9341_WHITE);
      tft.setTextColor(ILI9341_BLACK);
      tft.setCursor(0, 0);
      tft.println("📋 Manual Temp Input");
      tft.setCursor(0, 20);
      tft.println("No.  Temp(C)  Status");
    }

     int y = startY + rowNumber * rowHeight;


     // تحديد لون النص وحالة الـ LEDs
    if (temp > 35.0) {
      tft.setTextColor(ILI9341_RED);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, LOW);
    } else {
      tft.setTextColor(ILI9341_GREEN);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
    }

    // طباعة الصف الجديد في الجدول
    tft.setCursor(0, y);
    tft.print(rowNumber + 1);
    tft.print("    ");
    tft.print(temp, 1);
    tft.print("     ");
    if (temp > 35.0) {
      tft.println("HIGH");
    } else {
      tft.println("OK");
    }

    rowNumber++;

  }
}

