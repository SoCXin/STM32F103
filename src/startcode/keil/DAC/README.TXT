实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习STM32内部DAC的使用
	
硬件资源:
	1,DS0(连接在PA8) 
	2,串口1(波特率:9600,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h) 
	4,按键KEY0(PC5)/KEY_UP(PA0,也称之为WK_UP)
	5,DAC(STM32内部DAC通道1,通过PA4输出)
	6,ADC(STM32内部ADC1_CH1,连接在PA1).
	
实验现象:
	本实验利用按键（KEY0/KEY_UP两个按键,或USMART）控制STM32F1内部DAC模块的通道1来输出电压，通过ADC1的
	通道1采集DAC的输出电压，在LCD模块上面显示ADC获取到的电压值以及DAC的设定输出电压值等信息。同时可以通
	过usmart调用Dac1_Set_Vol函数，来直接设置DAC输出电压。
	
注意事项:  
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插
	4,STM32F103RCT6的ADC/DAC参考电压恒为VDDA（即3.3V）
	5,请用杜邦线连接PA1和PA4，方便查看测试结果。
	  

			正点原子@ALIENTEK
			2014-3-09
			广州市星翼电子科技有限公司
			电话：020-38271790
			传真：020-36773971
			购买：http://shop62103354.taobao.com
			http://shop62057469.taobao.com
			公司网站：www.alientek.com
			技术论坛：www.openedv.com