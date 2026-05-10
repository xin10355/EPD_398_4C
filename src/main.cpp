// GxEPD2_HelloWorld.ino by Jean-Marc Zingg
//
// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: https://www.good-display.com/companyfile/32/
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics (closed, read only):
// Good Display ePaper for Arduino: https://forum.arduino.cc/t/good-display-epaper-for-arduino/419657
// Waveshare e-paper displays with SPI: https://forum.arduino.cc/t/waveshare-e-paper-displays-with-spi/467865
//
// Add new topics in https://forum.arduino.cc/c/using-arduino/displays/23 for new questions and issues

// see GxEPD2_wiring_examples.h for wiring suggestions and examples
// if you use a different wiring, you need to adapt the constructor parameters!

#include <esp_heap_caps.h>

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
//#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <Fonts/FreeSansOblique24pt7b.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

// or select the display constructor line in one of the following files (old style):
#include "GxEPD2_display_selection.h"
#include "GxEPD2_display_selection_added.h"

// alternately you can copy the constructor from GxEPD2_display_selection.h or GxEPD2_display_selection_added.h to here
// e.g. for Wemos D1 mini:
//GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0154D67

//2.66 GxEPD2_266c
//GxEPD2_BW<GxEPD2_260_M01, GxEPD2_260_M01::HEIGHT> display(GxEPD2_260_M01(/*CS=5*/ 10, /*DC=*/ 8, /*RST=*/ 7, /*BUSY=*/ 9));

//4.2 GxEPD2_420c
GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(/*CS=5*/ 10, /*DC=*/ 8, /*RST=*/ 7, /*BUSY=*/ 9));

//3.98  GxEPD2_768c_GDEM076F55
//GxEPD2_4C<GxEPD2_768c_GDEM076F55, GxEPD2_768c_GDEM076F55::HEIGHT> display(GxEPD2_768c_GDEM076F55(/*CS=5*/ 10, /*DC=*/ 8, /*RST=*/ 7, /*BUSY=*/ 9));

// for handling alternative SPI pins (ESP32, RP2040) see example GxEPD2_Example.ino

const char HelloWorld[] = "Hello World!";

void helloWorld()
{
  display.setRotation(0);
  display.setFont(&FreeSansOblique24pt7b);
  display.setTextColor(GxEPD_RED);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  }
  while (display.nextPage());
}

void drawDiagonalLine()
{
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    // 从左上角 (0,0) 到右下角 (width-1, height-1) 画一条黑色对角线
    //display.drawLine(0, 0, display.width() - 1, display.height() - 1, GxEPD_BLACK);

    // 通过循环绘制多条偏移的平行线来实现加粗效果
    int thickness = 10; // 这里设置你想要的粗细（像素）
    for (int i = 0; i < thickness; i++) {
      display.drawLine(0, i, display.width() - 1, display.height() - 1 + i, GxEPD_BLACK);
    }
  }
  while (display.nextPage());
}

void drawSolidRectangle()
{
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    
    // 绘制实心黑色长方形：x, y, width, height, color
    display.fillRect(0, 0, 768, 520, GxEPD_BLACK);
  }
  while (display.nextPage());
}

void drawTextInCircle()
{
  const char text[] = "ABCDE"; // 要显示的文字
  display.setRotation(0);
  display.setFont(&FreeSansOblique24pt7b);
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    
    // 屏幕中心坐标
    int16_t centerX = display.width() / 2;
    int16_t centerY = display.height() / 2;
    uint16_t radius = 100; // 圆形半径

    // 1. 绘制实心黑色圆
    display.fillCircle(centerX, centerY, radius, GxEPD_BLACK);

    // 2. 计算文字居中位置
    display.setTextColor(GxEPD_RED);
    int16_t tbx, tby; uint16_t tbw, tbh;
    display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
    
    // 居中坐标 = 中心点 - (文字大小的一半) - 内部偏移量
    display.setCursor(centerX - (tbw / 2) - tbx, centerY - (tbh / 2) - tby);
    display.print(text);
  }
  while (display.nextPage());
}


void checkPSRAM()
{
  delay(1000);
  // 1. 检测PSRAM是否存在
  if (psramFound()) {
    Serial.printf("✅ PSRAM 检测成功！\n");
    Serial.printf("📦 PSRAM 总容量：%d KB\n", ESP.getPsramSize() / 1024);
    Serial.printf("🆓 PSRAM 空闲容量：%d KB\n", ESP.getFreePsram() / 1024);
  } else {
    Serial.println("❌ 未检测到 PSRAM！");
  }
  // 2. 测试PSRAM内存分配（验证读写可用性）
  if (psramFound()) {
    // 分配1MB PSRAM内存（ps_malloc专门用于PSRAM分配）
    uint8_t *psram_buf = (uint8_t *)ps_malloc(1024 * 1024);  
    if (psram_buf != NULL) {
      Serial.println("✅ 成功分配 1MB PSRAM 内存");
      // 写入测试数据（验证PSRAM可写）
      memset(psram_buf, 0xAA, 1024 * 1024);  
      // 释放内存（避免内存泄漏）
      free(psram_buf);  
    } else {
      Serial.println("❌ PSRAM 内存分配失败");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // 检查 PSRAM 总大小和可用大小
  //checkPSRAM();
  
  // 初始化显示，使用适合 Waveshare 板的设置
  Serial.println("Initializing display...");
  //display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  // helloWorld();
  // drawDiagonalLine();
  // drawTopLeftEllipse();
  drawTextInCircle();
  display.hibernate();
}

void loop() {
  // 实时打印空闲PSRAM容量（监控内存使用）
  // if (psramFound()) {
  //   Serial.printf("🔹 实时空闲 PSRAM：%d KB\n", ESP.getFreePsram() / 1024);
  // } else {
  //   Serial.println("🔹 PSRAM 未启用");
  // }
  // delay(1000);
};
