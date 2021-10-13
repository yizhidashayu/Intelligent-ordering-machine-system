#include "lcd.h"

//打开lcd的帧缓冲和触摸屏
int lcd_open(void)
{
	//打开lcd屏幕
	lcdfd=open(LCDDEV,O_RDWR);
	if(lcdfd == -1)
	{
		perror("open error\n");
		return -1;
	}
	//获取lcd的可变参数
	ioctl(lcdfd,FBIOGET_VSCREENINFO,&var);
	//计算可视化区域的显示字节数
	size=var.xres*var.yres*var.bits_per_pixel/8;
	
	//建立显存映射（将lcd的显存空间映射到系统内存）
	FB=(int *)mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(FB == MAP_FAILED)
	{
		perror("mmap error\n");
		return -1;
	}
	//打开文件
	tsfd=open(Event,O_RDWR);
	if(tsfd == -1)
	{
	   perror("open error\n");
	   return -1;	   
	}
	return 0;
}

//关闭lcd的帧缓冲和触摸屏
void lcd_close(void)
{
	munmap(FB,size);//取消映射
	close(tsfd);//关闭触摸屏
	close(lcdfd);//关闭lcd
}

//点击获取（登陆界面）
int get_xy(struct inputxy *point)
{
	//从输入子系统中读取数据
	struct input_event tsevent;
	//循环获取一次坐标
	while(1)
	{
	    //读函数获取结构体中的数据
        read(tsfd,&tsevent,sizeof(tsevent));
        //判断触摸事件以及x轴和y轴
		//获取x轴坐标  类型为触摸屏  编码为X轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_X)      point->x=tsevent.value*800/1024;
        //获取y轴坐标  类型为触摸屏  编码为y轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_Y)      point->y=tsevent.value*480/600;
        //点击获取，在按下压力值大于0, 松开压力值等于0
        if(tsevent.type == EV_KEY && tsevent.code == BTN_TOUCH && tsevent.value == 0)
		       break;//跳出循环 获取成功一个坐标
	}
	printf("(%d %d)\n",point->x,point->y);
	if ((point->x)>35 && (point->x)<742 && (point->y>4) && (point->y)<155)
		return 1;
	if ((point->x)>38 && (point->x)<740 && (point->y>182) && (point->y)<275)
		return 0;
	if ((point->x)>35 && (point->x)<745 && (point->y>315) && (point->y)<367)
		return 2;
	if ((point->x)>285 && (point->x)<700 && (point->y>400) && (point->y)<600)
		return 3;
	}



//滑动获取
int get_move(struct inputxy *old,struct inputxy *new)
{
	//old和new分别记录压力值大于0和压力值等于0坐标
	
	//获取old的坐标，也就是刚刚按下去的坐标值
	//从输入子系统中读取数据
	struct input_event in;
	//循环获取一次坐标
	while(1)
	{
	    //读函数获取结构体中的数据
        read(tsfd,&in,sizeof(in));
        //判断触摸事件以及x轴和y轴
		//获取x轴坐标  类型为触摸屏  编码为X轴
        if(in.type == EV_ABS && in.code == ABS_X)       
			old->x=in.value*800/1024;
        //获取y轴坐标  类型为触摸屏  编码为y轴
        if(in.type == EV_ABS && in.code == ABS_Y)       
			old->y=in.value*480/600;
        //点击获取，在按下压力值大于0
        if(in.type == EV_KEY && in.code == BTN_TOUCH && in.value >0)
		       break;//跳出循环 获取成功一个坐标
	}
	
	//获取new的坐标值，也就是最后松开的坐标
	//循环获取一次坐标
	while(1)
	{
	    //读函数获取结构体中的数据
        read(tsfd,&in,sizeof(in));
        //判断触摸事件以及x轴和y轴
		//获取x轴坐标  类型为触摸屏  编码为X轴
        if(in.type == EV_ABS && in.code == ABS_X)       
			new->x=in.value*800/1024;
        //获取y轴坐标  类型为触摸屏  编码为y轴
        if(in.type == EV_ABS && in.code == ABS_Y)       
			new->y=in.value*480/600;
        //点击获取，在松开压力值等于0
        if(in.type == EV_KEY && in.code == BTN_TOUCH && 
		   in.value ==0)
		       break;//跳出循环 获取成功一个坐标
	}
	printf("old (%d,%d)\n",old->x,old->y);
	printf("new (%d,%d)\n",new->x,new->y);
	
	//获取old和new之间的差值
	int subx=(old->x)-(new->x);
	int suby=(old->y)-(new->y);
	
	//左上角方向
	if(subx>=0 && suby>=0)
	{
		if(subx-suby >=0)//x轴的差值大于y轴的差值
		    return MOVE_LEFT;//向左
		else
			return MOVE_UP;//向上
			
	}
	//左下角方向
	if(subx>=0 && suby<0)
	{
	   if(subx-(-suby) >=0)//x轴的差值大于y轴的差值
		    return MOVE_LEFT;//向左
		else
			return MOVE_DOWN;//向下 
	}
	//右上角方向
	if(subx<0 && suby >=0)
	{
		if(-subx >= suby) //X轴的差值大于y轴的差值
		    return MOVE_RIGHT;//向右移动 y轴不变
		else 
			return MOVE_UP;// 向上移动 x轴没变
	}
	//右下角方向
	if(subx<0 && suby<0)
	{
		if(-subx >= -suby) //X轴的差值大于y轴的差值
		    return MOVE_RIGHT;//向右移动 y轴不变
		else
			return MOVE_UP;// 向下移动 x轴没变
	}
	
}

//int x起点横坐标,int y起点纵坐标,const char *pathname
int show_bmp(int x,int y,const char *pathname)
{
	int ret;
	//打开图片 获取图片数据
	int bmpfd=open(pathname,O_RDWR);
	if(bmpfd == -1)
	{
		perror("open error\n");
		return -1;
	}
	
	//读取bmp的头信息
	u_32c headinfo[54]={0};//存储54字节头信息
	ret=read(bmpfd,headinfo,54);//读取54字节头信息
	//printf("当前读取头信息%d字节\n",ret);
	
	//获取bmp的图片的高宽
	u_32t bmp_w=headinfo[18]|headinfo[19]<<8|headinfo[20]<<16|headinfo[21]<<24;
	u_32t bmp_h=headinfo[22]|headinfo[23]<<8|headinfo[24]<<16|headinfo[25]<<24;
    //printf("当前获取的图像的宽为%d,高为%d\n",bmp_w,bmp_h);
	
	u_32t win_size=(4-bmp_w*3%4)%4;//window补齐字节数
	u_32t bmp_lenth=bmp_w*3+win_size;//图片一行字节数
	//printf("当前获取的图片的长度为%d\n",win_size+bmp_w);
	
	//跳过前54个字节
	lseek(bmpfd,54,SEEK_SET);
	
	//读取BMP位图的有效的RGB数据
	char *bmpbuf=malloc(bmp_lenth*bmp_h);//申请读取图片空间
	ret=read(bmpfd,bmpbuf,bmp_lenth*bmp_h);//读取位图有效数据
	//printf("当前读取到有效字节数%d\n",ret);
	
	//将读取到BGR数据转为LCD的ARGB数据
	unsigned int lcdbuf[bmp_w*bmp_h];//字节数:bmp_w*bmp_h*4
	for(int j=0;j<bmp_h;j++)//管理转换像素
	{
		for(int i=0;i<bmp_w;i++)
		{
			//4字节                                          
	      lcdbuf[i+bmp_w*j]=bmpbuf[3*i+0]<<0|//1字节B 
			                bmpbuf[3*i+1]<<8|//1字节G
				            bmpbuf[3*i+2]<<16|//1字节R  
				            0x00<<24;         //1字节A
		}
        //更换到下一行
		bmpbuf +=bmp_w*3+win_size;
	}

	//图片翻转
	for(int j=0;j<var.yres-y&&j<bmp_h;j++)//480
	{
		for(int i=0;i<var.xres-x&&i<bmp_w;i++)//800
		{
			//将图片最后一行写入到lcd的第一行，依次进行
			//y*800+x 0             bmp_h-1-y 
			//        1             bmp_h-1-1           
			FB[(y+j)*var.xres+(x+i)]=lcdbuf[(bmp_h-1-j)*bmp_w+i];
			
		}
	}
	
	//10.关闭相关的文件
	close(bmpfd);//关闭图片
}

//进度条
void show_jin(const char *pathname)
{
	//定义进度条显示的起始位置
	int start=80;
	
	//显示进度条
	while(1)
	{
	    //显示小图片x  y
        show_bmp(start,400,pathname);
        //微秒延时
        usleep(50000);
		//sleep(1);
        //更新起点位置
        start += 10;
        //如果显示到720停止
        if(start > 720)
             break;			
	}
}

int little_bmp(int x,int y,const char *pathname)
{
	int ret;
	//打开图片 获取图片数据
	int bmpfd=open(pathname,O_RDWR);
	if(bmpfd == -1)
	{
		perror("open error\n");
		return -1;
	}
	
	//读取bmp的头信息
	u_32c headinfo[54]={0};//存储54字节头信息
	ret=read(bmpfd,headinfo,54);//读取54字节头信息
	//printf("当前读取头信息%d字节\n",ret);
	
	//获取bmp的图片的高宽
	u_32t bmp_w=headinfo[18]|headinfo[19]<<8|headinfo[20]<<16|headinfo[21]<<24;
	u_32t bmp_h=headinfo[22]|headinfo[23]<<8|headinfo[24]<<16|headinfo[25]<<24;
    //printf("当前获取的图像的宽为%d,高为%d\n",bmp_w,bmp_h);
	
	u_32t win_size=(4-bmp_w*3%4)%4;//window补齐字节数
	u_32t bmp_lenth=bmp_w*3+win_size;//图片一行字节数
	//printf("当前获取的图片的长度为%d\n",win_size+bmp_w);
	
	//跳过前54个字节
	lseek(bmpfd,54,SEEK_SET);
	
	//读取BMP位图的有效的RGB数据
	char *bmpbuf=malloc(bmp_lenth*bmp_h);//申请读取图片空间
	ret=read(bmpfd,bmpbuf,bmp_lenth*bmp_h);//读取位图有效数据
	//printf("当前读取到有效字节数%d\n",ret);
	
	//将读取到BGR数据转为LCD的ARGB数据
	unsigned int lcdbuf[bmp_w*bmp_h];//字节数:bmp_w*bmp_h*4
	for(int j=0;j<bmp_h;j++)//管理转换像素
	{
		for(int i=0;i<bmp_w;i++)
		{
			//4字节                                          
	      lcdbuf[i+bmp_w*j]=bmpbuf[3*i+0]<<0|//1字节B 
			                bmpbuf[3*i+1]<<8|//1字节G
				            bmpbuf[3*i+2]<<16|//1字节R  
				            0x00<<24;         //1字节A
		}
        //更换到下一行
		bmpbuf +=bmp_w*3+win_size;
	}
	int n=3;
	//图片翻转
	for(int j=0;j<var.yres-y&&j<bmp_h/n;j++)//480
	{
		for(int i=0;i<var.xres-x&&i<bmp_w/n;i++)//800
		{
			//将图片最后一行写入到lcd的第一行，依次进行
			//y*800+x 0             bmp_h-1-y 
			//        1             bmp_h-1-1           
			FB[(y+j)*var.xres+(x+i)]=lcdbuf[(bmp_h-1-(j*n))*bmp_w+(i*n)];
			
		}
	}
	
	//10.关闭相关的文件
	close(bmpfd);//关闭图片
}


//单击获取x和y的坐标(密码判断)
int get_xy_lu(struct inputxy *point)
{
	//从输入子系统中读取数据
	struct input_event tsevent;
	//循环获取一次坐标
	while(1)
	{
	    //读函数获取结构体中的数据
        read(tsfd,&tsevent,sizeof(tsevent));
        //判断触摸事件以及x轴和y轴
		//获取x轴坐标  类型为触摸屏  编码为X轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_X)      point->x=tsevent.value*800/1024;
        //获取y轴坐标  类型为触摸屏  编码为y轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_Y)      point->y=tsevent.value*480/600;
        //点击获取，在按下压力值大于0, 松开压力值等于0
        if(tsevent.type == EV_KEY && tsevent.code == BTN_TOUCH && tsevent.value == 0)  
		  break;//跳出循环 获取成功一个坐标
	}
	printf("(%d,%d)\n",point->x,point->y);
		if ((point->x)>75 && (point->x)<194 && (point->y>262) && (point->y)<314)
		return 0;
		if ((point->x)>235 && (point->x)<350 && (point->y>267) && (point->y)<308)
		return 1;
		if ((point->x)>383 && (point->x)<506 && (point->y>265) && (point->y)<305)
		return 2;
		if ((point->x)>545 && (point->x)<669 && (point->y>265) && (point->y)<302)
		return 3;
		if ((point->x)>233 && (point->x)<343 && (point->y>346) && (point->y)<395)
		return 4;
		if ((point->x)>395 && (point->x)<510 && (point->y>351) && (point->y)<392)
		return 5;
		if ((point->x)>541 && (point->x)<680 && (point->y>351) && (point->y)<385)
		return 6;
		if ((point->x)>237 && (point->x)<347 && (point->y>416) && (point->y)<474)
		return 7;
		if ((point->x)>393 && (point->x)<510 && (point->y>415) && (point->y)<476)
		return 8;
		if ((point->x)>544 && (point->x)<682 && (point->y>409) && (point->y)<476)
		return 9;
		if ((point->x)>74 && (point->x)<191 && (point->y>351) && (point->y)<400)
		return 10;
		if ((point->x)>72 && (point->x)<194 && (point->y>418) && (point->y)<471)
		return 11;
		else 
			return 12;
	}


//点击获取(点菜)
int get_xy_cai(struct inputxy *point)
{
	//从输入子系统中读取数据
	struct input_event tsevent;
	//循环获取一次坐标
	while(1)
	{
	    //读函数获取结构体中的数据
        read(tsfd,&tsevent,sizeof(tsevent));
        //判断触摸事件以及x轴和y轴
		//获取x轴坐标  类型为触摸屏  编码为X轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_X)      point->x=tsevent.value*800/1024;
        //获取y轴坐标  类型为触摸屏  编码为y轴
        if(tsevent.type == EV_ABS && tsevent.code == ABS_Y)      point->y=tsevent.value*480/600;
        //点击获取，在按下压力值大于0, 松开压力值等于0
        if(tsevent.type == EV_KEY && tsevent.code == BTN_TOUCH && tsevent.value == 0)  
		  break;//跳出循环 获取成功一个坐标
	}
	printf("(%d,%d)\n",point->x,point->y);
	if ((point->x)>0 && (point->x)<350 && (point->y>0) && (point->y)<190)
		return 0;
	if ((point->x)>450 && (point->x)<800 && (point->y>0) && (point->y)<190)
		return 1;
	if ((point->x)>0 && (point->x)<350 && (point->y>290) && (point->y)<480)
		return 2;
	if ((point->x)>450 && (point->x)<800 && (point->y>290) && (point->y)<480)
		return 3;
	if ((point->x)>350 && (point->x)<390 && (point->y>190) && (point->y)<230)
		return 4;
	if ((point->x)>390 && (point->x)<430 && (point->y>190) && (point->y)<230)
		return 5;
	if ((point->x)>350 && (point->x)<450 && (point->y>380) && (point->y)<480)
		return 6;
}