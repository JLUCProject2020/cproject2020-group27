#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
typedef struct MENU{		//����˵�ѡ���Ϸģʽ�� 
	int player;				//����� ��1���˻����� 2��˫�˶��� -1���ɴ浵������ 
	int forbidden_moves;	//�Ƿ���� (1:�� 0����)
	int first;				//�Ⱥ��� (1:���� 0������)
	int level;				//�Ѷ� ��1���� 2�����ѣ� 
} menu;
//����������Ϣ�ṹ�� 
typedef struct  t_point{
	int color;		//1:black	0:white
	int number;
} point;
typedef struct t_board{
	menu gamemode;				//�Ծ�ģʽ 
	int number;					//�������� 
	point everypoint[15][15];	//ÿ�����ӵ���Ϣ 
} board;
typedef struct t_value_of_point{
	int x;
	int y;
	int number;
	int number_attack;
	int number_defence; 
	int random_number;
	int value;
	int he_can_win;
	bool forbidden_moves;
	bool his_forbidden_moves;
	int count_of_his_livefour;
	int count_of_otherfour;
	int count_of_his_otherfour;
	int count_of_livethree_1;
	int count_of_his_livethree_1;
	int count_of_livethree_2;
	int count_of_his_livethree_2;
	int count_of_sleepthree;
	int count_of_his_sleepthree;
	int count_of_livetwo_true;
	int count_of_his_livetwo_true;
	int count_of_livetwo_false;
	int count_of_his_livetwo_false;
} value_of_point;

PIMAGE img_board,img_black,img_white;	//�������̣����ӣ����ӵ�ͼ���ȫ�ֱ��� 
int end=0; 				//�ж���Ϸ�Ƿ���� 
board board1;			//�����������洢���ڽ��е������Ϣ 

void click(int *px,int *py){//��������Ϣֱ�������������������������꣨x��y�� 
	mouse_msg msg = {0};
	int x=-100,y=-100,x1=-200,y1=-200;						//��ʼ��	��ֹx��y�ĳ�ʼֵ��x1��y1�ĳ�ʼֵ���; 
	bool f;
	f=is_run();
	for ( ; f; delay_fps(60)){
		while (mousemsg())									//��ȡ�����Ϣ���ȴ�ֱ������ϢΪֹ 
		{
			msg = getmouse();
		}
		if((int)msg.is_down()==1&&(int)msg.is_left()==1){	//��¼�����갴��λ�� 
			x=msg.x;y=msg.y;
		}
		if((int)msg.is_down()==0&&(int)msg.is_left()==1){	//��¼������̧��λ�� 
			x1=msg.x;y1=msg.y;
		}
		if((x1-x)*(x1-x)<100&&(y1-y)*(y1-y)<100){			//������º�̧��λ�����С��10����ȷ��Ϊ���� 
			f=false;										//��ֹͣ��ȡ�����Ϣ 
		}else	f=true;
	}
	*px=x;*py=y;											//��������������λ�� 
}

void material(){	//��ȡ�زģ�����ֵ������PIMAGE����ȫ�ֱ����� 
	PIMAGE pimg;
	pimg=newimage();
	getimage(pimg,"material\\board.jpg");
	img_board=newimage(600,600);
	putimage(img_board,0,0,600,600,pimg,0,0,getwidth(pimg),getheight(pimg));
	delimage(pimg);
	pimg=newimage();
	getimage(pimg,"material\\black.png");
	img_black=newimage(40,40);
	putimage(img_black,0,0,40,40,pimg,0,0,getwidth(pimg),getheight(pimg));
	delimage(pimg);
	pimg=newimage();
	getimage(pimg,"material\\white.jpg");
	img_white=newimage(40,40);
	putimage(img_white,0,0,40,40,pimg,0,0,getwidth(pimg),getheight(pimg));
	delimage(pimg);
}

void print_board(){
	setfillcolor(WHITE);
	bar(600,0,750,600);					//����Ҳ� 
	putimage(0,0,img_board);			// ������ӡ���� 
}

void print_game(){			//��ӡ���(��ʼ��ֺ��Ҳ����)�������Ҳ�������Ϣ 
	print_board();
	//����������Ϣ 
	setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	setfont(22, 0, "����");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	
}

void beginning(menu *pmenu){	//��ӡ��ʼ���� 
	print_game();
	if(pmenu->player==0){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "˫�˶���");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "�˻�����");
		bar(630, 180, 720, 210);
		outtextxy(675, 195, "��ȡ��¼");
		bar(630, 240, 720, 270);
		outtextxy(675, 255, "���ֹ���");
		bar(630, 300, 720, 330);
		outtextxy(675, 315, "������Ϸ");
	}
	if(pmenu->player==2){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "�н���");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "�޽���");
	}
	if(pmenu->player==1){
		if(pmenu->forbidden_moves==-1){
			bar(630, 60, 720, 90);
			outtextxy(675, 75, "�н���");
			bar(630, 120, 720, 150);
			outtextxy(675, 135, "�޽���");
		}else{
			if(pmenu->first==-1){
				bar(630, 60, 720, 90);
				outtextxy(675, 75, "����");
				bar(630, 120, 720, 150);
				outtextxy(675, 135, "����");
			}else{
				bar(630, 60, 720, 90);
				outtextxy(675, 75, "��");
				bar(630, 120, 720, 150);
				outtextxy(675, 135, "����");
			}
		}
	}
	if(pmenu->player==-1){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "�浵1");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "�浵2");
		bar(630, 180, 720, 210);
		outtextxy(675, 195, "�浵3");
		bar(630, 240, 720, 270);
		outtextxy(675, 255, "�浵4");
		bar(630, 300, 720, 330);
		outtextxy(675, 315, "�浵5");
		bar(630, 360, 720, 390);
		outtextxy(675, 375, "�浵6");
	}
}

void put_board(	int a,int b,board *pboard){//�������ڣ�a��b���� ������board1����Ϣ
	if(pboard->everypoint[a][b].color==-1){					//�����a��b��û���� 
		pboard->number=board1.number+1;						//������+1 
		pboard->everypoint[a][b].number=pboard->number;		//��¼���
		pboard->everypoint[a][b].color=(pboard->number)%2;	//��¼��ɫ (1����   0����)
		if((pboard->number)%2==1)
			putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*a,(25-(float)550/28)+((float)550/14)*b,BLACK);	//������ӡ����
		else
			putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*a,(25-(float)550/28)+((float)550/14)*b,0xF6F6F6);//˫����ӡ���� 
	}
}




int length(	int a,int b,board *pboard,int mode,		//��(a,b)�����鳤��
			int *frontx=NULL,int *fronty=NULL,		//����������(��������)���� 
			int *rearx=NULL,int *reary=NULL){
	//�������� 
	int colour=pboard->everypoint[a][b].color;		//�����������ɫ 
	int px,py,qx,qy; 								//ָ�� 
	bool f;											//�ж��Ƿ�ֹͣ
	int l;											//��¼���鳤�� 
	int x,y;										//��¼�������� 
	switch(mode){
		case 0:x=0;y=1;break;			//���ϵ��� 
		case 1:x=1;y=1;break;			//�����ϵ����� 
		case 2:x=1;y=0;break;			//������ 
		case 3:x=1;y=-1;break;			//�����µ����� 
	}
	px=a;py=b;qx=px-x;qy=py-y;f=true;l=0;//��ʼ���������� 
	//�ƶ�ָ�뵽���϶� 
	while(f){			
		if(qx>14||qx<0||qy>14||qy<0)
			f=false;	//��������߽���� 
		else
			if(pboard->everypoint[qx][qy].color!=colour)
				f=false;//���û���ӻ���ɫ��ͬ����
			else{
				qx=qx-x;qy=qy-y;px=px-x;py=py-y;//�ƶ�ָ�� 
			}
	}
	qx=px;qy=py;			//����ʱpx��pyӦָ��һ�� 
	//��ⳤ��l
	while(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==colour) {//����ɫ��ͬ��û�г���ʱ
		 qx=qx+x;qy=qy+y;	//��qx��qy������һ�� (�����������һ��)
		 l++;
	}
	qx=qx-x;qy=qy-y;		//��λ 
	//������������ 
	if(frontx!=NULL&&fronty!=NULL){
		*frontx=px;*fronty=py;
	}
	if(rearx!=NULL&&reary!=NULL){
		*rearx=qx;*reary=qy;
	}
	return l;				//�������鳤��l 
}

int five(int a,int b,board *pboard){	//��a��b���ĸ������γ�"5"�ĸ��� 
	int i=0,l=0;
	int n=0;		//��¼4������ �γ�"5"�ĸ��� 
	for(i=0;i<4;i++){
		 l=length(a,b,pboard,i);
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//�н��� 
		 	if(l==5) n++;
		 }else{																		//�޽��� 
		 	if(l>=5) n++;
		 }
	}
	return n;
}

int overfive(int a,int b,board *pboard){	//��a��b���ĸ������γɳ����ĸ��� 
	int i=0,l=0;
	int n=0;		//��¼4������ �γɳ����ĸ��� 
	for(i=0;i<4;i++){
		 l=length(a,b,pboard,i);
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1)//�н��� 
		 	if(l>5) n++;
	}
	return n;
}

int is_four(int a,int b,board *pboard,int mode,
			/*�ж�mode�����Ƿ���4�����ķ���2���������꣬���ķ����ܳ�5������,û��4�򷵻�0,�����γ����������򷵻�3*/ 
			int *px1=NULL,int *py1=NULL,int *px2=NULL,int *py2=NULL){//�����ܳ�5��������������귵��Ϊ-1
	int px,py,qx,qy;//����ָ�룬��־�������� 
	int n=0;			//�ж�"4"���� 
	length(a,b,pboard,mode,&px,&py,&qx,&qy);		//���������� 
	int x,y;										//��¼�������� 
	switch(mode){
		case 0:x=0;y=1;break;			//���ϵ��� 
		case 1:x=1;y=1;break;			//�����ϵ����� 
		case 2:x=1;y=0;break;			//������ 
		case 3:x=1;y=-1;break;			//�����µ����� 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//��ʼ���������� (����ָ��������һ��)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//���������������Ϣ 
	//��һ����� (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//��board1�����һ������ͬ��ɫ���� 
		if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//�н��� 
		 	if(length(a,b,&board_1,mode)==5){							//�ж��Ƿ��γ�5,���γ�n+1 
				n++;
				if(px1!=NULL&&py1!=NULL){								//���γ�5����һ������ 
					*px1=px;*py1=py;
				}
			}else{
				if(px1!=NULL&&py1!=NULL){								//��û���γ�5����-1 
					*px1=-1;*py1=-1;
				}
			}
		 }else{																		//�޽��� 
		 	if(length(a,b,&board_1,mode)>=5){							//�ж��Ƿ��γ�5,���γ�n+1 
				n++;
				if(px1!=NULL&&py1!=NULL){								//���γ�5����һ������ 
					*px1=px;*py1=py;
				}
			}else{
				if(px1!=NULL&&py1!=NULL){								//��û���γ�5����-1 
					*px1=-1;*py1=-1;
				}
			}
		 }
		
	}
	else{
		if(px1!=NULL&&py1!=NULL){								//���ڽ����õ����ӷ���-1 
			*px1=-1;*py1=-1;
		}
	}
	//�ڶ������(board_2) 
	if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//��board2�����һ������ͬ��ɫ���� 
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//�н��� 
			if(length(a,b,&board_2,mode)==5){							//�ж��Ƿ��γ�5,���γ�n+1 
				n++;
				if(px2!=NULL&&py2!=NULL){								//���γ�5����һ������ 
					*px2=qx;*py2=qy;
				}
			}else{
				if(px2!=NULL&&py2!=NULL){								//��û���γ�5����-1 
					*px2=-1;*py2=-1;
				}
			}
		 }else{																		//�޽��� 
			if(length(a,b,&board_2,mode)>=5){							//�ж��Ƿ��γ�5,���γ�n+1 
				n++;
				if(px2!=NULL&&py2!=NULL){								//���γ�5����һ������ 
					*px2=qx;*py2=qy;
				}
			}else{
				if(px2!=NULL&&py2!=NULL){								//��û���γ�5����-1 
					*px2=-1;*py2=-1;
				}
			}
		 }
	}else{
		if(px2!=NULL&&py2!=NULL){								//���ڽ����õ����ӷ���-1 
			*px2=-1;*py2=-1;
		}
	}
	if(n==2)															//������� 
		if(length(a,b,pboard,mode)<4)
			n++;
	return n;
}

void four(	int a,int b,board *pboard,			//��a��b���ĸ�������ĺͳ��ĵĸ��� 
			int *plivefour,int *potherfour){
	int i=0;//��־���� 
	int kind_four=0;//��־4������ 
	for(i=0;i<4;i++){
		kind_four=is_four(a,b,pboard,i);
		if(kind_four==2)
			*plivefour=*plivefour+1;
		if(kind_four==1)
			*potherfour=*potherfour+1;	
		if(kind_four==3)
			*potherfour=*potherfour+2;	
	} 
}

void is_three(	int a,int b,board *pboard,int mode,							//�жϣ�a��b��mode�����Ƿ�Ϊ"3" 
				int *p1,int *p2,											//�������ͣ�0���ޣ�1�����ģ�2�����ģ� 
				int *px1=NULL,int *py1=NULL,int *px2=NULL,int *py2=NULL){	//�������꣨��Ӧ��p1��p2�������򷵻�-1 
	if(is_four(a,b,pboard,mode)!=0){				//���ж��ǲ���4������Ƿ���0��0���������κ����͵����� 
		*p1=0;*p2=0;
		return;										//�˳����� 
	}
	int px,py,qx,qy;//����ָ�룬��־�������� 
	int n1=0,n2=0;			//�ж�����"3"���� 
	length(a,b,pboard,mode,&px,&py,&qx,&qy);		//���������� 
	int x,y;										//��¼�������� 
	switch(mode){
		case 0:x=0;y=1;break;			//���ϵ��� 
		case 1:x=1;y=1;break;			//�����ϵ����� 
		case 2:x=1;y=0;break;			//������ 
		case 3:x=1;y=-1;break;			//�����µ����� 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//��ʼ���������� (����ָ��������һ��)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//���������������Ϣ 
	//��һ����� (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//��board1�����һ������ͬ��ɫ���� 
		n1=is_four(a,b,&board_1,mode); 			//�ж�board1���������"4"�����͸�ֵ��n1��0���ޣ�1�����ģ�2�����ģ�
		if(n1==3) n1=1;							//���γ��������İ�1�� 
	}
	//�ڶ������(board_2)
	 if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//��board2�����һ������ͬ��ɫ���� 
		n2=is_four(a,b,&board_2,mode);			//�ж�board2���������"4"�����͸�ֵ��n2��0���ޣ�1�����ģ�2�����ģ�
		if(n2==3) n2=1;							//���γ��������İ�1��
	}
	if(px1!=NULL&&py1!=NULL){						//����p1�����ꣻ 
		if(px>=0&&px<15&&py>=0&&py<15){		//���û���� 
			*px1=px;*py1=py;				//�������� 
		}else{
			*px1=-1;*py1=-1;				//����-1 
		}
	}
	if(px2!=NULL&&py2!=NULL){						//����p2�����ꣻ 
		if(qx>=0&&qx<15&&qy>=0&&qy<15){		//���û���� 
			*px2=qx;*py2=qy;				//�������� 
		}else{
			*px2=-1;*py2=-1;				//����-1 
		}
	}
	*p1=n1;*p2=n2;
}

void three(	int a,int b,board *pboard,				//��a��b���ĸ���������������ĸ��� 
			int *plivethree,int *psleepthree){
				int i=0;						//��־���� 
				int kthree1=0,kthree2=0;	//��־�������ࣨ�����Ƿ��γ�4�� 
				for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if(kthree1==2||kthree2==2)//�л���Ϊ���� 
						*plivethree=*plivethree+1;
					else if(kthree1==1||kthree2==1)//û�л����г���Ϊ���� 
							*psleepthree=*psleepthree+1;
				}
}

int livethree_1(int a,int b,board *pboard){
	int i;//��־����
	int kthree1=0,kthree2=0; 	//��־�������ࣨ�����Ƿ��γ�4��
	int kind_of_livethree_1=0; 
	for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if(kthree1==2&&kthree2==2)//���˶��ܳɻ�����Ϊ����-1 
						kind_of_livethree_1++;
	}
	return kind_of_livethree_1;
}

int livethree_2(int a,int b,board *pboard){
	int i;//��־����
	int kthree1=0,kthree2=0; 	//��־�������ࣨ�����Ƿ��γ�4��
	int kind_of_livethree_2=0; 
	for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if((kthree1==2&&kthree2!=2)||(kthree1!=2&&kthree2==2))
						kind_of_livethree_2++;
	}
	return kind_of_livethree_2;	
}

int is_livetwo(int a,int b,board *pboard,int mode){//����ֵ��0�����ǻ��	1���ٻ��	2������ 
	if(is_four(a,b,pboard,mode)!=0){				//���ж��ǲ���4������Ƿ���0���������κ����͵Ķ��� 
		return 0;										//�˳����� 
	}
	int p1,p2;
	is_three(a,b,pboard,mode,&p1,&p2);
	if(p1!=0||p2!=0){								//�ж��ǲ������� ����Ƿ���0 
		return 0;
	}
	int px,py,qx,qy;//����ָ�룬��־�������� 
	bool can_livethree=false;//�ж��������º��ܷ��γɻ��� 
	int l;
	l=length(a,b,pboard,mode,&px,&py,&qx,&qy);		//���������� 
	int x,y;										//��¼�������� 
	switch(mode){
		case 0:x=0;y=1;break;			//���ϵ��� 
		case 1:x=1;y=1;break;			//�����ϵ����� 
		case 2:x=1;y=0;break;			//������ 
		case 3:x=1;y=-1;break;			//�����µ����� 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//��ʼ���������� (����ָ��������һ��)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//���������������Ϣ 
	//��һ����� (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//��board1�����һ������ͬ��ɫ���� 
		is_three(a,b,&board_1,mode,&p1,&p2);			//�ж�board1���������"3"�����͸�ֵ��p1,p2
		if(p1==2||p2==2){//������κ����͵Ļ��� 
			can_livethree=true;
		} 
	}
	//�ڶ������(board_2)
	 if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//����˵��������ڲ���Ϊû���� 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//��board2�����һ������ͬ��ɫ���� 
		is_three(a,b,&board_2,mode,&p1,&p2);
		if(p1==2||p2==2){
			can_livethree=true;
		}
	}
	if(can_livethree){
		if(l==2){
			return 2;
		}else return 1;
	}
	return 0;
}

int livetwo_true(int a,int b,board *pboard){
	int i;//��־����
	int k; 	//��־�������� 
	int n=0;//��־�����ĸ��� 
	for(i=0;i<4;i++){
		k=is_livetwo(a,b,pboard,i);
		if(k==2)
			n++;	
	}
	return n;
}

int livetwo_false(int a,int b,board *pboard){
	int i;//��־����
	int k; 	//��־�������� 
	int n=0;//��־�ٻ���ĸ��� 
	for(i=0;i<4;i++){
		k=is_livetwo(a,b,pboard,i);
		if(k==1)
			n++;	
	}
	return n;
}

int can_form_overfive(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γɳ��������س����ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_overfive=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_overfive=overfive(a,b,&board_1);
	}
	return count_of_overfive;
}

int can_form_five(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γ��岢������ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_five=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_five=five(a,b,&board_1);
	}
	return count_of_five;
}

int can_form_four(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γ��Ĳ����ػ���+���ĵĸ��� 
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_livefour+count_of_otherfour;
	return n;
}

int can_form_livefour(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γɻ��Ĳ����ػ��ĵĸ��� 
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_livefour;
	return n;
}

int can_form_otherfour(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γɳ��Ĳ����س��ĵĸ��� 
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_otherfour;
	return n;
}

int can_form_livethree(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γ��������ػ����ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		three(a,b,&board_1,&count_of_livethree,&count_of_sleepthree);
	}
	return count_of_livethree;
}

int can_form_sleepthree(int a,int b,board *pboard){//�ж����¼������Ӻ��ܷ��γ����������������ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		three(a,b,&board_1,&count_of_livethree,&count_of_sleepthree);
	}
	return count_of_sleepthree;
}

int can_form_livethree_1(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livethree_1=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livethree_1=livethree_1(a,b,&board_1);
	}
	return count_of_livethree_1;
}

int can_form_livethree_2(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livethree_2=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livethree_2=livethree_2(a,b,&board_1);
	}
	return count_of_livethree_2;
}

int can_form_livetwo_true(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livetwo_true=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livetwo_true=livetwo_true(a,b,&board_1);
	}
	return count_of_livetwo_true;
}

int can_form_livetwo_false(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livetwo_false=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.number++;
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livetwo_false=livetwo_false(a,b,&board_1);
	}
	return count_of_livetwo_false;
}

int can_he_form_overfive(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γɳ��������س����ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_overfive=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_overfive=overfive(a,b,&board_1);
	}
	return count_of_overfive;
}

int can_he_form_five(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γ��岢������ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_five=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_five=five(a,b,&board_1);
	}
	return count_of_five;
}

int can_he_form_four(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γ��Ĳ����ػ���+���ĵĸ��� 
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_livefour+count_of_otherfour;
	return n;
}

int can_he_form_livefour(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γɻ��Ĳ����ػ��ĵĸ���
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_livefour;
	return n;
}

int can_he_form_otherfour(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γɳ��Ĳ����س��ĵĸ���
	int n=0;
	board board_1;
	board_1=*pboard;
	int count_of_livefour=0;
	int count_of_otherfour=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		four(a,b,&board_1,&count_of_livefour,&count_of_otherfour);
	}
	n=count_of_otherfour;
	return n;
}

int can_he_form_livethree(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γ��������ػ����ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		three(a,b,&board_1,&count_of_livethree,&count_of_sleepthree);
	}
	return count_of_livethree;
}

int can_he_form_sleepthree(int a,int b,board *pboard){//�ж����¶Է����Ӻ��ܷ��γ����������������ĸ���
	board board_1;
	board_1=*pboard;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		three(a,b,&board_1,&count_of_livethree,&count_of_sleepthree);
	}
	return count_of_sleepthree;
}

int can_he_form_livethree_1(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livethree_1=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livethree_1=livethree_1(a,b,&board_1);
	}
	return count_of_livethree_1;
}

int can_he_form_livethree_2(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livethree_2=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livethree_2=livethree_2(a,b,&board_1);
	}
	return count_of_livethree_2;
}

int can_he_form_livetwo_true(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livetwo_true=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livetwo_true=livetwo_true(a,b,&board_1);
	}
	return count_of_livetwo_true;
}

int can_he_form_livetwo_false(int a,int b,board *pboard){
	board board_1;
	board_1=*pboard;
	int count_of_livetwo_false=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_livetwo_false=livetwo_false(a,b,&board_1);
	}
	return count_of_livetwo_false;
}

bool can_form_forbidden_moves(int a,int b,board *pboard){
	bool g=false;
	if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==0){
		int count_of_overfive=can_form_overfive(a,b,pboard);
		int count_of_five=can_form_five(a,b,pboard);
		int count_of_four=can_form_four(a,b,pboard);
		int count_of_livethree=can_form_livethree(a,b,pboard);
		if(count_of_five>0){
			g=false;
		}else{
			if(count_of_four>1||count_of_livethree>1||count_of_overfive>0){
				g=true;
			}
		}
	}else g=false;
	return g;
}

bool can_he_form_forbidden_moves(int a,int b,board *pboard){
	bool g=false;
	if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==1){
		int count_of_overfive=can_he_form_overfive(a,b,pboard);
		int count_of_five=can_he_form_five(a,b,pboard);
		int count_of_four=can_he_form_four(a,b,pboard);
		int count_of_livethree=can_he_form_livethree(a,b,pboard);
		if(count_of_five>0){
			g=false;
		}else
			if(count_of_four>1||count_of_livethree>1||count_of_overfive>0)
				g=true;
	}else g=false;
	return g;
}

int sure_to_win(board *pboard,int *a=NULL,int *b=NULL){
	int i,j;
	int pa=-1,pb=-1;
	int f=0;
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			f=can_form_five(i,j,pboard);
			if(f!=0){
				pa=i;pb=j;
			}
		}
	if(a!=NULL&&b!=NULL){
		*a=pa;*b=pb;
	}
	if(pa==-1)
		return 0;	//һ������ʤ����0 
	else
		return 1;	//һ����ʤ����1 
}

int steps_to_win(board *pboard,int n,int *a=NULL,int *b=NULL){
	if(n==1){
		int m; 
		m=sure_to_win(pboard,a,b);
		return m;
	}else{
		board board_1,board_2;
		int f,g,h,x,y;
		bool forbidden_moves,forbidden_moves_2;
		bool can_go=false;//�жϼ��������Ӻ�Է��ܷ����� 
		int i,j,k,l;
		int pa=-1,pb=-1;
		f=sure_to_win(pboard,a,b);
		if(f==1){			//��������ʤ�������� 
			//printf("!\n!\n!\n%d:(%d,%d)1����ʤ\n",n,*a,*b);
			return 1;
		}			//����������
		for(i=0;i<15;i++)	//�ж϶Է��Ƿ����ģ����Է��ܷ���壩 �����ܳ���������� 
			for(j=0;j<15;j++){
					f=can_he_form_five(i,j,pboard);
					if(f!=0){
						pa=i;pb=j; 
						if(a!=NULL&&b!=NULL){
							*a=pa;*b=pb;
						}
						//printf("%d:�Է�����\n",n);
						return 0;
					}
			}
		for(i=0;i<15;i++)		//�жϼ����Ƿ��л��������ܳɻ��ģ�����������õ㲢���ظõ����� 
			for(j=0;j<15;j++){
				f=can_form_livefour(i,j,pboard);
				if(f!=0){
					if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==0){//����Ǻ��壬�н��� 
						g=can_form_otherfour(i,j,pboard);
						h=can_form_livethree(i,j,pboard);
						x=can_form_overfive(i,j,pboard);
						if((f+g)<2&&h<2&&x==0){//������º��γ��ĵ����ǽ���,�򷵻����� 
							pa=i;pb=j;
							if(a!=NULL&&b!=NULL){
								*a=pa;*b=pb;
							}
							//printf("!\n!\n!\n%d:(%d,%d)������ʤ\n",n,pa,pb);
							return 1;
						}
					}else{
						pa=i;pb=j;
						if(a!=NULL&&b!=NULL){
						*a=pa;*b=pb;
						}
						//printf("!\n!\n!\n%d:(%d,%d)������ʤ\n",n,pa,pb);
						return 1;
					} 
				}
			}
		for(i=0;i<15;i++)		//�ж϶Է��ܷ��γɻ��� 
			for(j=0;j<15;j++){
				f=can_he_form_livefour(i,j,pboard);
				if(f!=0){
					if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==1){//����Ǻ��壬�н��� 
						g=can_he_form_otherfour(i,j,pboard);
						h=can_he_form_livethree(i,j,pboard);
						x=can_he_form_overfive(i,j,pboard);
						if((f+g)<2&&h<2&&x==0){//������º��γ��ĵ����ǽ���,��������� 
							if(a!=NULL&&b!=NULL){
								*a=pa;*b=pb;
							}
							//printf("%d:�Է��л���������û�л���,˫����û���ģ�\n",n);
							return 0;
						}
					}else{ 
						if(a!=NULL&&b!=NULL){
						*a=pa;*b=pb;
						}
						//printf("%d:�Է��л�����˫����û���ģ�\n",n);
						return 0;
					} 
				}
			}
		for(i=0;i<15;i++)								//�������е㣬�ж��ܷ��γɻ�������� 
			for(j=0;j<15;j++){
				 forbidden_moves=can_form_forbidden_moves(i,j,pboard);
				 if(!forbidden_moves){//������º�û���γɽ���
				 	f=can_form_livethree(i,j,pboard);
					g=can_form_otherfour(i,j,pboard);
					if(f>0||g>0){//��������γɻ��������
						//�伺����
						board_1=*pboard;
						board_1.number++;
						board_1.everypoint[i][j].color=board_1.number%2;
						board_1.everypoint[i][j].number=board_1.number;
						//��Է���
						if(g>0){//�����������ǳ��ĵ� 
							for(k=0;k<15;k++)			//�ж�ÿ���㼺���Ƿ���� 
								for(l=0;l<15;l++){
									h=can_he_form_five(k,l,&board_1);
									if(h>0){//��������ܳ��� 
										//����Է��ǽ���
										forbidden_moves_2=can_form_forbidden_moves(k,l,&board_1); 
										if(forbidden_moves_2){
											pa=i;pb=j;
											if(a!=NULL&&b!=NULL){
												*a=pa;*b=pb;
											}
											//printf("!\n!\n!\n%d:(%d,%d)���ıƽ���\n",n,pa,pb);
											return 1;
										}
										//��Է��� 
										board_2=board_1;
										board_2.number++;
										board_2.everypoint[k][l].color=board_2.number%2;
										board_2.everypoint[k][l].number=board_2.number;
										//�ж�n-1���Ƿ��ʤ
										x=steps_to_win(&board_2,n-1);
										if(x==1){//���n-1����ʤ�����أ�i��j�� 
											pa=i;pb=j;
											if(a!=NULL&&b!=NULL){
												*a=pa;*b=pb;
											}
											//printf("!\n!\n!\n%d:(%d,%d)���Ļ�ʤ(���n=2����˫�Ļ�ʤ)\n",n,pa,pb);
											return 1;
										} 
									}
								} 
							//printf("%d:�м������ĵ�(%d��%d)���䣬�����ܱ�ʤ\n",n,i,j);
						}else{//������º�û���γɳ��ĵ��γ��˻��� 
							can_go=false;
							for(k=0;k<15;k++)//�ж�ÿ���㼺���ܷ�ɻ��Ļ�Է��ܷ�ɳ��� 
								for(l=0;l<15;l++){
									h=can_he_form_livefour(k,l,&board_1);
									x=can_form_otherfour(k,l,&board_1);
									if(h>0||x>0){//��������ܳɻ��Ļ�Է��ܳɳ��� 
										forbidden_moves_2=can_form_forbidden_moves(k,l,&board_1); 
										if(!forbidden_moves_2){//����Է����ǽ��� 
											can_go=true;//�Է��п����
											//��Է��� 
											board_2=board_1;
											board_2.number++;
											board_2.everypoint[k][l].color=board_2.number%2;
											board_2.everypoint[k][l].number=board_2.number;
											//�ж�n-���Ƿ��ʤ 
											y=steps_to_win(&board_2,n-1);
											if(y==1){//���n-1����ʤ 
												pa=i;pb=j;
												if(a!=NULL&&b!=NULL){
													*a=pa;*b=pb;
												}
												//printf("!\n!\n!\n%d:(%d,%d)���µ����ɷ��Ļ���\n",n,i,j);
												return 1;
											}
										}
									}
								}
								if(!can_go){
									pa=i;pb=i;
									if(a!=NULL&&b!=NULL){
										*a=pa;*b=pb;
									}
									//printf("!\n!\n!\n%d:(%d,%d)�����²��ɷ��Ļ���\n",n,i,j);
									return 1;
								}	
						}
					} 
				 }
			}
		if(a!=NULL&&b!=NULL){
			*a=-1;*b=-1;
		}
		//printf("%d:δ�ҵ���ʤ��\n",n);
		return 0;	
	}
}

void give_value(value_of_point *p,board *pboard){
	int a,b;
	a=p->x;
	b=p->y;
	p->forbidden_moves=can_form_forbidden_moves(a,b,pboard);
	p->his_forbidden_moves=can_he_form_forbidden_moves(a,b,pboard);
	p->count_of_his_livefour=can_he_form_livefour(a,b,pboard);
	p->count_of_otherfour=can_form_otherfour(a,b,pboard);
	p->count_of_his_otherfour=can_he_form_otherfour(a,b,pboard);
	p->count_of_livethree_1=can_form_livethree_1(a,b,pboard);
	p->count_of_his_livethree_1=can_he_form_livethree_1(a,b,pboard);
	p->count_of_livethree_2=can_form_livethree_2(a,b,pboard);
	p->count_of_his_livethree_2=can_he_form_livethree_2(a,b,pboard);
	p->count_of_sleepthree=can_form_sleepthree(a,b,pboard);
	p->count_of_his_sleepthree=can_he_form_sleepthree(a,b,pboard);
	p->count_of_livetwo_true=can_form_livetwo_true(a,b,pboard);
	p->count_of_his_livetwo_true=can_he_form_livetwo_true(a,b,pboard);
	p->count_of_livetwo_false=can_form_livetwo_false(a,b,pboard);
	p->count_of_his_livetwo_false=can_he_form_livetwo_false(a,b,pboard);
}

void best_point(board *pboard,int *a,int *b){
	srand(time(NULL));
	int random;
	int i,j,r;
	value_of_point value_of_everypoint[15][15];
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			value_of_everypoint[i][j].x=i;
			value_of_everypoint[i][j].y=j;
			value_of_everypoint[i][j].number=0;
			value_of_everypoint[i][j].number_attack=0;
			value_of_everypoint[i][j].number_defence=0;
			value_of_everypoint[i][j].random_number=0;
			value_of_everypoint[i][j].value=0;
			value_of_everypoint[i][j].he_can_win=0;
			value_of_everypoint[i][j].his_forbidden_moves=false;
			value_of_everypoint[i][j].forbidden_moves=false;
			value_of_everypoint[i][j].count_of_his_livefour=0;
			value_of_everypoint[i][j].count_of_his_livethree_1=0;
			value_of_everypoint[i][j].count_of_his_livethree_2=0;
			value_of_everypoint[i][j].count_of_his_livetwo_false=0;
			value_of_everypoint[i][j].count_of_his_livetwo_true=0;
			value_of_everypoint[i][j].count_of_his_otherfour=0;
			value_of_everypoint[i][j].count_of_his_sleepthree=0;
			value_of_everypoint[i][j].count_of_livethree_1=0;
			value_of_everypoint[i][j].count_of_livethree_2=0;
			value_of_everypoint[i][j].count_of_livetwo_false=0;
			value_of_everypoint[i][j].count_of_livetwo_true=0;
			value_of_everypoint[i][j].count_of_otherfour=0;
			value_of_everypoint[i][j].count_of_sleepthree=0;
		} 
	//��ֵ��ʼ�� 
	if(pboard->number==0){
		*a=7;*b=7;
		return;
	}
	if(pboard->number==1){
		int x1,y1;
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				if(pboard->everypoint[i][j].number==1){
					x1=i;y1=j;
				}
			}
		if(x1==0||y1==0||x1==14||y1==14){
			*a=7;*b=7;
			return;
		}else{
			for(i=0;i<15;i++)
				for(j=0;j<15;j++){
					if(can_he_form_livetwo_true(i,j,pboard)>0){
						value_of_everypoint[i][j].number=1;
					}
				}
			r=0;
			for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].number==1){
						r++;
						value_of_everypoint[i][j].random_number=r;
					}
				}
			random=rand();
			r=random%r+1;//��r��һ�������
			for(i=0;i<15;i++)//����������������� 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].random_number==r){
						*a=i;*b=j;
					}
				}
			return; 
		}
	}
	if(pboard->number>=6){//��������������������ڵ���6 
		if(pboard->gamemode.level==2){
			steps_to_win(pboard,7,a,b);//�ж�13�����Ƿ��ʤ
			if(*a!=-1)//�����ʤ�򷵻����겢�������� 
				return;
		}else{
			steps_to_win(pboard,3,a,b);//�ж�5�����Ƿ��ʤ
			if(*a!=-1)//�����ʤ�򷵻����겢�������� 
			return;
		}
	}//������������ڵ���6��13�����ܱ�ʤ��������Ϊ5������������� 

	bool is_here_livethree=false;
	int f,g,h,k,l,m;
	board board_1;
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			f=can_he_form_livefour(i,j,pboard);//����Է��ܳɻ��� (���Է��л���)
			if(f>0){
				is_here_livethree=true;
				value_of_everypoint[i][j].number=1;//����õ�
				give_value(&(value_of_everypoint[i][j]),pboard);//����ֵ�ṹ�帳ֵ
				board_1=*pboard;
				board_1.number++;
				board_1.everypoint[i][j].color=board_1.number%2;//�ж���˵��Է��Ƿ��ʤ 
				value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//����Է���ʤ������� 
				//����õ��ֵ
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//����������ֵΪ1
				else 
					value_of_everypoint[i][j].value=100-value_of_everypoint[i][j].he_can_win*98+
					value_of_everypoint[i][j].count_of_his_livethree_1*10+
					value_of_everypoint[i][j].count_of_his_livethree_2*10+
					value_of_everypoint[i][j].count_of_his_livetwo_false+
					value_of_everypoint[i][j].count_of_his_livetwo_true+
					value_of_everypoint[i][j].count_of_his_otherfour*10+
					value_of_everypoint[i][j].count_of_his_sleepthree+
					value_of_everypoint[i][j].count_of_livethree_1*10+
					value_of_everypoint[i][j].count_of_livethree_2*10+
					value_of_everypoint[i][j].count_of_livetwo_false+
					value_of_everypoint[i][j].count_of_livetwo_true+
					value_of_everypoint[i][j].count_of_otherfour*10+
					value_of_everypoint[i][j].count_of_sleepthree;
				}
			}
	int value,value_max;
	value=0;value_max=0;
	r=0;
	if(is_here_livethree){
		for(i=0;i<15;i++)//�������е�,������ļ�ֵ 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].number==1){//����õ㱻������Է��ܳɻ��ĵĵ㣨������������) 
					value=value_of_everypoint[i][j].value;
					if(value>value_max)//�Ƚϼ�ֵ 
						value_max=value;
				}
			}
		for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].value==value_max){
					r++;
					value_of_everypoint[i][j].random_number=r;
				}
			}
		random=rand();
		r=random%r+1;//��r��һ�������
		for(i=0;i<15;i++)//����������������� 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].random_number==r){
					*a=i;*b=j;
				}
			}
		return; 
	}
	int my_points=0;
	int his_points=0;//���ڼ�¼�����ͶԷ��ܳɻ�������ĵ�ĸ��� 
	if(pboard->number>10){
	for(i=0;i<15;i++)//����ÿ���㣬ͳ���ܳɻ������ĵ�ĸ����������ֵ�ܳɻ�������ĵĵ� 
		for(j=0;j<15;j++){
			f=can_form_livethree_1(i,j,pboard);
			g=can_form_livethree_2(i,j,pboard);
			h=can_form_otherfour(i,j,pboard);
			k=can_he_form_livethree_1(i,j,pboard);
			l=can_he_form_livethree_2(i,j,pboard);
			m=can_he_form_otherfour(i,j,pboard);
			if((f+g+h)>0){//��������ܳɻ�������� 
				if((f+g)<2&&h<2){//������ǽ��� 
					my_points++;//ͳ������+1 
					value_of_everypoint[i][j].number_attack=1;//���������㱻����Ϊ1 
					give_value(&(value_of_everypoint[i][j]),pboard);//����ֵ�ṹ�帳ֵ
					board_1=*pboard;
					board_1.number++;
					board_1.everypoint[i][j].color=board_1.number%2;//�ж���˵��Է��Ƿ��ʤ 
					value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//����Է���ʤ������� 
					//����õ��ֵ
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//����������ֵΪ1
				else //�������ֵ�����ǶԷ���� 
					value_of_everypoint[i][j].value=100-value_of_everypoint[i][j].he_can_win*98+
					value_of_everypoint[i][j].count_of_his_livethree_1*10+
					value_of_everypoint[i][j].count_of_his_livethree_2*10+
					value_of_everypoint[i][j].count_of_his_livetwo_false+
					value_of_everypoint[i][j].count_of_his_livetwo_true+
					value_of_everypoint[i][j].count_of_his_otherfour*10+
					value_of_everypoint[i][j].count_of_his_sleepthree+
					value_of_everypoint[i][j].count_of_livethree_1*10+
					value_of_everypoint[i][j].count_of_livethree_2*10+
					value_of_everypoint[i][j].count_of_livetwo_false*2+
					value_of_everypoint[i][j].count_of_livetwo_true*2+
					value_of_everypoint[i][j].count_of_otherfour*10+
					value_of_everypoint[i][j].count_of_sleepthree*2;
				}
			}
			if((k+l+m)>0){//�Է� 
				if((k+l)<2&&m<2){
					his_points++;
					value_of_everypoint[i][j].number_defence=1;//�Է������㱻���� 
					give_value(&(value_of_everypoint[i][j]),pboard);//����ֵ�ṹ�帳ֵ
					board_1=*pboard;
					board_1.number++;
					board_1.everypoint[i][j].color=board_1.number%2;//�ж���˵��Է��Ƿ��ʤ 
					value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//����Է���ʤ������� 
					//����õ��ֵ
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//����������ֵΪ1
				else //���ص��ֵ�����ǶԷ���� 
					value_of_everypoint[i][j].value=100-value_of_everypoint[i][j].he_can_win*98+
					value_of_everypoint[i][j].count_of_his_livethree_1*10+
					value_of_everypoint[i][j].count_of_his_livethree_2*10+
					value_of_everypoint[i][j].count_of_his_livetwo_false*2+
					value_of_everypoint[i][j].count_of_his_livetwo_true*2+
					value_of_everypoint[i][j].count_of_his_otherfour*10+
					value_of_everypoint[i][j].count_of_his_sleepthree*2+
					value_of_everypoint[i][j].count_of_livethree_1*10+
					value_of_everypoint[i][j].count_of_livethree_2*10+
					value_of_everypoint[i][j].count_of_livetwo_false+
					value_of_everypoint[i][j].count_of_livetwo_true+
					value_of_everypoint[i][j].count_of_otherfour*10+
					value_of_everypoint[i][j].count_of_sleepthree;
				}
			}
		}
	} 
	if(my_points!=0||his_points!=0){//������ڿ��µ� 
		if(pboard->number%2==0){//���������� 
			if(my_points!=0){
				if((my_points-his_points)>=-2){//�Ƚϼ������ֵ 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//�������е� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1){//��������㱻����
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//�Ƚϼ�ֵ 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
							//����ǽ����㲢�Ҽ�ֵ���
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//��r��һ�������
					for(i=0;i<15;i++)//����������������� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}else{//�Ƚ϶Է����ֵ 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//�������е� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1){//������ص㱻����
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//�Ƚϼ�ֵ 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
							//����Ƿ��ص㲢�Ҽ�ֵ��� 
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//��r��һ�������
					for(i=0;i<15;i++)//����������������� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}
			}else{//�Ƚ϶Է����ֵ 
				value=0;value_max=0;
				r=0;
				for(i=0;i<15;i++)//�������е� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_defence==1){//������ص㱻����
							value=value_of_everypoint[i][j].value;
							if(value>value_max)//�Ƚϼ�ֵ 
								value_max=value;
						}
					}
				for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
						//����Ƿ��ص㲢�Ҽ�ֵ��� 
							r++;
							value_of_everypoint[i][j].random_number=r;
						}
					}
				random=rand();
				r=random%r+1;//��r��һ�������
				for(i=0;i<15;i++)//����������������� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].random_number==r){
							*a=i;*b=j;
						}
					}
				return; 
			}
		}else{//�� 
			if(his_points!=0){
				if((his_points-my_points)>=-2){//�Ƚ϶Է����ֵ 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//�������е� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1){//������ص㱻����
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//�Ƚϼ�ֵ 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
							//����Ƿ��ص㲢�Ҽ�ֵ��� 
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//��r��һ�������
					for(i=0;i<15;i++)//����������������� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}else{//�Ƚϼ������ֵ 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//�������е� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1){//��������㱻����
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//�Ƚϼ�ֵ 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
							//����ǽ����㲢�Ҽ�ֵ���
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//��r��һ�������
					for(i=0;i<15;i++)//����������������� 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}
			}else{//�Ƚϼ������ֵ 
				value=0;value_max=0;
				r=0;
				for(i=0;i<15;i++)//�������е� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_attack==1){//��������㱻����
							value=value_of_everypoint[i][j].value;
							if(value>value_max)//�Ƚϼ�ֵ 
								value_max=value;
						}
					}
				for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
						//����ǽ����㲢�Ҽ�ֵ���
							r++;
							value_of_everypoint[i][j].random_number=r;
						}
					}
				random=rand();
				r=random%r+1;//��r��һ�������
				for(i=0;i<15;i++)//����������������� 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].random_number==r){
							*a=i;*b=j;
						}
					}
				return; 
			}
		}
	}
	if(my_points==0&&his_points==0){//��������ͶԷ�������Ϊ0
		int count_of_two=0;//��¼���γɻ���ĵ���� 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				f=can_form_sleepthree(i,j,pboard);//����ܳɻ�������� 
				g=can_form_livetwo_true(i,j,pboard);
				h=can_form_livetwo_false(i,j,pboard);
				k=can_he_form_sleepthree(i,j,pboard); 
				l=can_he_form_livetwo_true(i,j,pboard);
				m=can_he_form_livetwo_false(i,j,pboard);
				if((f+g+h+k+l+m)>0){
					count_of_two++;
					value_of_everypoint[i][j].number=1;//����õ�
					give_value(&(value_of_everypoint[i][j]),pboard);
					if(pboard->number>10)
						if(value_of_everypoint[i][j].forbidden_moves==true)
							value_of_everypoint[i][j].value=1;//����������ֵΪ1
						else 
							value_of_everypoint[i][j].value=100+(f+g+h+k+l+m);
					if(pboard->number<=10){
						if(pboard->number<5)
							//����õ��ֵ
							if(value_of_everypoint[i][j].forbidden_moves==true)
								value_of_everypoint[i][j].value=1;//����������ֵΪ1
							else 
								value_of_everypoint[i][j].value=100+
								value_of_everypoint[i][j].count_of_his_livethree_1*2+
								value_of_everypoint[i][j].count_of_his_livethree_2+
								value_of_everypoint[i][j].count_of_his_livetwo_false+
								value_of_everypoint[i][j].count_of_his_livetwo_true*2+
								value_of_everypoint[i][j].count_of_his_otherfour+
								value_of_everypoint[i][j].count_of_his_sleepthree+
								value_of_everypoint[i][j].count_of_livethree_1*2+
								value_of_everypoint[i][j].count_of_livethree_2+
								value_of_everypoint[i][j].count_of_livetwo_false+
								value_of_everypoint[i][j].count_of_livetwo_true*2+
								value_of_everypoint[i][j].count_of_otherfour+
								value_of_everypoint[i][j].count_of_sleepthree;
						else
							if(value_of_everypoint[i][j].forbidden_moves==true)
								value_of_everypoint[i][j].value=1;//����������ֵΪ1
							else 
								value_of_everypoint[i][j].value=100+
								value_of_everypoint[i][j].count_of_his_livethree_1*3+
								value_of_everypoint[i][j].count_of_his_livethree_2*2+
								value_of_everypoint[i][j].count_of_his_livetwo_false+
								value_of_everypoint[i][j].count_of_his_livetwo_true+
								value_of_everypoint[i][j].count_of_his_otherfour*2+
								value_of_everypoint[i][j].count_of_his_sleepthree+
								value_of_everypoint[i][j].count_of_livethree_1*2+
								value_of_everypoint[i][j].count_of_livethree_2*2+
								value_of_everypoint[i][j].count_of_livetwo_false+
								value_of_everypoint[i][j].count_of_livetwo_true+
								value_of_everypoint[i][j].count_of_otherfour*2+
								value_of_everypoint[i][j].count_of_sleepthree;
					}
				}
			}
		if(count_of_two!=0){//����л������������ 
			value=0;value_max=0;
			r=0;
			for(i=0;i<15;i++)//�������е�,������ļ�ֵ 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].number==1){//����õ㱻����
						value=value_of_everypoint[i][j].value;
						if(value>value_max)//�Ƚϼ�ֵ 
							value_max=value;
					}
				}
			for(i=0;i<15;i++)//������ֵ�ĵ㸳����� 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].value==value_max){
						r++;
						value_of_everypoint[i][j].random_number=r;
					}
				}
			random=rand();
			r=random%r+1;//��r��һ�������
			for(i=0;i<15;i++)//����������������� 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].random_number==r){
						*a=i;*b=j;
					}
				}
			return; 
		}
		if(count_of_two==0){//���û�л���� 
			if(pboard->number>=224){
				*a=-1;*b=-1;
				return;
			}
			if(pboard->everypoint[7][7].color==-1){
				*a=7;*b=7;
				return;
			}
			for(i=0;i<15;i++)
				for(j=0;j<15;j++){
					if(pboard->everypoint[i][j].color==-1){
						*a=i;*b=j;
						return;
					}
				}
		}
	}
}



void huiqi(){
	int i,j;
	if(board1.number!=0){
		if(board1.gamemode.player==2){
		for(i=0;i<15;i++)
			for(j=0;j<15;j++)
				if(board1.everypoint[i][j].number==board1.number){
					board1.everypoint[i][j].color=-1;
					board1.everypoint[i][j].number=0;	
				}
		board1.number=board1.number-1;
		}
		if(board1.gamemode.player==1){
			for(i=0;i<15;i++)
				for(j=0;j<15;j++)
					if(board1.everypoint[i][j].number==board1.number){
						board1.everypoint[i][j].color=-1;
						board1.everypoint[i][j].number=0;	
					}
			board1.number=board1.number-1;
			for(i=0;i<15;i++)
				for(j=0;j<15;j++)
					if(board1.everypoint[i][j].number==board1.number){
						board1.everypoint[i][j].color=-1;
						board1.everypoint[i][j].number=0;	
					}
			board1.number=board1.number-1;
		}
	}
	//���´�ӡ���� 
	putimage(0,0,img_board);			// ������ӡ���� 
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			if(board1.everypoint[i][j].color==1)
				putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//��ӡ����
			if(board1.everypoint[i][j].color==0)
				putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//��ӡ���� 
		}
}

void two_players(){	//˫�˶��� 
	bool f=true;			//f�����ж�����Ƿ����(trueΪδ������flaseΪ����)
	int x=0,y=0;			//�����λ�� 
	int a=0,b=0;			//�����ϵ����� 
	int count_of_five=0;	//��¼��a��b�����γ�5�ĸ��� 
	FILE *fp;int c=0;				//������ 
	
	int count_of_livefour=0;
	int count_of_otherfour=0;
	int count_of_overfive=0;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	int count_of_livethree_1=0;
	int count_of_livethree_2=0;
	int count_of_livetwo_true=0;
	int count_of_livetwo_false=0;
	
	int ender=-1;			//���һ����ɫ��1�����壬0�����壩 
	bool forbidden_moves=false;//�ж��Ƿ���� 
	

	while(f){
		if(board1.gamemode.player==1&&(board1.gamemode.first==board1.number%2)){//����ֵ������� 
			best_point(&board1,&a,&b);
			put_board(a,b,&board1);		//�������ڵ㣨a��b���� 
			//��ʼ������ 
			count_of_livefour=0;
			count_of_otherfour=0;
			count_of_overfive=0;
			count_of_five=0;
			count_of_livethree=0;
			count_of_sleepthree=0;
			count_of_livethree_1=0;
			count_of_livethree_2=0;
			count_of_livetwo_true=0;
			count_of_livetwo_false=0;
			//���� 
			count_of_five=five(a,b,&board1);							//���γ�5�ĸ���
			count_of_overfive=overfive(a,b,&board1);
			four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
			three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
			count_of_livethree_1=livethree_1(a,b,&board1);
			count_of_livethree_2=livethree_2(a,b,&board1);
			count_of_livetwo_true=livetwo_true(a,b,&board1);
			count_of_livetwo_false=livetwo_false(a,b,&board1);
			//������Ƿ���ȷ 
			printf("��(%d,%d)\n������%4d\n��  ��%4d\n���ģ�%4d\n���ģ�%4d\n������%4d\n������%4d\n",a,b,
			count_of_overfive,count_of_five,
			count_of_livefour,count_of_otherfour,
			count_of_livethree,count_of_sleepthree);
			printf("����-1��%4d\n����-2��%4d\n������%4d\n�ٻ����%4d\n",
			count_of_livethree_1,
			count_of_livethree_2,
			count_of_livetwo_true,
			count_of_livetwo_false);
			
			if(board1.everypoint[a][b].number==board1.number){			//�������������������µ� 
			//�ж�ʤ�� 
				if(count_of_five!=0){		//�������5�����ֽ��� 
					ender=board1.number%2;
					f=false;	
				}else{						//���û���� 
					if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//����н������Ǻ��� 
						if(	(count_of_livefour+count_of_otherfour)>1||		//���"˫��""˫��"��"����" 
							count_of_livethree>1||count_of_overfive>0){
							ender=0;
							forbidden_moves=true;
							f=false;						//����ʤ������ʤ����ʽΪ�ڷ����� 
						}
					}
				}
			}
		}else{
		click(&x,&y);
		if(x<590&&y<590&&x>10&&y>10){									//������������� 
			a=((float)x-(25-(float)550/28))/((float)550/14);
			b=((float)y-(25-(float)550/28))/((float)550/14);
			put_board(a,b,&board1);		//�������ڵ㣨a��b���� 
			
			//��ʼ������ 
			count_of_livefour=0;
			count_of_otherfour=0;
			count_of_overfive=0;
			count_of_five=0;
			count_of_livethree=0;
			count_of_sleepthree=0;
			count_of_livethree_1=0;
			count_of_livethree_2=0;
			count_of_livetwo_true=0;
			count_of_livetwo_false=0;
			//���� 
			count_of_five=five(a,b,&board1);							//���γ�5�ĸ���
			count_of_overfive=overfive(a,b,&board1);
			four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
			three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
			count_of_livethree_1=livethree_1(a,b,&board1);
			count_of_livethree_2=livethree_2(a,b,&board1);
			count_of_livetwo_true=livetwo_true(a,b,&board1);
			count_of_livetwo_false=livetwo_false(a,b,&board1);
			//������Ƿ���ȷ 
			printf("��(%d,%d)\n������%4d\n��  ��%4d\n���ģ�%4d\n���ģ�%4d\n������%4d\n������%4d\n",a,b,
			count_of_overfive,count_of_five,
			count_of_livefour,count_of_otherfour,
			count_of_livethree,count_of_sleepthree);
			printf("����-1��%4d\n����-2��%4d\n������%4d\n�ٻ����%4d\n",
			count_of_livethree_1,
			count_of_livethree_2,
			count_of_livetwo_true,
			count_of_livetwo_false);
			
			if(board1.everypoint[a][b].number==board1.number){			//�������������������µ� 
			//�ж�ʤ�� 
				if(count_of_five!=0){		//�������5�����ֽ��� 
					ender=board1.number%2;
					f=false;	
				}else{						//���û���� 
					if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//����н������Ǻ��� 
						if(	(count_of_livefour+count_of_otherfour)>1||		//���"˫��""˫��"��"����" 
							count_of_livethree>1||count_of_overfive>0){
							ender=0;
							forbidden_moves=true;
							f=false;						//����ʤ������ʤ����ʽΪ�ڷ����� 
						}
					}
				}
			}
		}
		if(x>=630&&x<720){												//�������ť 
				if(y>=60&&y<90){							//��ʾ 
					best_point(&board1,&a,&b);
					put_board(a,b,&board1);		//�������ڵ㣨a��b���� 
					//��ʼ������ 
					count_of_livefour=0;
					count_of_otherfour=0;
					count_of_overfive=0;
					count_of_five=0;
					count_of_livethree=0;
					count_of_sleepthree=0;
					count_of_livethree_1=0;
					count_of_livethree_2=0;
					count_of_livetwo_true=0;
					count_of_livetwo_false=0;
					//���� 
					count_of_five=five(a,b,&board1);							//���γ�5�ĸ���
					count_of_overfive=overfive(a,b,&board1);
					four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
					three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
					count_of_livethree_1=livethree_1(a,b,&board1);
					count_of_livethree_2=livethree_2(a,b,&board1);
					count_of_livetwo_true=livetwo_true(a,b,&board1);
					count_of_livetwo_false=livetwo_false(a,b,&board1);
					//������Ƿ���ȷ 
					printf("��(%d,%d)\n������%4d\n��  ��%4d\n���ģ�%4d\n���ģ�%4d\n������%4d\n������%4d\n",a,b,
					count_of_overfive,count_of_five,
					count_of_livefour,count_of_otherfour,
					count_of_livethree,count_of_sleepthree);
					printf("����-1��%4d\n����-2��%4d\n������%4d\n�ٻ����%4d\n",
					count_of_livethree_1,
					count_of_livethree_2,
					count_of_livetwo_true,
					count_of_livetwo_false);
					
					if(board1.everypoint[a][b].number==board1.number){			//�������������������µ� 
					//�ж�ʤ�� 
						if(count_of_five!=0){		//�������5�����ֽ��� 
							ender=board1.number%2;
							f=false;	
						}else{						//���û���� 
							if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//����н������Ǻ��� 
								if(	(count_of_livefour+count_of_otherfour)>1||		//���"˫��""˫��"��"����" 
									count_of_livethree>1||count_of_overfive>0){
									ender=0;
									forbidden_moves=true;
									f=false;						//����ʤ������ʤ����ʽΪ�ڷ����� 
								}
							}
						}
					}
				}
				if(y>=120&&y<150){							//���� 
					huiqi();
				}
				if(y>=180&&y<210){							//���� 
					setfillcolor(WHITE);
					bar(600,0,750,600);					//����Ҳ� 
					setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
					setcolor(EGERGB(0x0, 0x0, 0x0));
					setfont(22, 0, "����");
					setbkmode(TRANSPARENT);
					settextjustify(1,1);
					bar(630, 60, 720, 90);
					outtextxy(675, 75, "�浵1");
					bar(630, 120, 720, 150);
					outtextxy(675, 135, "�浵2");
					bar(630, 180, 720, 210);
					outtextxy(675, 195, "�浵3");
					bar(630, 240, 720, 270);
					outtextxy(675, 255, "�浵4");
					bar(630, 300, 720, 330);
					outtextxy(675, 315, "�浵5");
					bar(630, 360, 720, 390);
					outtextxy(675, 375, "�浵6");
					
					while(c==0){				//���ֱ����ť 
						click(&x,&y);
						if(x>=630&&x<720){
							if(y>=60&&y<90) {fp=fopen("save\\save1.dat","wb");c=1;}
							if(y>=120&&y<150) {fp=fopen("save\\save2.dat","wb");c=2;}
							if(y>=180&&y<210) {fp=fopen("save\\save3.dat","wb");c=3;}
							if(y>=240&&y<270) {fp=fopen("save\\save4.dat","wb");c=4;}
							if(y>=300&&y<330) {fp=fopen("save\\save5.dat","wb");c=5;}
							if(y>=360&&y<390) {fp=fopen("save\\save6.dat","wb");c=6;}
						}
					}							//�������
					c=0;
					fwrite(&board1,sizeof(board),1,fp);			//�����д��浵 
					fclose(fp);
					//��ԭ���� 
					setfillcolor(WHITE);
					bar(600,0,750,600);					//����Ҳ� 
					setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
					setcolor(EGERGB(0x0, 0x0, 0x0));
					setfont(22, 0, "����");
					setbkmode(TRANSPARENT);
					settextjustify(1,1);
					bar(630, 60, 720, 90);
					outtextxy(675, 75, "��ʾ");
					bar(630, 120, 720, 150);
					outtextxy(675, 135, "����");
					bar(630, 180, 720, 210);
					outtextxy(675, 195, "����");
					bar(630, 240, 720, 270);
					outtextxy(675, 255, "�����˵�");
				}
				if(y>=240&&y<270){							//�����˵� 
					return;				//��ֽ��� 
				}
			}
		}
	}
	

	//ʤ������ 
	setcolor(0xFF0000);
	setfont(35,0,"����");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	if(ender==1)
		outtextxy(300, 300, "����ʤ��");
	else if(forbidden_moves)
			outtextxy(300, 300, "�ڷ����֣�����ʤ��");
		else
			outtextxy(300, 300, "����ʤ��");
	setfillcolor(WHITE);
	//�˳���Ϸ 
	bar(600,0,750,600);					//����Ҳ� 
	setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	setfont(22, 0, "����");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "�����˵�");
	while(c==0){				//���ֱ����ť 
		click(&x,&y);
		if(x>=630&&x<720){
			if(y>=60&&y<90) c=1;
		}
	}							//�������
}

void playing(menu mode){
	printf("start game!!!\n");
	//��ӡ�Ծֽ��� 
	print_game();
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "��ʾ");
	bar(630, 120, 720, 150);
	outtextxy(675, 135, "����");
	bar(630, 180, 720, 210);
	outtextxy(675, 195, "����");
	bar(630, 240, 720, 270);
	outtextxy(675, 255, "�����˵�");
	//��ʼ�������Ϣ 
	if(mode.player!=-1){						//������Ǹ��� 
		board1.gamemode=mode;
		board1.number=0;
		int i,j;
		for(j=0;j<15;j++)
			for(i=0;i<15;i++){
				board1.everypoint[j][i].color=-1;
				board1.everypoint[j][i].number=0;
			}
	}else{												//����Ǹ��� 
		FILE *fp;
		switch(mode.level){
			case 1:fp=fopen("save\\save1.dat","rb");break;
			case 2:fp=fopen("save\\save2.dat","rb");break;
			case 3:fp=fopen("save\\save3.dat","rb");break;
			case 4:fp=fopen("save\\save4.dat","rb");break;
			case 5:fp=fopen("save\\save5.dat","rb");break;
			case 6:fp=fopen("save\\save6.dat","rb");break;
		}
		fread(&board1,sizeof(board),1,fp);
		fclose(fp);
		int i,j;
		//��ӡ��ʼ���� 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				if(board1.everypoint[i][j].color==1)
					putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//��ӡ����
				if(board1.everypoint[i][j].color==0)
					putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//��ӡ���� 
			}
	
	}
	
	//���
	printf("player:%d\nforbidden_moves:%d\nfirst:%d\nlevel:%d\n",
	board1.gamemode.player,board1.gamemode.forbidden_moves,
	board1.gamemode.first,board1.gamemode.level); 
	
	if(board1.gamemode.player==2||board1.gamemode.player==1){
		two_players();
	}
}

void print_page(int n){
	FILE *fp;
	switch(n){
		case 1:fp=fopen("rules\\rule1.dat","rb");break;
		case 2:fp=fopen("rules\\rule2.dat","rb");break;
		case 3:fp=fopen("rules\\rule3.dat","rb");break;
		case 4:fp=fopen("rules\\rule4.dat","rb");break;
		case 5:fp=fopen("rules\\rule5.dat","rb");break;
		case 6:fp=fopen("rules\\rule6.dat","rb");break;
	}
	board board_1;
	fread(&board_1,sizeof(board),1,fp);
	fclose(fp);
		int i,j;
		//��ӡ��ʼ���� 
		putimage(0,0,img_board);			// ������ӡ���� 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				if(board_1.everypoint[i][j].color==1)
					putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//��ӡ����
				if(board_1.everypoint[i][j].color==0)
					putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//��ӡ���� 
			}
	//����������Ϣ 
	setcolor(0xFF0000);
	setfont(25,0,"����");
	setbkmode(TRANSPARENT);
	settextjustify(0,0);
	switch(n){
		case 1:	outtextxy(30,30,"���Ͻ�����Ϊ��0��0�������½�����Ϊ��14��1");
				outtextxy(30,60,"4����ʾ��������������Դ�Ϊ��׼");
				outtextxy(30,90,"ͬɫ��������5�����ϣ�������5������Ϊ����");
				break;
		case 2:	outtextxy(30,30,"ͬɫ��������4���������˶��ܳ�5��������");
				outtextxy(30,60,"���ǽ����򲻰�����������Ϊ���ġ����ĳ��");
				outtextxy(30,90,"û�γɻ��ģ�����8�������ͬɫ�ӵ�8�����");
				outtextxy(30,120,"������ͬɫ�����γ�n��5����(����ǽ���");
				outtextxy(30,150,"�򲻰�����������Ƹõ�����γ���n������"); 
				outtextxy(30,180,"������7��6���γ���1�����ģ���7��7���γ�");
				outtextxy(30,210,"��һ�����Ķ���7��9���γ���2������");
				break;
		case 3:	outtextxy(30,30,"���ĳ����4���������ϵ����£��ϵ��£���");
				outtextxy(30,60,"�µ����ϣ��У�n����������������ͬɫ����");
				outtextxy(30,90,"��һ������ͬɫ�Ӻ����γɻ��ģ���Ƹõ�Ϊ��");
				outtextxy(30,120,"���γ���n������");
				outtextxy(30,150,"������7��7������8��7�����γ���һ������");
				break;
		case 4:	outtextxy(30,30,"���º����γɳ����ĵ�Ϊ���ֵ�");
				outtextxy(30,60,"������7��7��Ϊ���ֵ�");
				break;
		case 5:	outtextxy(30,30,"���º���ͬʱ�γ��������Ļ���ĵĵ�Ϊ���ֵ�");
				outtextxy(30,60,"������7��7��Ϊ���ֵ�");
				break;
		case 6:	outtextxy(30,30,"���º���ͬʱ�γ����������ĵ�Ϊ���ֵ�");
				outtextxy(30,60,"������7��7��Ϊ���ֵ�");
				break;
	}
}

void rule_of_forbidden_moves(){
	//��ӡ�Ծֽ��� 
	print_game();
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "��һҳ");
	print_page(1);
	int page=1;
	int x,y;
	while(page<7){				//���������ť����ӡ��һҳ 
		click(&x,&y);
		if(x>=630&&x<720)
			if(y>=60&&y<90){
				page++;
				if(page<7)
					print_page(page);
			}
	}							//��ҳ������6�������� 
}

int main(){
	initgraph(750,600,0);						//�������� 750*600 
	setcaption("������");					//���ô��ڱ���
	setbkcolor(WHITE);							//���ñ���Ϊ��ɫ
	material();									//��ȡ�ز� 
	//������ 
	menu mode;	//������Ϸģʽ 
	int x=-20,y=-20;	//��¼�����λ�� 
	int a=0;			//��¼����İ�ť 
	do{
		//�ṹ���ʼ�� 
		mode.player=0;
		mode.forbidden_moves=-1;
		mode.first=-1;
		mode.level=0;
		beginning(&mode);
		//��ʼ���棨˫�˶��ģ��˻����ģ���ȡ��¼�����ֹ��򣬽�����Ϸ�� 
		a=0;			//aΪ��ťλ�� 
		while(a==0){				//���ֱ����ť 
			click(&x,&y);
			if(x>=630&&x<720){
				if(y>=60&&y<90) a=1;
				if(y>=120&&y<150) a=2;
				if(y>=180&&y<210) a=3;
				if(y>=240&&y<270) a=4;
				if(y>=300&&y<330) a=5;
			}
		}							//�������
		//���ݰ�ť����
		if(a==5) end=1;	//��Ϸ���� 
		//˫�˶��� 
		if(a==1){
			mode.player=2;			//�����Ϣ-���������Ϊ2 
			beginning(&mode);
			a=0;
			while(a==0){				//���ֱ����ť 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//�������
			//�������޽��� 
			if(a==1) mode.forbidden_moves=1;
			if(a==2) mode.forbidden_moves=0;
			playing(mode);				//ģʽѡ����ʼ��Ϸ 
			a=0;						//��ťa��ʼ�� 
		}
		//�˻����� 
		if(a==2){
			mode.player=1;			//�����Ϣ-���������Ϊ1
			beginning(&mode);
			a=0;
			while(a==0){				//���ֱ����ť 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//�������
			//�������޽��� 
			if(a==1) mode.forbidden_moves=1;
			if(a==2) mode.forbidden_moves=0;
			beginning(&mode);
			a=0;
			while(a==0){				//���ֱ����ť 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//�������
			//�����Ⱥ���
			if(a==1) mode.first=1;
			if(a==2) mode.first=0;
			beginning(&mode);
			a=0;
			while(a==0){				//���ֱ����ť 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//�������
			//�����Ѷ�
			if(a==1) mode.level=1;
			if(a==2) mode.level=2;
			playing(mode);				//ģʽѡ����ʼ��Ϸ 
			a=0;						//��ťa��ʼ�� 
		}
		//��ȡ��¼ 
		if(a==3){
			mode.player=-1;
			beginning(&mode);
			a=0;
			while(a==0){				//���ֱ����ť 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
					if(y>=180&&y<210) a=3;
					if(y>=240&&y<270) a=4;
					if(y>=300&&y<330) a=5;
					if(y>=360&&y<390) a=6;
				}
			}							//�������
			mode.level=a;
			playing(mode);
			a=0;
		}
		//���ֹ��� 
		if(a==4){
			rule_of_forbidden_moves();
		}
	}while(end==0);
	
	//�ر����̣����ӣ�����ͼ��
	delimage(img_board);
	delimage(img_black);
	delimage(img_white);
	//�رմ��� 
	closegraph();
}
