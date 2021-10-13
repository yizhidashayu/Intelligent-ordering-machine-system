#include "lcd.h"

peo crivte_haed()
{
    peo head=malloc(sizeof(people));
    if(head == NULL)
    {
        perror("申请失败\n");
        return NULL;
    }
    head->next=head;
    head->prev=head;
    return head;
}

peo crivte_newnode(peo head,int zhang[],int mi[],int a,int b)
{
    int i;
    peo newnode=malloc(sizeof(people));
    if(newnode == NULL)
    {
        perror("申请失败\n");
        return NULL;
    }
    newnode->next=newnode;
    newnode->prev=newnode;
    for (int i = 0; i < a; i++)
    {
        newnode->zhang[i]=zhang[i];
    }
    
    for(i=0;i<b;i++)
    {
        newnode->mi[i]=mi[i];
    }

    return newnode;
}

//尾部添加
void add_tail(peo head,peo newnode)
{
    peo p=head;
    while(p->prev != head)
    {
        p=p->next;
    }
    newnode->next=head;
    newnode->prev=p;
    head->prev=newnode;
    p->next=newnode;
}


//打印
void show(peo head)
{
	peo p=head;
	while(p->next!=head)
	{
		p=p->next;
		for(int i=0;i<8;i++)
		{
			printf("%d-",p->zhang[i]);
		}
	for(int i=0;i<6;i++)
		{
			printf("%d-",p->mi[i]);
		} 
	}
}


//菜单
pcai crivte_haed_cai()
{
    pcai head=malloc(sizeof(cai));
    if(head == NULL)
    {
        perror("申请失败\n");
        return NULL;
    }
    head->next=head;
    head->prev=head;
    return head;
}

pcai crivte_newnode_cai(pcai head,char *name,char *price)
{
   pcai newnode=malloc(sizeof(cai));
	if(newnode==NULL)
	{
		printf("申请失败\n");
		return NULL;
	}
	newnode->next=newnode;
	newnode->prev=newnode;
	newnode->name=name;
	newnode->price=price;
	return newnode;
}

//尾部添加
void add_tail_cai(pcai head,pcai newnode)
{
    pcai p=head;
    while(p->prev != head)
    {
        p=p->next;
    }
    newnode->next=head;
    newnode->prev=p;
    head->prev=newnode;
    p->next=newnode;
}

//d打印数组
void show2(int a[],int b[],int c,int d)
{
	for(int i=0;i<c;i++)
	{
		printf("%d-",a[i]);
	}
	for(int i=0;i<d;i++)
	{
		printf("%d-",b[i]);
	}
}



//结算
caa crivte_haed_jie()
{
    caa head = malloc(sizeof(jie));
    if(head == NULL)
    {
        perror("申请失败\n");
        return NULL;
    }
     head->next=head;
    head->prev=head;
    return head;
}


//购物车
caa crivte_newnode_jie(caa head,char *price,int prices)
{
    caa newnode=malloc(sizeof(jie));
    if(newnode == NULL)
    {
        perror("申请失败\n");
        return NULL;
    }
    newnode->next=newnode;
    newnode->prev=newnode;
    newnode->price=price;
    newnode->prices=prices;
    return newnode;
}



//尾部添加
void add_tail_jie(caa head,caa newnode)
{
    caa p=head;
    while(p->prev != head)
    {
        p=p->next;
    }
    newnode->next=head;
    newnode->prev=p;
    head->prev=newnode;
    p->next=newnode;
}



void show_jie(caa head)
{
    show_bmp(0,0,"beijing.bmp");
	caa p=head;
	int i=0,j=0;
	while(p->next!=head)
	{
		p=p->next;
		show_bmp(i,j,p->price);
		i+=450;
		if(i>=800)
		{
			i=0;
			j+=50;
		}
	}
}
