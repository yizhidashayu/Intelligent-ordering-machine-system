#include "lcd.h"



int main()
{
	peo head=crivte_haed();
	caa heads=crivte_haed_jie();
	peo newnode;
	caa newnodes;
	//打开lcd设备
    lcd_open();
	show_bmp(0,0,"welocm.bmp");
	show_jin("./jin.bmp");
	int m;
	
	//判断所选
	m=touch1(head,newnode,heads,newnodes);
	//关闭lcd设备
    lcd_close();
	return 0;
}

