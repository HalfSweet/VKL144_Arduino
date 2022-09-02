#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define VKL144_ADDR (0x7c >> 1)     // IIC地址
#define VKL144_ICSET 0xe8           // IC设置  bit0时钟源  bit1软复位	bit2映射地址bit5
                                    // 默认为0 内部RC晶振  不执行软件复位
#define BIT0_CLKEXT 0x01            // 外部时钟
#define BIT1_ENSRST 0x02            // 执行软件复位
#define BIT2_ADDR5 0x04             // 映射地址bit5
#define VKL144_ADSET 0x00           // 映射起始地址bit4~bit0
#define VKL144_MODESET_1_3_ON 0xc8  // 打开显示 1/3bias
#define VKL144_MODESET_1_3_OFF 0xc0 // 关闭显示 1/3bias
#define VKL144_MODESET_1_2_ON 0xcc  // 打开显示 1/2bias
#define VKL144_MODESET_1_2_OFF 0xc4 // 关闭显示 1/2bias

//显示控制命令
#define VKL144_DISCTL 0xA0 // bit7~bit5=101

// bit4-bit3省电模式FR
#define VKL144_FRNOR 0x00 // bit4-bit3=00   FR NORMAL 				上电默认
#define VKL144_FRPM1 0x08 // bit4-bit3=01   FR POWER SAVE MODE1
#define VKL144_FRPM2 0x10 // bit4-bit3=10   FR POWER SAVE MODE2
#define VKL144_FRPM3 0x18 // bit4-bit3=11   FR POWER SAVE MODE3 	最省电

// bit2 LINE,FRAME驱动波形LINE 翻转时的工作电流>FRAME翻转的电流
#define VKL144_LINER 0x00  // bit2=0   LINE翻转	 					上电默认
#define VKL144_FRAMER 0x04 // bit2=1   FRAME翻转 					最省电

// bit1-bit0省电模式SR
#define VKL144_SRHP 0x03  // bit1-bit0=11   SR NORMAL 				最大
#define VKL144_SRNOR 0x02 // bit1-bit0=10   SR POWER SAVE MODE1 	上电默认
#define VKL144_SRPM2 0x01 // bit1-bit0=01   SR POWER SAVE MODE2
#define VKL144_SRPM1 0x00 // bit1-bit0=00   SR POWER SAVE MODE1 	最省电

//常用显示控制组合
// VKL144_DISCTL|VKL144_FRNOR|VKL144_LINER |VKL144_SRHP      //电流最大
// VKL144_DISCTL|VKL144_FRPM1|VKL144_FRAMER|VKL144_FRAMER    //电流典型
// VKL144_DISCTL|VKL144_FRPM3|VKL144_FRAMER|VKL144_SRPM1    	//电流最省

//工作电流设置
#define VKL144_DISCTL_DATA VKL144_DISCTL | VKL144_FRPM3 | VKL144_FRAMER | VKL144_SRPM1

//闪烁控制
#define VKL144_BLKCTL_OFF 0xF0  // 关闭闪烁
#define VKL144_BLKCTL_05HZ 0xF1 // 闪烁频率为0.5HZ
#define VKL144_BLKCTL_1HZ 0xF2  // 闪烁频率为1HZ
#define VKL144_BLKCTL_2HZ 0xF3  // 闪烁频率为2HZ

//全屏强行开或关,与显示内存内容无关,两个功能中全屏关为优先执行
#define VKL144_APCTL_APON_ON 0xFE   // 全屏强行全显示_开
#define VKL144_APCTL_APON_OFF 0xFC  // 全屏强行全显示_关
#define VKL144_APCTL_APOFF_ON 0xFD  // 全屏强行关显示_开
#define VKL144_APCTL_APOFF_OFF 0xFC // 全屏强行关显示_关

//驱动seg数
#define VKL144_SEGNUM 8
//#define VKL144_RAMBUFNUM VKL144_SEGNUM / 2 + 1 //偶数除2，奇数除2+1
#define VKL144_RAMBUFNUM (VKL144_SEGNUM / 2 + 1)

// 1/3bias		1/2bias		1=有效		0=无效
#define BIAS_1_2 0
#define BIAS_1_3 1

#define LCD_A 0x01
#define LCD_B 0x02
#define LCD_C 0x04
#define LCD_D 0x08
#define LCD_E 0x10
#define LCD_F 0x20
#define LCD_G 0x40
#define LCD_P 0x00

#define SEG_0 (1 << 0)
#define SEG_1 (1 << 1)
#define SEG_2 (1 << 2)
#define SEG_3 (1 << 3)
#define SEG_4 (1 << 4)
#define SEG_5 (1 << 5)
#define SEG_6 (1 << 6)
#define SEG_7 (1 << 7)

class VKL144
{
private:
    /* data */

    const uint8_t VKL144_segtab[VKL144_SEGNUM] =
        {
            7, 8, 9, 10, 11, 12, 13, 14};

    uint8_t VKL144_addrbit = 6; //驱动ic的ram地址线数A5-A0
    uint8_t VKL144_segi, VKL144_comi;
    uint8_t VKL144_dispram[VKL144_RAMBUFNUM] = {0};

    void FunSet(uint8_t cmd);
    //void NumInit();                                              //自动生成液晶屏需要的数组
    void WriteData(uint8_t Addr, uint8_t *Databuf, uint8_t Cnt); //写数组
    void WriteData(uint8_t Addr, uint8_t Dat);                   //写单个字节数据

public:
    VKL144(/* args */);;
    ~VKL144();

    void Init();
    void DisAll(uint8_t state);
    void DisSegOn(uint8_t Seg, uint8_t Com);
    void DisSegOff(uint8_t Seg, uint8_t Com);
    void Half();
    void DisplayTime(uint16_t Time);
    void DisNum(uint8_t num, uint8_t order);
    void DisPoint(uint8_t order);
    void Disfloat(float num);
    void Updata()
    {
        uint8_t dispram[8] = {0};
        for (int i = 0; i < VKL144_RAMBUFNUM; i++)
            dispram[VKL144_segtab[0] / 2 + i] = VKL144_dispram[i];
        WriteData(0, dispram, 8);
    }
    void cleanBuffer()
    {
        for (int i = 0; i < VKL144_RAMBUFNUM; i++)
        {
            VKL144_dispram[i] &= 0x00;
        }
    }
};
