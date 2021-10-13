#ifndef _LCD_H
#define _LCD_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>//帧缓冲
#include <linux/input.h>//输入子系统
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>

#define LCDDEV "/dev/fb0"//帧缓冲的驱动
#define Event  "/dev/input/event0"//触摸屏驱动
#define MOVE_LEFT  1
#define MOVE_RIGHT 2
#define MOVE_UP    3
#define MOVE_DOWN  4


//坐标结构体
typedef struct inputxy
{
	int x;
	int y;
}input,*p;

//账号结构体 
typedef struct VIP
{
	int zhang[8];
	int mi[6];
	struct VIP *next;
	struct VIP *prev;
}people,*peo;

//菜单
typedef struct caidan
{
	char *name;
	char *price;
	struct caidan *next;
	struct caidan *prev;
}cai,*pcai;


//结算
typedef struct jiesuan
{
	struct jiesuan *next;
	struct jiesuan *prev;
	char *price;
	int prices;
}jie,*caa;

static struct fb_var_screeninfo var;//可变参数结构体
static int tsfd;//触摸屏的文件描述符
static int lcdfd;//帧缓冲的文件描述符
static int *FB;//映射lcd
static int size;//可见区大小
typedef  unsigned int u_32t;
typedef  unsigned char u_32c;

/*==关于lcd的帧缓冲、触摸屏打开和关闭==*/
int lcd_open(void);
//关闭lcd的帧缓冲和触摸屏
void lcd_close(void);

/*==========显示图片=========*/
int show_bmp(int x,int y,const char *pathname);

/*==========获取触摸数据=====*/
//单击获取x和y的坐标
int  get_xy(struct inputxy *point);
//滑动获取
int get_move(struct inputxy *old,struct inputxy *new);
//进度条声明
void show_jin(const char *pathname);
//小板图片
int little_bmp(int x,int y,const char *pathname);

//管理登录
void menu();
//会员头
peo crivte_haed();
//会员头
peo crivte_newnode(peo head,int zhang[],int mi[],int a,int b);

//尾部添加
void add_tail(peo head,peo newnode);
//菜单
pcai crivte_haed_cai();
//菜单
pcai crivte_newnode_cai(pcai head,char *name,char *price);
//购物车
caa crivte_newnode_jie(caa head,char *price,int prices);
caa crivte_haed_jie();
//尾部添加
void add_tail_cai(pcai head,pcai newnode);
//1/2显示
int show_2(int x,int y,const char *pathname);

//打印
void show(peo head);
//滑动显示图片
void move_bmp();
//判断选择
int touch1(peo head,peo newnode,caa heads,caa newnodes);
int get_xy_lu(struct inputxy *point);

/* ==== */
int touch2(peo head,peo newnode);
//打印链表
void show(peo head);
//检测数组
void show2(int a[],int b[],int c,int d);

//尾部添加
void add_tail_jie(caa head,caa newnode);




void show_jie(caa head);

//点菜
int get_xy_cai(struct inputxy *point);







#endif