#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define BCDtoDEC(x) ( ((x)>>4)*10+(x)%0x10  )           //BCD码转换为十进制表示方式
#define DECtoBCD(x) ( (((x)/10)<<4)+(x)%10  )           //十进制转换为BCD码表示方式


#endif

