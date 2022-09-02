#include "VKL144.h"
#include <Wire.h>

VKL144::VKL144(/* args */)
{
    Wire.begin();
}

VKL144::~VKL144()
{
    Wire.end();
}

void VKL144::FunSet(uint8_t cmd)
{
    uint8_t i;

    i = 1;
    while (i--)
    {
        delay(10);
        Wire.beginTransmission(VKL144_ADDR);
        Wire.write(cmd);
        Wire.endTransmission();
    }
    // Wire.endTransmission();
}

void VKL144::Init()
{
    Wire.beginTransmission(VKL144_ADDR);
    Wire.write(VKL144_ICSET | BIT1_ENSRST); //复位
    Wire.write(VKL144_ICSET);               //映射起始地址 bit5=0
    Wire.write(VKL144_ADSET);               //映射起始地址 bit4~bit0=0
    for (int n = 0; n < VKL144_RAMBUFNUM; n++)
    {
        Wire.write(0x00); // 初始化的显示内存显示映射,可修改这里.
    }
    Wire.endTransmission();
    delay(5);
    Wire.beginTransmission(VKL144_ADDR);

#if BIAS_1_2
    Wire.write(VKL144_MODESET_1_2_ON); //打开显示 1/2bias
#endif

#if BIAS_1_3
    Wire.write(VKL144_MODESET_1_3_ON); //打开显示 1/3bias
#endif
    Wire.endTransmission();
    FunSet(VKL144_DISCTL_DATA); //工作电流设置
    DisAll(0);
}

void VKL144::WriteData(uint8_t Addr, uint8_t *Databuf, uint8_t Cnt)
{
    uint8_t i, n;

    i = 1;
    while (i--)
    {
        Wire.beginTransmission(VKL144_ADDR);
        if (Addr > 0x1f)
            Wire.write(VKL144_ICSET | BIT2_ADDR5);
        else
            Wire.write(VKL144_ICSET);

        Wire.write(Addr & 0x1f);
        for (n = 0; n < Cnt; n++)
        {
            Wire.write(*Databuf++);
        }
    }
    Wire.endTransmission();
}

void VKL144::WriteData(uint8_t Addr, uint8_t Dat)
{
    unsigned char i;

    i = 1;
    while (i--)
    {

        Wire.beginTransmission(VKL144_ADDR);
        if (Addr > 0x1f)
            Wire.write(VKL144_ICSET | BIT2_ADDR5);
        else
            Wire.write(VKL144_ICSET);

        Wire.write(Addr & 0x1f);

        Wire.write(Dat);
    }
    Wire.endTransmission();
}

void VKL144::DisAll(uint8_t state)
{
    uint8_t segi;
    uint8_t dispram[8];

    /* for (segi = 0; segi < 18 / 2; segi++)
    {
        dispram[segi] = 0;
    } */
    for (segi = 0; segi < VKL144_RAMBUFNUM; segi++)
    {
        if (state == 1)
        {
            VKL144_dispram[segi] = 0xff;
        }
        else
        {
            VKL144_dispram[segi] = 0x00;
        }
        dispram[VKL144_segtab[0] / 2 + segi] = VKL144_dispram[segi];
    }
    WriteData(0, dispram, 8);
}

void VKL144::DisSegOn(uint8_t Seg, uint8_t Com)
{
    if (Seg % 2 == 0)
    {
        VKL144_dispram[(Seg - VKL144_segtab[0]) / 2] |= (1 << (Com + 4));
        WriteData(Seg, VKL144_dispram[(Seg - VKL144_segtab[0]) / 2]);
    }
    else
    {
        VKL144_dispram[(Seg - VKL144_segtab[0]) / 2] |= (1 << Com);
        WriteData(Seg - 1, VKL144_dispram[(Seg - VKL144_segtab[0]) / 2]);
    }
}

void VKL144::DisSegOff(uint8_t Seg, uint8_t Com)
{
    if (Seg % 2 == 0)
    {
        VKL144_dispram[(Seg - VKL144_segtab[0]) / 2] &= ~(1 << (Com + 4));
        WriteData(Seg, VKL144_dispram[(Seg - VKL144_segtab[0]) / 2]);
    }
    else
    {
        VKL144_dispram[(Seg - VKL144_segtab[0]) / 2] &= ~(1 << Com);
        WriteData(Seg - 1, VKL144_dispram[(Seg - VKL144_segtab[0]) / 2]);
    }
}

void VKL144::DisNum(uint8_t num, uint8_t order)
{
    if (order > 5)
        return;
    switch (num)
    {
    case 0:
        VKL144_dispram[4 + 1 - order] |= SEG_7 | SEG_6 | SEG_5;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_3;
        break;
    case 1:
        VKL144_dispram[4 + 1 - order - 1] |= SEG_1 | SEG_3;
        break;
    case 2:
        VKL144_dispram[4 + 1 - order] |= SEG_7 | SEG_5;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_2 | SEG_3;
        break;
    case 3:
        VKL144_dispram[4 + 1 - order] |= SEG_7;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_2 | SEG_3;
        break;
    case 4:
        VKL144_dispram[4 + 1 - order] |= SEG_6;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_1 | SEG_2 | SEG_3;
        break;
    case 5:
        VKL144_dispram[4 + 1 - order] |= SEG_7 | SEG_6;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_2;
        break;
    case 6:
        VKL144_dispram[4 + 1 - order] |= SEG_5 | SEG_7 | SEG_6;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_2;
        break;
    case 7:
        VKL144_dispram[4 + 1 - order] |= SEG_7;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_1 | SEG_3;
        break;
    case 8:
        VKL144_dispram[4 + 1 - order] |= SEG_5 | SEG_7 | SEG_6;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_2 | SEG_3;
        break;
    case 9:
        VKL144_dispram[4 + 1 - order] |= SEG_7 | SEG_6;
        VKL144_dispram[4 + 1 - order - 1] |= SEG_0 | SEG_1 | SEG_2 | SEG_3;
        break;
    default:
        break;
    }
}

void VKL144::DisPoint(uint8_t order)
{
    if (order > 3)
        return;
    VKL144_dispram[4 - order] |= SEG_4;
}

void VKL144::Half()
{
    unsigned char i;

    i = 1;
    while (i--)
    {
        Wire.beginTransmission(VKL144_ADDR);

#if BIAS_1_2
        Wire.write(VKL144_MODESET_1_2_OFF); //关闭显示 1/2bias
#endif

#if BIAS_1_3
        Wire.write(VKL144_MODESET_1_3_OFF); //关闭显示 1/3bias
#endif
    }
    Wire.endTransmission();
}

void VKL144::Disfloat(float num)
{
    String temp;
    uint8_t pointFlag = 0;
    if (num < 0)
    {
        num = abs(num);
        temp = String(num, 2);
        VKL144_dispram[3] |= SEG_2;
        for (int i = 0; i < 5; i++)
        {
            if (temp[i] == '.')
            {
                DisPoint(i + 1);
                pointFlag = 1;
            }
            else
            {
                DisNum(temp[i] - '0', i - pointFlag + 2);
            }
        }
    }
    else
    {
        temp = String(num, 3);
        for (int i = 0; i < 5; i++)
        {
            if (temp[i] == '.')
            {
                DisPoint(i);
                pointFlag = 1;
            }
            else
            {
                DisNum(temp[i] - '0', i - pointFlag + 1);
            }
        }
    }
}

void VKL144::DisplayTime(uint16_t Time)
{
    String temp = String(Time);
    for (int i = 1; i <= 4; i++)
    {
        DisNum(temp[i - 1], i);
    }
}