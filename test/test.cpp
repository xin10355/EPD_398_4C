class test : {
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

      // 垂直交错扫描
      if (y % 2 == 0) 
      {
        //右边 (_page_height/2) ~ (_page_height-1)
        if (y < (_page_height / 2)) {
          y = height() - y - 1;
          y += (_page_height / 2);
        } else {
          return;
        }
      } else {
        //左边 0~(_page_height/2-1)
        if (y < (_page_height / 2)) {
          //nothing to do
        } else {
          return;
        }
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
      // switch(x % 4)
      // {
      //   case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F) | (pv << 6); break;
      //   case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF) | (pv << 4); break;
      //   case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3) | (pv << 2); break;
      //   case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC) | pv; break;
      // }

      //Serial.printf("Drawing pixel at (%d, %d) \n", x, y);
      //Serial.printf("Page height: %d \n", _page_height);
      if (y % 2 == 0) 
      {
        //右边
        if (y < (_page_height / 2)) {
          switch(x % 4)
          {
            case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F) | (pv << 6); break;
            case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF) | (pv << 4); break;
            case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3) | (pv << 2); break;
            case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC) | pv; break;
          }
          
        } else {
          // switch(x % 4)
          // {
          //   case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F); break;
          //   case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF); break;
          //   case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3); break;
          //   case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC); break;
          // }
        }
      } else {
        //左边
        if (y < (_page_height / 2)) {
          switch(x % 4)
          {
            case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F) | (pv << 6); break;
            case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF) | (pv << 4); break;
            case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3) | (pv << 2); break;
            case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC) | pv; break;
          }
        } else {
          // switch(x % 4)
          // {
          //   case 0: _pixel_buffer[i] = (_pixel_buffer[i] & 0x3F); break;
          //   case 1: _pixel_buffer[i] = (_pixel_buffer[i] & 0xCF); break;
          //   case 2: _pixel_buffer[i] = (_pixel_buffer[i] & 0xF3); break;
          //   case 3: _pixel_buffer[i] = (_pixel_buffer[i] & 0xFC); break;
          // }
        }
      }

    }
};
