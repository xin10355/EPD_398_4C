// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: https://www.good-display.com/companyfile/32/
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifndef _GxEPD2_4C_H_
#define _GxEPD2_4C_H_
// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>

#ifndef ENABLE_GxEPD2_GFX
// default is off
#define ENABLE_GxEPD2_GFX 0
#endif

#if ENABLE_GxEPD2_GFX
#include "GxEPD2_GFX.h"
#define GxEPD2_GFX_BASE_CLASS GxEPD2_GFX
#elif defined(_GFX_H_)
#define GxEPD2_GFX_BASE_CLASS GFX
#else
#include <Adafruit_GFX.h>
#define GxEPD2_GFX_BASE_CLASS Adafruit_GFX
#endif

#include "GxEPD2_EPD.h"

// for __has_include see https://en.cppreference.com/w/cpp/preprocessor/include
// see also https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
// #if !defined(__has_include) || __has_include("epd/GxEPD2_102.h") is not portable!

#if defined __has_include
#  if __has_include("GxEPD2.h")
#    // __has_include can be used
#  else
#    // __has_include doesn't work for us, include anyway
#    undef __has_include
#    define __has_include(x) true
#  endif
#else
#  // no __has_include, include anyway
#  define __has_include(x) true
#endif

#if __has_include("epd4c/GxEPD2_154c_GDEM0154F51H.h")
#include "epd4c/GxEPD2_154c_GDEM0154F51H.h"
#endif
#if __has_include("epd4c/GxEPD2_213c_GDEY0213F51.h")
#include "epd4c/GxEPD2_213c_GDEY0213F51.h"
#endif
#if __has_include("epd4c/GxEPD2_266c_GDEY0266F51H.h")
#include "epd4c/GxEPD2_266c_GDEY0266F51H.h"
#endif
#if __has_include("epd4c/GxEPD2_290c_GDEY029F51H.h")
#include "epd4c/GxEPD2_290c_GDEY029F51H.h"
#endif
#if __has_include("epd4c/GxEPD2_300c.h")
#include "epd4c/GxEPD2_300c.h"
#endif
#if __has_include("epd4c/GxEPD2_350c_GDEM035F51.h")
#include "epd4c/GxEPD2_350c_GDEM035F51.h"
#endif
#if __has_include("epd4c/GxEPD2_397c_GDEM0397F81.h")
#include "epd4c/GxEPD2_397c_GDEM0397F81.h"
#endif
#if __has_include("epd4c/GxEPD2_420c_GDEY0420F51.h")
#include "epd4c/GxEPD2_420c_GDEY0420F51.h"
#endif
#if __has_include("epd4c/GxEPD2_437c.h")
#include "epd4c/GxEPD2_437c.h"
#endif
#if __has_include("epd4c/GxEPD2_579c_GDEY0579F51.h")
#include "epd4c/GxEPD2_579c_GDEY0579F51.h"
#endif
#if __has_include("epd4c/GxEPD2_750c_GDEM075F52.h")
#include "epd4c/GxEPD2_750c_GDEM075F52.h"
#endif
#if __has_include("epd4c/GxEPD2_768c_GDEM076F55.h")
#include "epd4c/GxEPD2_768c_GDEM076F55.h"
#endif
#if __has_include("epd4c/GxEPD2_1160c_GDEY116F51.h")
#include "epd4c/GxEPD2_1160c_GDEY116F51.h"
#endif

template<typename GxEPD2_Type, const uint16_t page_height>
class GxEPD2_4C : public GxEPD2_GFX_BASE_CLASS
{
  public:
    GxEPD2_Type epd2;
    // 构造函数：初始化GxEPD2_4C实例并设置默认参数
#if ENABLE_GxEPD2_GFX
    GxEPD2_4C(GxEPD2_Type epd2_instance) : GxEPD2_GFX_BASE_CLASS(epd2, GxEPD2_Type::WIDTH_VISIBLE, GxEPD2_Type::HEIGHT), epd2(epd2_instance)
#else
    GxEPD2_4C(GxEPD2_Type epd2_instance) : GxEPD2_GFX_BASE_CLASS(GxEPD2_Type::WIDTH_VISIBLE, GxEPD2_Type::HEIGHT), epd2(epd2_instance)
#endif
    {
      _page_height = page_height;
      _pages = (HEIGHT / _page_height) + ((HEIGHT % _page_height) > 0);
      _mirror = false;
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    // 获取总页数
    uint16_t pages()
    {
      return _pages;
    }

    // 获取页面高度
    uint16_t pageHeight()
    {
      return _page_height;
    }

    // 设置镜像模式
    bool mirror(bool m)
    {
      _swap_ (_mirror, m);
      return m;
    }

    // 绘制单个像素点，根据坐标和颜色值
    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      //合法性检查：首先确认传入的坐标是否在当前逻辑宽度和高度范围内。
      if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;
      //水平镜像：如果开启了镜像模式 (_mirror)，则对 x 坐标进行翻转。
      if (_mirror) x = width() - x - 1;

      //旋转处理 (Rotation)
      switch (getRotation())
      {
        case 1: // 旋转90度
          _swap_(x, y);
          x = WIDTH - x - 1;
          break;
        case 2: // 旋转180度
          x = WIDTH - x - 1;
          y = HEIGHT - y - 1;
          break;
        case 3: // 旋转270度
          _swap_(x, y);
          y = HEIGHT - y - 1;
          break;
      }

      // 如果正在使用局部更新（Partial Update），坐标需要相对于局部窗口的起点 (_pw_x, _pw_y) 进行偏移。
      // transpose partial window to 0,0
      x -= _pw_x;
      y -= _pw_y;
      // 再次检查偏移后的坐标是否落在定义的局部窗口范围内，不在范围内则直接返回。
      if ((x < 0) || (x >= int16_t(_pw_w)) || (y < 0) || (y >= int16_t(_pw_h))) return;

      // 分页渲染逻辑 (Paged Rendering)
      // adjust for current page
      y -= _current_page * _page_height;
      // 函数检查当前像素的物理 y 轴坐标是否属于当前正在处理的这一页 (_current_page)。如果该点不在当前页的纵向范围内，则丢弃该绘制指令。
      if ((y < 0) || (y >= int16_t(_page_height))) return;

      /**
        缓冲区定位：
        该类支持 4 色，意味着每个像素需要 2 位 (bits) 来表示（$2^2 = 4$ 种颜色）。
        因此，一个字节 (8 bits) 可以存储 4 个像素。
        x / 4 确定该像素在哪一个字节。
        y * (_pw_w / 4) 确定该像素在缓冲区的哪一行。
      */
      uint32_t i = x / 4 + uint32_t(y) * (_pw_w / 4);

      // 颜色转换：调用 color4(color) 将 16 位的 RGB565 颜色值映射为 0-3 之间的 2 位索引值。
      uint8_t pv = color4(color);

      /**
       位操作写入缓冲区 (Bitwise Manipulation)
       根据 x % 4 的余数，确定该像素在字节内的具体位置，并使用位掩码（Mask）进行更新：
        case 0: 对应字节最高 2 位 (bits 7, 6)。掩码 0x3F (00111111) 清除旧数据。
        case 1: 对应 bits 5, 4。掩码 0xCF (11001111)。
        case 2: 对应 bits 3, 2。掩码 0xF3 (11110011)。
        case 3: 对应最低 2 位 (bits 1, 0)。掩码 0xFC (11111100)。
       */
      switch(x % 4)
      {
        case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F) | (pv << 6); break;
        case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF) | (pv << 4); break;
        case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3) | (pv << 2); break;
        case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC) | pv; break;
      }
    }

    // 初始化显示驱动，串口诊断比特率为0表示禁用
    void init(uint32_t serial_diag_bitrate = 0) // = 0 : disabled
    {
      epd2.init(serial_diag_bitrate);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    // 带额外参数的初始化方法：
    // initial false 表示处理器深度睡眠唤醒后重新初始化，如果保持显示屏电源供应
    // 仅与具有快速局部更新的黑白显示屏相关
    // reset_duration = 20 是默认值；值为2可能有助于解决来自Waveshare的新板子上的"智能"复位电路问题
    // pulldown_rst_mode true 表示使用替代RST处理方式以避免通过RST引脚馈送5V电压
    // 使用额外参数初始化显示驱动器
    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration = 20, bool pulldown_rst_mode = false)
    {
      epd2.init(serial_diag_bitrate, initial, reset_duration, pulldown_rst_mode);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    // 带额外参数的初始化方法：
    // SPIClass& spi: SPI或备用硬件SPI通道
    // SPISettings spi_settings: 如用于更高SPI速度选择
    // 使用SPI参数初始化显示驱动器
    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration, bool pulldown_rst_mode, SPIClass& spi, SPISettings spi_settings)
    {
      epd2.selectSPI(spi, spi_settings);
      epd2.init(serial_diag_bitrate, initial, reset_duration, pulldown_rst_mode);
      _using_partial_mode = false;
      _current_page = 0;
      setFullWindow();
    }

    // 释放SPI和控制引脚
    void end() 
    {
      epd2.end();
    }

    // 用指定颜色填充整个屏幕
    void fillScreen(uint16_t color)
    {
      uint8_t pv = color4(color) * 0x55; // 0b01010101
      for (uint32_t x = 0; x < sizeof(_pixel_buffer); x++)
      {
        _pixel_buffer[x] = pv;
      }
    }

    // 将缓冲区内容显示到屏幕，适用于全屏缓冲区
    void display(bool partial_update_mode = false)
    {
      epd2.writeNative(_pixel_buffer, 0, 0, 0, GxEPD2_Type::WIDTH, _page_height);
      epd2.refresh(partial_update_mode);
      if (!partial_update_mode) epd2.powerOff();
    }

    // 将部分缓冲区内容显示到屏幕，适用于全屏缓冲区
    // displayWindow，使用符合实际旋转方向的参数
    // 对于旋转0或2，x和w应该是8的倍数
    // 对于旋转1或3，y和h应该是8的倍数
    // 否则窗口将按需增加
    // 这是电子纸控制器的寻址限制
    // 显示指定窗口区域的内容到屏幕
    void displayWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      x = gx_uint16_min(x, width());
      y = gx_uint16_min(y, height());
      w = gx_uint16_min(w, width() - x);
      h = gx_uint16_min(h, height() - y);
      _rotate(x, y, w, h);
      epd2.writeNativePart(_pixel_buffer, 0, x, y, GxEPD2_Type::WIDTH, _page_height, x, y, w, h);
      epd2.refresh(x, y, w, h);
    }

    // 设置全屏窗口
    void setFullWindow()
    {
      _using_partial_mode = false;
      _pw_x = 0;
      _pw_y = 0;
      _pw_w = GxEPD2_Type::WIDTH;
      _pw_h = HEIGHT;
    }

    // setPartialWindow，使用符合实际旋转方向的参数
    // 对于旋转0或2，x和w应该是8的倍数
    // 对于旋转1或3，y和h应该是8的倍数
    // 否则窗口将按需增加
    // 这是电子纸控制器的寻址限制
    // 设置部分窗口，用于局部刷新
    void setPartialWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
      if (!epd2.hasPartialUpdate) return;
      _pw_x = gx_uint16_min(x, width());
      _pw_y = gx_uint16_min(y, height());
      _pw_w = gx_uint16_min(w, width() - _pw_x);
      _pw_h = gx_uint16_min(h, height() - _pw_y);
      _rotate(_pw_x, _pw_y, _pw_w, _pw_h);
      _using_partial_mode = true;
      // make _pw_x, _pw_w multiple of 4
      _pw_w += _pw_x % 4;
      if (_pw_w % 4 > 0) _pw_w += 4 - _pw_w % 4;
      _pw_x -= _pw_x % 4;
    }

    // 开始第一页的绘制
    void firstPage()
    {
      fillScreen(GxEPD_WHITE);
      _current_page = 0;
      _second_phase = false;
      epd2.setPaged(); // for GxEPD2_565c paged workaround
    }

    // 翻到下一页，返回是否还有更多页需要绘制
    bool nextPage()
    {
      uint16_t page_ys = _current_page * _page_height;
      if (_using_partial_mode)
      {
        //Serial.print("  nextPage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(_pw_y); Serial.print(", ");
        //Serial.print(_pw_w); Serial.print(", "); Serial.print(_pw_h); Serial.print(") P"); Serial.println(_current_page);
        uint16_t page_ye = _current_page < int16_t(_pages - 1) ? page_ys + _page_height : HEIGHT;
        uint16_t dest_ys = _pw_y + page_ys; // transposed
        uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
        if (dest_ye > dest_ys)
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.println(")");
          epd2.writeNative(_pixel_buffer, 0, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
        }
        else
        {
          //Serial.print("writeImage("); Serial.print(_pw_x); Serial.print(", "); Serial.print(dest_ys); Serial.print(", ");
          //Serial.print(_pw_w); Serial.print(", "); Serial.print(dest_ye - dest_ys); Serial.print(") skipped ");
          //Serial.print(dest_ys); Serial.print(".."); Serial.println(dest_ye);
        }
        _current_page++;
        if (_current_page == int16_t(_pages))
        {
          _current_page = 0;
          if (!_second_phase)
          {
            epd2.refresh(_pw_x, _pw_y, _pw_w, _pw_h);
            if (epd2.hasFastPartialUpdate)
            {
              _second_phase = true;
              return true;
            }
          }
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
      else // full update
      {
        epd2.writeNative(_pixel_buffer, 0, 0, page_ys, GxEPD2_Type::WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        _current_page++;
        if (_current_page == int16_t(_pages))
        {
          _current_page = 0;
          if ((epd2.panel == GxEPD2::GDEW0154Z04) && (_pages > 1))
          {
            if (!_second_phase)
            {
              epd2.refresh(false); // full update after first phase
              _second_phase = true;
              fillScreen(GxEPD_WHITE);
              return true;
            }
            else epd2.refresh(true); // partial update after second phase
          } else epd2.refresh(false); // full update after only phase
          epd2.powerOff();
          return false;
        }
        fillScreen(GxEPD_WHITE);
        return true;
      }
    }

    // 返回前一页的处理结果（此方法在此类中不执行任何操作）
    bool nextPageToPrevious() {return false;}; // no-op in this class

    // GxEPD风格的分页绘制；drawCallback()会被按需调用多次
    void drawPaged(void (*drawCallback)(const void*), const void* pv)
    {
      if (_using_partial_mode)
      {
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          uint16_t page_ye = _current_page < (_pages - 1) ? page_ys + _page_height : HEIGHT;
          uint16_t dest_ys = _pw_y + page_ys; // transposed
          uint16_t dest_ye = gx_uint16_min(_pw_y + _pw_h, _pw_y + page_ye);
          if (dest_ye > dest_ys)
          {
            fillScreen(GxEPD_WHITE);
            drawCallback(pv);
            epd2.writeNative(_pixel_buffer, 0, _pw_x, dest_ys, _pw_w, dest_ye - dest_ys);
          }
        }
        epd2.refresh(_pw_x, _pw_y, _pw_w, _pw_h);
      }
      else // full update
      {
        epd2.setPaged(); // for GxEPD2_154c paged workaround
        for (_current_page = 0; _current_page < _pages; _current_page++)
        {
          uint16_t page_ys = _current_page * _page_height;
          fillScreen(GxEPD_WHITE);
          drawCallback(pv);
          epd2.writeNative(_pixel_buffer, 0, 0, page_ys, GxEPD2_Type::WIDTH, gx_uint16_min(_page_height, HEIGHT - page_ys));
        }
        epd2.refresh(false); // full update
        epd2.powerOff();
      }
      _current_page = 0;
    }

    // 绘制反色位图
    void drawInvertedBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      // taken from Adafruit_GFX.cpp, modified
      int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
      uint8_t byte = 0;
      for (int16_t j = 0; j < h; j++)
      {
        for (int16_t i = 0; i < w; i++ )
        {
          if (i & 7) byte <<= 1;
          else
          {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
            byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
#else
            byte = bitmap[j * byteWidth + i / 8];
#endif
          }
          if (!(byte & 0x80))
          {
            drawPixel(x + i, y + j, color);
          }
        }
      }
    }

    //  支持位图（精灵）到控制器缓冲区和屏幕
    // 清除屏幕，将控制器内存和屏幕初始化为指定值（默认白色）
    void clearScreen(uint8_t value = 0xFF) // init controller memory and screen (default white)
    {
      epd2.clearScreen(value);
    }
    // 写入屏幕缓冲区，将控制器内存初始化为指定值（默认白色）
    void writeScreenBuffer(uint8_t value = 0xFF) // init controller memory (default white)
    {
      epd2.writeScreenBuffer(value);
    }
    // 写入控制器内存，无屏幕刷新；x和w应该是8的倍数
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.writeImage(black, color, x, y, w, h, false, false, false);
    }
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, false, false, false);
    }
    // 写入本地数据精灵到控制器内存，无屏幕刷新；x和w应该是8的倍数
    void writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
    }
    // 写入控制器内存，带屏幕刷新；x和w应该是8的倍数
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.drawImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false)
    {
      epd2.drawImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawImage(black, color, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.drawImage(black, color, x, y, w, h, false, false, false);
    }
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
    }
    void drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                       int16_t x, int16_t y, int16_t w, int16_t h)
    {
      epd2.drawImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, false, false, false);
    }
    // 写入本地数据精灵到控制器内存，带屏幕刷新；x和w应该是8的倍数
    void drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
    {
      epd2.drawNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
    }
    // 屏幕刷新从控制器内存到全屏，默认为非局部更新模式
    void refresh(bool partial_update_mode = false) // screen refresh from controller memory to full screen
    {
      epd2.refresh(partial_update_mode);
      if (!partial_update_mode) epd2.powerOff();
    }
    // 屏幕刷新从控制器内存到部分屏幕
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h) // screen refresh from controller memory, partial screen
    {
      epd2.refresh(x, y, w, h);
    }
    // 关闭面板驱动电压生成，避免屏幕随时间推移而褪色
    void powerOff()
    {
      epd2.powerOff();
    }
    // 关闭powerOff()并将控制器设置为深度睡眠以最小化功耗，仅当可由RST唤醒时(rst >= 0)
    void hibernate()
    {
      epd2.hibernate();
    }
  private:
    // 交换两个变量的值
    template <typename T> static inline void
    _swap_(T & a, T & b)
    {
      T t = a;
      a = b;
      b = t;
    };
    // 计算两个uint16_t值的最小值
    static inline uint16_t gx_uint16_min(uint16_t a, uint16_t b)
    {
      return (a < b ? a : b);
    };
    // 计算两个uint16_t值的最大值
    static inline uint16_t gx_uint16_max(uint16_t a, uint16_t b)
    {
      return (a > b ? a : b);
    };
    // 根据旋转角度调整坐标和尺寸参数
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
    {
      switch (getRotation())
      {
        case 1:
          _swap_(x, y);
          _swap_(w, h);
          x = WIDTH - x - w;
          break;
        case 2:
          x = WIDTH - x - w;
          y = HEIGHT - y - h;
          break;
        case 3:
          _swap_(x, y);
          _swap_(w, h);
          y = HEIGHT - y - h;
          break;
      }
    }
    // 将16位颜色转换为4级颜色值
    uint8_t color4(uint16_t color)
    {
      static uint16_t _prev_color = GxEPD_BLACK;
      static uint8_t _prev_color4 = 0x00; // black
      if (color == _prev_color) return _prev_color4;
      uint8_t cv4 = 0x00;
      switch (color)
      {
        case GxEPD_BLACK: cv4 = 0x00; break;
        case GxEPD_WHITE: cv4 = 0x01; break;
        case GxEPD_GREEN: cv4 = 0x02; break; // use yellow?
        case GxEPD_BLUE:  cv4 = 0x00; break; // use black
        case GxEPD_RED:   cv4 = 0x03; break;
        case GxEPD_YELLOW: cv4 = 0x02; break;
        case GxEPD_ORANGE: cv4 = 0x02; break; // use yellow?
        default:
          {
            uint16_t red = color & 0xF800;
            uint16_t green = (color & 0x07E0) << 5;
            uint16_t blue = (color & 0x001F) << 11;
            if ((red < 0x8000) && (green < 0x8000) && (blue < 0x8000)) cv4 = 0x00; // black
            else if ((red >= 0x8000) && (green >= 0x8000) && (blue >= 0x8000)) cv4 = 0x01; // white
            else if ((red >= 0x8000) && (blue >= 0x8000)) cv4 = 0x03; //  red, blue > red
            else if ((green >= 0x8000) && (blue >= 0x8000)) cv4 = 0x01; //  green, blue > white
            else if ((red >= 0x8000) && (green >= 0xC000)) cv4 = 0x02; // yellow
            else if ((red >= 0x8000) && (green >= 0x4000)) cv4 = 0x03; // orange > red
            else if (red >= 0x8000) cv4 = 0x03; // red
            else if (green >= 0x8000) cv4 = 0x00; // green > black
            else cv4 = 0x03; // blue
          }
      }
      _prev_color = color;
      _prev_color4 = cv4;
      return cv4;
    }
  private:
    uint8_t _pixel_buffer[(GxEPD2_Type::WIDTH / 4) * page_height];
    bool _using_partial_mode, _second_phase, _mirror;
    uint16_t _width_bytes, _pixel_bytes;
    int16_t _current_page;
    uint16_t _pages, _page_height;
    uint16_t _pw_x, _pw_y, _pw_w, _pw_h;
};

#endif