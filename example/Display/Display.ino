#include <VKL144.h>
VKL144 vkl;
void setup()
{
    // put your setup code here, to run once:
    vkl.Init();    //初始化
    vkl.DisAll(1); //全部打开
    delay(5000);
    vkl.DisAll(0); //全部关上
    delay(5000);
    vkl.Disfloat(-1.24); //绘制浮点数
    vkl.Updata();        //更新屏幕内容
    delay(5000);
    vkl.cleanBuffer();
    for (int i = 1; i < 5; i++) //轮番显示一遍数字
    {
        for (int j = 0; j < 10; j++)
        {
            vkl.cleanBuffer();
            vkl.DisNum(j, i);
            vkl.Updata();
            delay(1000);
        }
    }
    for (int i = 0; i < 4; i++) //轮番显示一遍点
    {
        vkl.cleanBuffer();
        vkl.DisPoint(i);
        vkl.Updata();
        delay(1000);
    }
    delay(5000);
    vkl.Half(); //关闭屏幕，进入低功耗省电模式
}

void loop()
{
    // put your main code here, to run repeatedly:
}
