// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: https://www.good-display.com/companyfile/1519.html
// Panel: GDEM075F52 : https://www.good-display.com/product/546.html
// Controller: JD79665AA, no specs, see GDEM075F52 panel specs for command list subset
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_768c_GDEM076F55_H_
#define _GxEPD2_768c_GDEM076F55_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_768c_GDEM076F55 : public GxEPD2_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 768;
    static const uint16_t WIDTH_VISIBLE = WIDTH;
    static const uint16_t HEIGHT = 552;
    static const GxEPD2::Panel panel = GxEPD2::GDEM076F55;
    static const bool hasColor = true;
    static const bool hasPartialUpdate = true; // can be switched off
    static const bool hasFastPartialUpdate = false;
    static const bool useFastFullUpdate = false; // set false to avoid residues, set false for extended (low) temperature range
    static const uint16_t power_on_time = 150; // ms, e.g. 138000us
    static const uint16_t power_off_time = 100; // ms, e.g. 82000us
    static const uint16_t full_refresh_time = 21000; // ms, e.g. 20918000us
    static const uint16_t partial_refresh_time = 21000; // ms, e.g. 20918000us
    // constructor
    // 构造函数：初始化CS、DC、RST和BUSY引脚
    GxEPD2_768c_GDEM076F55(int16_t cs, int16_t dc, int16_t rst, int16_t busy);
    // methods (virtual)
    //  Support for Bitmaps (Sprites) to Controller Buffer and to Screen
    // 清除屏幕：初始化控制器内存和屏幕显示（默认白色）
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    // 清除屏幕：使用指定的黑色值和彩色值初始化控制器内存和屏幕
    void clearScreen(uint8_t black_value, uint8_t color_value); // init controller memory and screen
    // 写入屏幕缓冲区：仅初始化控制器内存，不刷新屏幕（默认白色）
    void writeScreenBuffer(uint8_t value = 0xFF); // init controller memory (default white)
    // 写入屏幕缓冲区：使用指定的黑色值和彩色值初始化控制器内存，不刷新屏幕
    void writeScreenBuffer(uint8_t black_value, uint8_t color_value); // init controller memory
    // write to controller memory, without screen refresh; x和w应该是8的倍数
    // 写入图像到控制器内存（单色位图），不刷新屏幕；x坐标和宽度应为8的倍数
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 写入图像的一部分到控制器内存（单色位图），不刷新屏幕；支持从源位图中提取指定区域
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 写入图像到控制器内存（黑白+彩色双图层），不刷新屏幕；支持黑白和彩色两个位图数据
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 写入图像的一部分到控制器内存（黑白+彩色双图层），不刷新屏幕；支持从源位图中提取指定区域
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, without screen refresh; x和w应该是8的倍数
    // 写入原生格式数据到控制器内存，不刷新屏幕；data1为4色打包格式数据（每像素2位）
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 写入原生格式数据的一部分到控制器内存，不刷新屏幕；支持从源数据中提取指定区域
    void writeNativePart(const uint8_t* data1, const uint8_t* data2, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write to controller memory, with screen refresh; x和w应该是8的倍数
    // 绘制图像到屏幕（单色位图），写入控制器内存并刷新指定区域；x坐标和宽度应为8的倍数
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 绘制图像的一部分到屏幕（单色位图），写入控制器内存并刷新指定区域；支持从源位图中提取指定区域
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 绘制图像到屏幕（黑白+彩色双图层），写入控制器内存并刷新指定区域；支持黑白和彩色两个位图数据
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 绘制图像的一部分到屏幕（黑白+彩色双图层），写入控制器内存并刷新指定区域；支持从源位图中提取指定区域
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // write sprite of native data to controller memory, with screen refresh; x和w应该是8的倍数
    // 绘制原生格式数据到屏幕，写入控制器内存并刷新指定区域；data1为4色打包格式数据（每像素2位）
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    // 刷新屏幕：从控制器内存刷新到全屏，可选择局部更新模式
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    // 刷新屏幕：从控制器内存刷新指定矩形区域，自动计算与屏幕的交集
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, partial screen
    // 关闭电源：关闭面板驱动电压生成，避免屏幕随时间褪色
    void powerOff(); // turns off generation of panel driving voltages, avoids screen fading over time
    // 休眠：关闭电源并设置控制器进入深度睡眠模式以最小化功耗，仅在rst>=0时可通过RST唤醒
    void hibernate(); // turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
    // 设置分页模式：用于GxEPD2_154c的分页工作around，也适用于4色屏在hasPartialUpdate=false时使用
    void setPaged(); // for GxEPD2_154c paged workaround, also for 4-color for use with hasPartialUpdate = false
  private:
    // 内部刷新方法：执行实际的屏幕刷新操作，根据partial_update_mode参数决定使用局部或全局刷新
    void _refresh(bool partial_update_mode);
    // 设置局部RAM区域：配置控制器的部分刷新区域参数，包括起始坐标和宽高
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial_mode = true);
    // 上电：开启面板电源，等待电源稳定
    void _PowerOn();
    // 断电：关闭面板电源，等待电源完全关闭
    void _PowerOff();
    // 初始化显示：配置控制器寄存器，设置分辨率、时序参数等，完成显示屏初始化
    void _InitDisplay();
    // 特殊反转变换：对4色数据进行特殊的位重排反转操作，用于demo位图的特殊格式
    uint8_t _invert(uint8_t data); // special inversion for demo bitmap
    // 选择快速全屏更新模式：启用或禁用快速更新功能，切换时会重新初始化显示
    void selectFastFullUpdate(bool);
  private:
    bool _paged;
    bool _use_fast_update;
};

#endif
