#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
typedef struct MENU{		//定义菜单选项（游戏模式） 
	int player;				//玩家数 （1：人机对弈 2：双人对弈 -1：由存档决定） 
	int forbidden_moves;	//是否禁手 (1:有 0：无)
	int first;				//先后手 (1:先手 0：后手)
	int level;				//难度 （1：简单 2：困难） 
} menu;
//声明棋盘信息结构体 
typedef struct  t_point{
	int color;		//1:black	0:white
	int number;
} point;
typedef struct t_board{
	menu gamemode;				//对局模式 
	int number;					//落子总数 
	point everypoint[15][15];	//每个棋子的信息 
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

PIMAGE img_board,img_black,img_white;	//声明棋盘，黑子，白子的图像的全局变量 
int end=0; 				//判断游戏是否结束 
board board1;			//工作变量，存储正在进行的棋局信息 

void click(int *px,int *py){//检测鼠标信息直到单击左键，返回鼠标点击的坐标（x，y） 
	mouse_msg msg = {0};
	int x=-100,y=-100,x1=-200,y1=-200;						//初始化	防止x，y的初始值与x1，y1的初始值相近; 
	bool f;
	f=is_run();
	for ( ; f; delay_fps(60)){
		while (mousemsg())									//获取鼠标消息，等待直到有消息为止 
		{
			msg = getmouse();
		}
		if((int)msg.is_down()==1&&(int)msg.is_left()==1){	//记录左键鼠标按下位置 
			x=msg.x;y=msg.y;
		}
		if((int)msg.is_down()==0&&(int)msg.is_left()==1){	//记录鼠标左键抬起位置 
			x1=msg.x;y1=msg.y;
		}
		if((x1-x)*(x1-x)<100&&(y1-y)*(y1-y)<100){			//如果按下和抬起位置相差小于10，则确认为单击 
			f=false;										//并停止获取鼠标消息 
		}else	f=true;
	}
	*px=x;*py=y;											//返回鼠标左键单击位置 
}

void material(){	//获取素材，并赋值到三个PIMAGE类型全局变量中 
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
	bar(600,0,750,600);					//清空右侧 
	putimage(0,0,img_board);			// 在左侧打印棋盘 
}

void print_game(){			//打印棋局(初始棋局和右侧白条)，设置右侧字体信息 
	print_board();
	//设置字体信息 
	setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	setfont(22, 0, "宋体");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	
}

void beginning(menu *pmenu){	//打印初始界面 
	print_game();
	if(pmenu->player==0){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "双人对弈");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "人机对弈");
		bar(630, 180, 720, 210);
		outtextxy(675, 195, "读取记录");
		bar(630, 240, 720, 270);
		outtextxy(675, 255, "禁手规则");
		bar(630, 300, 720, 330);
		outtextxy(675, 315, "结束游戏");
	}
	if(pmenu->player==2){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "有禁手");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "无禁手");
	}
	if(pmenu->player==1){
		if(pmenu->forbidden_moves==-1){
			bar(630, 60, 720, 90);
			outtextxy(675, 75, "有禁手");
			bar(630, 120, 720, 150);
			outtextxy(675, 135, "无禁手");
		}else{
			if(pmenu->first==-1){
				bar(630, 60, 720, 90);
				outtextxy(675, 75, "先手");
				bar(630, 120, 720, 150);
				outtextxy(675, 135, "后手");
			}else{
				bar(630, 60, 720, 90);
				outtextxy(675, 75, "简单");
				bar(630, 120, 720, 150);
				outtextxy(675, 135, "困难");
			}
		}
	}
	if(pmenu->player==-1){
		bar(630, 60, 720, 90);
		outtextxy(675, 75, "存档1");
		bar(630, 120, 720, 150);
		outtextxy(675, 135, "存档2");
		bar(630, 180, 720, 210);
		outtextxy(675, 195, "存档3");
		bar(630, 240, 720, 270);
		outtextxy(675, 255, "存档4");
		bar(630, 300, 720, 330);
		outtextxy(675, 315, "存档5");
		bar(630, 360, 720, 390);
		outtextxy(675, 375, "存档6");
	}
}

void put_board(	int a,int b,board *pboard){//将子落在（a，b）上 并更改board1的信息
	if(pboard->everypoint[a][b].color==-1){					//如果（a，b）没有子 
		pboard->number=board1.number+1;						//棋子数+1 
		pboard->everypoint[a][b].number=pboard->number;		//记录序号
		pboard->everypoint[a][b].color=(pboard->number)%2;	//记录颜色 (1：黑   0：白)
		if((pboard->number)%2==1)
			putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*a,(25-(float)550/28)+((float)550/14)*b,BLACK);	//单数打印黑子
		else
			putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*a,(25-(float)550/28)+((float)550/14)*b,0xF6F6F6);//双数打印白子 
	}
}




int length(	int a,int b,board *pboard,int mode,		//求(a,b)点连珠长度
			int *frontx=NULL,int *fronty=NULL,		//并返回两端(包含棋子)坐标 
			int *rearx=NULL,int *reary=NULL){
	//工作变量 
	int colour=pboard->everypoint[a][b].color;		//所求的棋子颜色 
	int px,py,qx,qy; 								//指针 
	bool f;											//判断是否停止
	int l;											//记录连珠长度 
	int x,y;										//记录工作方向 
	switch(mode){
		case 0:x=0;y=1;break;			//从上到下 
		case 1:x=1;y=1;break;			//从左上到右下 
		case 2:x=1;y=0;break;			//从左到右 
		case 3:x=1;y=-1;break;			//从左下到右上 
	}
	px=a;py=b;qx=px-x;qy=py-y;f=true;l=0;//初始化工作变量 
	//移动指针到最上端 
	while(f){			
		if(qx>14||qx<0||qy>14||qy<0)
			f=false;	//如果超出边界结束 
		else
			if(pboard->everypoint[qx][qy].color!=colour)
				f=false;//如果没有子或颜色不同结束
			else{
				qx=qx-x;qy=qy-y;px=px-x;py=py-y;//移动指针 
			}
	}
	qx=px;qy=py;			//结束时px，py应指向一段 
	//检测长度l
	while(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==colour) {//当颜色相同且没有出界时
		 qx=qx+x;qy=qy+y;	//将qx，qy移向另一端 (最后在连珠外一格)
		 l++;
	}
	qx=qx-x;qy=qy-y;		//归位 
	//返回两端坐标 
	if(frontx!=NULL&&fronty!=NULL){
		*frontx=px;*fronty=py;
	}
	if(rearx!=NULL&&reary!=NULL){
		*rearx=qx;*reary=qy;
	}
	return l;				//返回连珠长度l 
}

int five(int a,int b,board *pboard){	//求（a，b）四个方向形成"5"的个数 
	int i=0,l=0;
	int n=0;		//记录4个方向 形成"5"的个数 
	for(i=0;i<4;i++){
		 l=length(a,b,pboard,i);
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//有禁手 
		 	if(l==5) n++;
		 }else{																		//无禁手 
		 	if(l>=5) n++;
		 }
	}
	return n;
}

int overfive(int a,int b,board *pboard){	//求（a，b）四个方向形成长连的个数 
	int i=0,l=0;
	int n=0;		//记录4个方向 形成长连的个数 
	for(i=0;i<4;i++){
		 l=length(a,b,pboard,i);
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1)//有禁手 
		 	if(l>5) n++;
	}
	return n;
}

int is_four(int a,int b,board *pboard,int mode,
			/*判断mode方向是否是4，活四返回2及两端坐标，冲四返回能成5端坐标,没有4则返回0,若能形成两个冲四则返回3*/ 
			int *px1=NULL,int *py1=NULL,int *px2=NULL,int *py2=NULL){//若不能成5或在棋盘外坐则标返回为-1
	int px,py,qx,qy;//工作指针，标志两端坐标 
	int n=0;			//判断"4"类型 
	length(a,b,pboard,mode,&px,&py,&qx,&qy);		//求两端坐标 
	int x,y;										//记录工作方向 
	switch(mode){
		case 0:x=0;y=1;break;			//从上到下 
		case 1:x=1;y=1;break;			//从左上到右下 
		case 2:x=1;y=0;break;			//从左到右 
		case 3:x=1;y=-1;break;			//从左下到右上 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//初始化工作变量 (即将指针向外移一格)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//两种情况的棋盘信息 
	//第一种情况 (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//如果端点在棋盘内并且为没有子 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//将board1情况的一端落相同颜色的子 
		if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//有禁手 
		 	if(length(a,b,&board_1,mode)==5){							//判断是否形成5,若形成n+1 
				n++;
				if(px1!=NULL&&py1!=NULL){								//若形成5返回一端坐标 
					*px1=px;*py1=py;
				}
			}else{
				if(px1!=NULL&&py1!=NULL){								//若没有形成5返回-1 
					*px1=-1;*py1=-1;
				}
			}
		 }else{																		//无禁手 
		 	if(length(a,b,&board_1,mode)>=5){							//判断是否形成5,若形成n+1 
				n++;
				if(px1!=NULL&&py1!=NULL){								//若形成5返回一端坐标 
					*px1=px;*py1=py;
				}
			}else{
				if(px1!=NULL&&py1!=NULL){								//若没有形成5返回-1 
					*px1=-1;*py1=-1;
				}
			}
		 }
		
	}
	else{
		if(px1!=NULL&&py1!=NULL){								//若在界外或该点有子返回-1 
			*px1=-1;*py1=-1;
		}
	}
	//第二种情况(board_2) 
	if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//如果端点在棋盘内并且为没有子 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//将board2情况的一端落相同颜色的子 
		 if(pboard->gamemode.forbidden_moves==1&&pboard->everypoint[a][b].color==1){//有禁手 
			if(length(a,b,&board_2,mode)==5){							//判断是否形成5,若形成n+1 
				n++;
				if(px2!=NULL&&py2!=NULL){								//若形成5返回一端坐标 
					*px2=qx;*py2=qy;
				}
			}else{
				if(px2!=NULL&&py2!=NULL){								//若没有形成5返回-1 
					*px2=-1;*py2=-1;
				}
			}
		 }else{																		//无禁手 
			if(length(a,b,&board_2,mode)>=5){							//判断是否形成5,若形成n+1 
				n++;
				if(px2!=NULL&&py2!=NULL){								//若形成5返回一端坐标 
					*px2=qx;*py2=qy;
				}
			}else{
				if(px2!=NULL&&py2!=NULL){								//若没有形成5返回-1 
					*px2=-1;*py2=-1;
				}
			}
		 }
	}else{
		if(px2!=NULL&&py2!=NULL){								//若在界外或该点有子返回-1 
			*px2=-1;*py2=-1;
		}
	}
	if(n==2)															//特殊情况 
		if(length(a,b,pboard,mode)<4)
			n++;
	return n;
}

void four(	int a,int b,board *pboard,			//求（a，b）四个方向活四和冲四的个数 
			int *plivefour,int *potherfour){
	int i=0;//标志方向 
	int kind_four=0;//标志4的种类 
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

void is_three(	int a,int b,board *pboard,int mode,							//判断（a，b）mode方向是否为"3" 
				int *p1,int *p2,											//两端类型（0：无，1：冲四，2：活四） 
				int *px1=NULL,int *py1=NULL,int *px2=NULL,int *py2=NULL){	//两端坐标（对应于p1，p2）出界则返回-1 
	if(is_four(a,b,pboard,mode)!=0){				//先判断是不是4，如果是返回0，0（即不是任何类型的三） 
		*p1=0;*p2=0;
		return;										//退出函数 
	}
	int px,py,qx,qy;//工作指针，标志两端坐标 
	int n1=0,n2=0;			//判断两端"3"类型 
	length(a,b,pboard,mode,&px,&py,&qx,&qy);		//求两端坐标 
	int x,y;										//记录工作方向 
	switch(mode){
		case 0:x=0;y=1;break;			//从上到下 
		case 1:x=1;y=1;break;			//从左上到右下 
		case 2:x=1;y=0;break;			//从左到右 
		case 3:x=1;y=-1;break;			//从左下到右上 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//初始化工作变量 (即将指针向外移一格)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//两种情况的棋盘信息 
	//第一种情况 (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//如果端点在棋盘内并且为没有子 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//将board1情况的一端落相同颜色的子 
		n1=is_four(a,b,&board_1,mode); 			//判断board1端情况并将"4"的类型赋值给n1（0：无，1：冲四，2：活四）
		if(n1==3) n1=1;							//若形成两个冲四按1算 
	}
	//第二种情况(board_2)
	 if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//如果端点在棋盘内并且为没有子 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//将board2情况的一端落相同颜色的子 
		n2=is_four(a,b,&board_2,mode);			//判断board2端情况并将"4"的类型赋值给n2（0：无，1：冲四，2：活四）
		if(n2==3) n2=1;							//若形成两个冲四按1算
	}
	if(px1!=NULL&&py1!=NULL){						//返回p1端坐标； 
		if(px>=0&&px<15&&py>=0&&py<15){		//如果没出界 
			*px1=px;*py1=py;				//返回坐标 
		}else{
			*px1=-1;*py1=-1;				//返回-1 
		}
	}
	if(px2!=NULL&&py2!=NULL){						//返回p2端坐标； 
		if(qx>=0&&qx<15&&qy>=0&&qy<15){		//如果没出界 
			*px2=qx;*py2=qy;				//返回坐标 
		}else{
			*px2=-1;*py2=-1;				//返回-1 
		}
	}
	*p1=n1;*p2=n2;
}

void three(	int a,int b,board *pboard,				//求（a，b）四个方向活三和眠三的个数 
			int *plivethree,int *psleepthree){
				int i=0;						//标志方向 
				int kthree1=0,kthree2=0;	//标志三的种类（两端是否形成4） 
				for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if(kthree1==2||kthree2==2)//有活四为活三 
						*plivethree=*plivethree+1;
					else if(kthree1==1||kthree2==1)//没有活四有冲四为眠三 
							*psleepthree=*psleepthree+1;
				}
}

int livethree_1(int a,int b,board *pboard){
	int i;//标志方向
	int kthree1=0,kthree2=0; 	//标志三的种类（两端是否形成4）
	int kind_of_livethree_1=0; 
	for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if(kthree1==2&&kthree2==2)//两端都能成活四则为活三-1 
						kind_of_livethree_1++;
	}
	return kind_of_livethree_1;
}

int livethree_2(int a,int b,board *pboard){
	int i;//标志方向
	int kthree1=0,kthree2=0; 	//标志三的种类（两端是否形成4）
	int kind_of_livethree_2=0; 
	for(i=0;i<4;i++){
					is_three(a,b,pboard,i,&kthree1,&kthree2);
					if((kthree1==2&&kthree2!=2)||(kthree1!=2&&kthree2==2))
						kind_of_livethree_2++;
	}
	return kind_of_livethree_2;	
}

int is_livetwo(int a,int b,board *pboard,int mode){//返回值：0，不是活二	1，假活二	2，真活二 
	if(is_four(a,b,pboard,mode)!=0){				//先判断是不是4，如果是返回0（即不是任何类型的二） 
		return 0;										//退出函数 
	}
	int p1,p2;
	is_three(a,b,pboard,mode,&p1,&p2);
	if(p1!=0||p2!=0){								//判断是不是三， 如果是返回0 
		return 0;
	}
	int px,py,qx,qy;//工作指针，标志两端坐标 
	bool can_livethree=false;//判断两端落下后能否形成活三 
	int l;
	l=length(a,b,pboard,mode,&px,&py,&qx,&qy);		//求两端坐标 
	int x,y;										//记录工作方向 
	switch(mode){
		case 0:x=0;y=1;break;			//从上到下 
		case 1:x=1;y=1;break;			//从左上到右下 
		case 2:x=1;y=0;break;			//从左到右 
		case 3:x=1;y=-1;break;			//从左下到右上 
	}
	px=px-x;py=py-y;qx=qx+x;qy=qy+y;//初始化工作变量 (即将指针向外移一格)
	board board_1,board_2;
	board_1=*pboard;board_2=*pboard;		//两种情况的棋盘信息 
	//第一种情况 (board_1)
	if(px>=0&&px<15&&py>=0&&py<15&&pboard->everypoint[px][py].color==-1){		//如果端点在棋盘内并且为没有子 
		board_1.everypoint[px][py].color=pboard->everypoint[a][b].color;			//将board1情况的一端落相同颜色的子 
		is_three(a,b,&board_1,mode,&p1,&p2);			//判断board1端情况并将"3"的类型赋值给p1,p2
		if(p1==2||p2==2){//如果是任何类型的活三 
			can_livethree=true;
		} 
	}
	//第二种情况(board_2)
	 if(qx>=0&&qx<15&&qy>=0&&qy<15&&pboard->everypoint[qx][qy].color==-1){		//如果端点在棋盘内并且为没有子 
		board_2.everypoint[qx][qy].color=pboard->everypoint[a][b].color;			//将board2情况的一端落相同颜色的子 
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
	int i;//标志方向
	int k; 	//标志二的种类 
	int n=0;//标志真活二的个数 
	for(i=0;i<4;i++){
		k=is_livetwo(a,b,pboard,i);
		if(k==2)
			n++;	
	}
	return n;
}

int livetwo_false(int a,int b,board *pboard){
	int i;//标志方向
	int k; 	//标志二的种类 
	int n=0;//标志假活二的个数 
	for(i=0;i<4;i++){
		k=is_livetwo(a,b,pboard,i);
		if(k==1)
			n++;	
	}
	return n;
}

int can_form_overfive(int a,int b,board *pboard){//判断落下己方棋子后能否形成长连并返回长连的个数
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

int can_form_five(int a,int b,board *pboard){//判断落下己方棋子后能否形成五并返回五的个数
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

int can_form_four(int a,int b,board *pboard){//判断落下己方棋子后能否形成四并返回活四+冲四的个数 
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

int can_form_livefour(int a,int b,board *pboard){//判断落下己方棋子后能否形成活四并返回活四的个数 
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

int can_form_otherfour(int a,int b,board *pboard){//判断落下己方棋子后能否形成冲四并返回冲四的个数 
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

int can_form_livethree(int a,int b,board *pboard){//判断落下己方棋子后能否形成三并返回活三的个数
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

int can_form_sleepthree(int a,int b,board *pboard){//判断落下己方棋子后能否形成眠三并返回眠三的个数
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

int can_he_form_overfive(int a,int b,board *pboard){//判断落下对方棋子后能否形成长连并返回长连的个数
	board board_1;
	board_1=*pboard;
	int count_of_overfive=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_overfive=overfive(a,b,&board_1);
	}
	return count_of_overfive;
}

int can_he_form_five(int a,int b,board *pboard){//判断落下对方棋子后能否形成五并返回五的个数
	board board_1;
	board_1=*pboard;
	int count_of_five=0;
	if(board_1.everypoint[a][b].color==-1){
		board_1.everypoint[a][b].color=board_1.number%2;
		count_of_five=five(a,b,&board_1);
	}
	return count_of_five;
}

int can_he_form_four(int a,int b,board *pboard){//判断落下对方棋子后能否形成四并返回活四+冲四的个数 
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

int can_he_form_livefour(int a,int b,board *pboard){//判断落下对方棋子后能否形成活四并返回活四的个数
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

int can_he_form_otherfour(int a,int b,board *pboard){//判断落下对方棋子后能否形成冲四并返回冲四的个数
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

int can_he_form_livethree(int a,int b,board *pboard){//判断落下对方棋子后能否形成三并返回活三的个数
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

int can_he_form_sleepthree(int a,int b,board *pboard){//判断落下对方棋子后能否形成眠三并返回眠三的个数
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
		return 0;	//一步不能胜返回0 
	else
		return 1;	//一步能胜返回1 
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
		bool can_go=false;//判断己方落完子后对方能否落子 
		int i,j,k,l;
		int pa=-1,pb=-1;
		f=sure_to_win(pboard,a,b);
		if(f==1){			//若单步必胜返回坐标 
			//printf("!\n!\n!\n%d:(%d,%d)1步必胜\n",n,*a,*b);
			return 1;
		}			//并结束计算
		for(i=0;i<15;i++)	//判断对方是否有四（即对方能否成五） ，若能成五结束计算 
			for(j=0;j<15;j++){
					f=can_he_form_five(i,j,pboard);
					if(f!=0){
						pa=i;pb=j; 
						if(a!=NULL&&b!=NULL){
							*a=pa;*b=pb;
						}
						//printf("%d:对方有四\n",n);
						return 0;
					}
			}
		for(i=0;i<15;i++)		//判断己方是否有活三（即能成活四），若有则落该点并返回该点坐标 
			for(j=0;j<15;j++){
				f=can_form_livefour(i,j,pboard);
				if(f!=0){
					if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==0){//如果是黑棋，有禁手 
						g=can_form_otherfour(i,j,pboard);
						h=can_form_livethree(i,j,pboard);
						x=can_form_overfive(i,j,pboard);
						if((f+g)<2&&h<2&&x==0){//如果落下后形成四但不是禁手,则返回坐标 
							pa=i;pb=j;
							if(a!=NULL&&b!=NULL){
								*a=pa;*b=pb;
							}
							//printf("!\n!\n!\n%d:(%d,%d)两步必胜\n",n,pa,pb);
							return 1;
						}
					}else{
						pa=i;pb=j;
						if(a!=NULL&&b!=NULL){
						*a=pa;*b=pb;
						}
						//printf("!\n!\n!\n%d:(%d,%d)两步必胜\n",n,pa,pb);
						return 1;
					} 
				}
			}
		for(i=0;i<15;i++)		//判断对方能否形成活四 
			for(j=0;j<15;j++){
				f=can_he_form_livefour(i,j,pboard);
				if(f!=0){
					if(pboard->gamemode.forbidden_moves==1&&(pboard->number%2)==1){//如果是黑棋，有禁手 
						g=can_he_form_otherfour(i,j,pboard);
						h=can_he_form_livethree(i,j,pboard);
						x=can_he_form_overfive(i,j,pboard);
						if((f+g)<2&&h<2&&x==0){//如果落下后形成四但不是禁手,则结束计算 
							if(a!=NULL&&b!=NULL){
								*a=pa;*b=pb;
							}
							//printf("%d:对方有活三（己方没有活三,双方都没有四）\n",n);
							return 0;
						}
					}else{ 
						if(a!=NULL&&b!=NULL){
						*a=pa;*b=pb;
						}
						//printf("%d:对方有活三（双方都没有四）\n",n);
						return 0;
					} 
				}
			}
		for(i=0;i<15;i++)								//遍历所有点，判断能否形成活三或冲四 
			for(j=0;j<15;j++){
				 forbidden_moves=can_form_forbidden_moves(i,j,pboard);
				 if(!forbidden_moves){//如果落下后没有形成禁手
				 	f=can_form_livethree(i,j,pboard);
					g=can_form_otherfour(i,j,pboard);
					if(f>0||g>0){//如果可以形成活三或冲四
						//落己方子
						board_1=*pboard;
						board_1.number++;
						board_1.everypoint[i][j].color=board_1.number%2;
						board_1.everypoint[i][j].number=board_1.number;
						//落对方子
						if(g>0){//如果己方落的是冲四点 
							for(k=0;k<15;k++)			//判断每个点己方是否成五 
								for(l=0;l<15;l++){
									h=can_he_form_five(k,l,&board_1);
									if(h>0){//如果己方能成五 
										//如果对方是禁手
										forbidden_moves_2=can_form_forbidden_moves(k,l,&board_1); 
										if(forbidden_moves_2){
											pa=i;pb=j;
											if(a!=NULL&&b!=NULL){
												*a=pa;*b=pb;
											}
											//printf("!\n!\n!\n%d:(%d,%d)冲四逼禁手\n",n,pa,pb);
											return 1;
										}
										//落对方子 
										board_2=board_1;
										board_2.number++;
										board_2.everypoint[k][l].color=board_2.number%2;
										board_2.everypoint[k][l].number=board_2.number;
										//判断n-1步是否必胜
										x=steps_to_win(&board_2,n-1);
										if(x==1){//如果n-1步必胜，返回（i，j） 
											pa=i;pb=j;
											if(a!=NULL&&b!=NULL){
												*a=pa;*b=pb;
											}
											//printf("!\n!\n!\n%d:(%d,%d)冲四获胜(如果n=2则是双四获胜)\n",n,pa,pb);
											return 1;
										} 
									}
								} 
							//printf("%d:有己方冲四点(%d，%d)可落，但不能必胜\n",n,i,j);
						}else{//如果落下后没有形成冲四但形成了活三 
							can_go=false;
							for(k=0;k<15;k++)//判断每个点己方能否成活四或对方能否成冲四 
								for(l=0;l<15;l++){
									h=can_he_form_livefour(k,l,&board_1);
									x=can_form_otherfour(k,l,&board_1);
									if(h>0||x>0){//如果己方能成活四或对方能成冲四 
										forbidden_moves_2=can_form_forbidden_moves(k,l,&board_1); 
										if(!forbidden_moves_2){//如果对方不是禁手 
											can_go=true;//对方有可落点
											//落对方子 
											board_2=board_1;
											board_2.number++;
											board_2.everypoint[k][l].color=board_2.number%2;
											board_2.everypoint[k][l].number=board_2.number;
											//判断n-步是否必胜 
											y=steps_to_win(&board_2,n-1);
											if(y==1){//如果n-1步必胜 
												pa=i;pb=j;
												if(a!=NULL&&b!=NULL){
													*a=pa;*b=pb;
												}
												//printf("!\n!\n!\n%d:(%d,%d)可下但不可防的活三\n",n,i,j);
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
									//printf("!\n!\n!\n%d:(%d,%d)不可下不可防的活三\n",n,i,j);
									return 1;
								}	
						}
					} 
				 }
			}
		if(a!=NULL&&b!=NULL){
			*a=-1;*b=-1;
		}
		//printf("%d:未找到必胜点\n",n);
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
	//价值初始化 
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
			for(i=0;i<15;i++)//给最大价值的点赋上序号 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].number==1){
						r++;
						value_of_everypoint[i][j].random_number=r;
					}
				}
			random=rand();
			r=random%r+1;//给r赋一个随机数
			for(i=0;i<15;i++)//根据随机数返回坐标 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].random_number==r){
						*a=i;*b=j;
					}
				}
			return; 
		}
	}
	if(pboard->number>=6){//如果棋盘现有总子数大于等于6 
		if(pboard->gamemode.level==2){
			steps_to_win(pboard,7,a,b);//判断13步内是否必胜
			if(*a!=-1)//如果必胜则返回坐标并结束函数 
				return;
		}else{
			steps_to_win(pboard,3,a,b);//判断5步内是否必胜
			if(*a!=-1)//如果必胜则返回坐标并结束函数 
			return;
		}
	}//如果总子数大于等于6且13步不能必胜活总子数为5则进行如下运算 

	bool is_here_livethree=false;
	int f,g,h,k,l,m;
	board board_1;
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			f=can_he_form_livefour(i,j,pboard);//如果对方能成活四 (即对方有活三)
			if(f>0){
				is_here_livethree=true;
				value_of_everypoint[i][j].number=1;//激活该点
				give_value(&(value_of_everypoint[i][j]),pboard);//给价值结构体赋值
				board_1=*pboard;
				board_1.number++;
				board_1.everypoint[i][j].color=board_1.number%2;//判断落此点后对方是否必胜 
				value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//如果对方必胜则最后考虑 
				//计算该点价值
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//如果禁手则价值为1
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
		for(i=0;i<15;i++)//遍历所有点,求出最大的价值 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].number==1){//如果该点被激活，即对方能成活四的点（即活三的两端) 
					value=value_of_everypoint[i][j].value;
					if(value>value_max)//比较价值 
						value_max=value;
				}
			}
		for(i=0;i<15;i++)//给最大价值的点赋上序号 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].value==value_max){
					r++;
					value_of_everypoint[i][j].random_number=r;
				}
			}
		random=rand();
		r=random%r+1;//给r赋一个随机数
		for(i=0;i<15;i++)//根据随机数返回坐标 
			for(j=0;j<15;j++){
				if(value_of_everypoint[i][j].random_number==r){
					*a=i;*b=j;
				}
			}
		return; 
	}
	int my_points=0;
	int his_points=0;//用于记录己方和对方能成活三或冲四点的个数 
	if(pboard->number>10){
	for(i=0;i<15;i++)//遍历每个点，统计能成活三冲四点的个数，激活并赋值能成活三或冲四的点 
		for(j=0;j<15;j++){
			f=can_form_livethree_1(i,j,pboard);
			g=can_form_livethree_2(i,j,pboard);
			h=can_form_otherfour(i,j,pboard);
			k=can_he_form_livethree_1(i,j,pboard);
			l=can_he_form_livethree_2(i,j,pboard);
			m=can_he_form_otherfour(i,j,pboard);
			if((f+g+h)>0){//如果己方能成活三或冲四 
				if((f+g)<2&&h<2){//如果不是禁手 
					my_points++;//统计数量+1 
					value_of_everypoint[i][j].number_attack=1;//己方进攻点被激活为1 
					give_value(&(value_of_everypoint[i][j]),pboard);//给价值结构体赋值
					board_1=*pboard;
					board_1.number++;
					board_1.everypoint[i][j].color=board_1.number%2;//判断落此点后对方是否必胜 
					value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//如果对方必胜则最后考虑 
					//计算该点价值
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//如果禁手则价值为1
				else //进攻点价值不考虑对方活二 
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
			if((k+l+m)>0){//对方 
				if((k+l)<2&&m<2){
					his_points++;
					value_of_everypoint[i][j].number_defence=1;//对方防御点被激活 
					give_value(&(value_of_everypoint[i][j]),pboard);//给价值结构体赋值
					board_1=*pboard;
					board_1.number++;
					board_1.everypoint[i][j].color=board_1.number%2;//判断落此点后对方是否必胜 
					value_of_everypoint[i][j].he_can_win=steps_to_win(&board_1,2);//如果对方必胜则最后考虑 
					//计算该点价值
				if(value_of_everypoint[i][j].forbidden_moves==true)
					value_of_everypoint[i][j].value=1;//如果禁手则价值为1
				else //防守点价值不考虑对方活二 
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
	if(my_points!=0||his_points!=0){//如果存在可下点 
		if(pboard->number%2==0){//黑棋主进攻 
			if(my_points!=0){
				if((my_points-his_points)>=-2){//比较己方点价值 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//遍历所有点 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1){//如果进攻点被激活
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//比较价值 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//给最大价值的点赋上序号 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
							//如果是进攻点并且价值最大
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//给r赋一个随机数
					for(i=0;i<15;i++)//根据随机数返回坐标 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}else{//比较对方点价值 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//遍历所有点 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1){//如果防守点被激活
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//比较价值 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//给最大价值的点赋上序号 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
							//如果是防守点并且价值最大 
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//给r赋一个随机数
					for(i=0;i<15;i++)//根据随机数返回坐标 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}
			}else{//比较对方点价值 
				value=0;value_max=0;
				r=0;
				for(i=0;i<15;i++)//遍历所有点 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_defence==1){//如果防守点被激活
							value=value_of_everypoint[i][j].value;
							if(value>value_max)//比较价值 
								value_max=value;
						}
					}
				for(i=0;i<15;i++)//给最大价值的点赋上序号 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
						//如果是防守点并且价值最大 
							r++;
							value_of_everypoint[i][j].random_number=r;
						}
					}
				random=rand();
				r=random%r+1;//给r赋一个随机数
				for(i=0;i<15;i++)//根据随机数返回坐标 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].random_number==r){
							*a=i;*b=j;
						}
					}
				return; 
			}
		}else{//白 
			if(his_points!=0){
				if((his_points-my_points)>=-2){//比较对方点价值 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//遍历所有点 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1){//如果防守点被激活
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//比较价值 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//给最大价值的点赋上序号 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_defence==1&&value_of_everypoint[i][j].value==value_max){
							//如果是防守点并且价值最大 
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//给r赋一个随机数
					for(i=0;i<15;i++)//根据随机数返回坐标 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}else{//比较己方点价值 
					value=0;value_max=0;
					r=0;
					for(i=0;i<15;i++)//遍历所有点 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1){//如果进攻点被激活
								value=value_of_everypoint[i][j].value;
								if(value>value_max)//比较价值 
									value_max=value;
							}
						}
					for(i=0;i<15;i++)//给最大价值的点赋上序号 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
							//如果是进攻点并且价值最大
								r++;
								value_of_everypoint[i][j].random_number=r;
							}
						}
					random=rand();
					r=random%r+1;//给r赋一个随机数
					for(i=0;i<15;i++)//根据随机数返回坐标 
						for(j=0;j<15;j++){
							if(value_of_everypoint[i][j].random_number==r){
								*a=i;*b=j;
							}
						}
					return; 
				}
			}else{//比较己方点价值 
				value=0;value_max=0;
				r=0;
				for(i=0;i<15;i++)//遍历所有点 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_attack==1){//如果进攻点被激活
							value=value_of_everypoint[i][j].value;
							if(value>value_max)//比较价值 
								value_max=value;
						}
					}
				for(i=0;i<15;i++)//给最大价值的点赋上序号 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].number_attack==1&&value_of_everypoint[i][j].value==value_max){
						//如果是进攻点并且价值最大
							r++;
							value_of_everypoint[i][j].random_number=r;
						}
					}
				random=rand();
				r=random%r+1;//给r赋一个随机数
				for(i=0;i<15;i++)//根据随机数返回坐标 
					for(j=0;j<15;j++){
						if(value_of_everypoint[i][j].random_number==r){
							*a=i;*b=j;
						}
					}
				return; 
			}
		}
	}
	if(my_points==0&&his_points==0){//如果己方和对方点数都为0
		int count_of_two=0;//记录能形成活二的点个数 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				f=can_form_sleepthree(i,j,pboard);//如果能成活二或眠三 
				g=can_form_livetwo_true(i,j,pboard);
				h=can_form_livetwo_false(i,j,pboard);
				k=can_he_form_sleepthree(i,j,pboard); 
				l=can_he_form_livetwo_true(i,j,pboard);
				m=can_he_form_livetwo_false(i,j,pboard);
				if((f+g+h+k+l+m)>0){
					count_of_two++;
					value_of_everypoint[i][j].number=1;//激活该点
					give_value(&(value_of_everypoint[i][j]),pboard);
					if(pboard->number>10)
						if(value_of_everypoint[i][j].forbidden_moves==true)
							value_of_everypoint[i][j].value=1;//如果禁手则价值为1
						else 
							value_of_everypoint[i][j].value=100+(f+g+h+k+l+m);
					if(pboard->number<=10){
						if(pboard->number<5)
							//计算该点价值
							if(value_of_everypoint[i][j].forbidden_moves==true)
								value_of_everypoint[i][j].value=1;//如果禁手则价值为1
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
								value_of_everypoint[i][j].value=1;//如果禁手则价值为1
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
		if(count_of_two!=0){//如果有活二或者眠三点 
			value=0;value_max=0;
			r=0;
			for(i=0;i<15;i++)//遍历所有点,求出最大的价值 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].number==1){//如果该点被激活
						value=value_of_everypoint[i][j].value;
						if(value>value_max)//比较价值 
							value_max=value;
					}
				}
			for(i=0;i<15;i++)//给最大价值的点赋上序号 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].value==value_max){
						r++;
						value_of_everypoint[i][j].random_number=r;
					}
				}
			random=rand();
			r=random%r+1;//给r赋一个随机数
			for(i=0;i<15;i++)//根据随机数返回坐标 
				for(j=0;j<15;j++){
					if(value_of_everypoint[i][j].random_number==r){
						*a=i;*b=j;
					}
				}
			return; 
		}
		if(count_of_two==0){//如果没有活二点 
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
	//重新打印棋盘 
	putimage(0,0,img_board);			// 在左侧打印棋盘 
	for(i=0;i<15;i++)
		for(j=0;j<15;j++){
			if(board1.everypoint[i][j].color==1)
				putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//打印黑子
			if(board1.everypoint[i][j].color==0)
				putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//打印白子 
		}
}

void two_players(){	//双人对弈 
	bool f=true;			//f用于判断棋局是否结束(true为未结束，flase为结束)
	int x=0,y=0;			//鼠标点击位置 
	int a=0,b=0;			//棋盘上的坐标 
	int count_of_five=0;	//记录（a，b）点形成5的个数 
	FILE *fp;int c=0;				//保存用 
	
	int count_of_livefour=0;
	int count_of_otherfour=0;
	int count_of_overfive=0;
	int count_of_livethree=0;
	int count_of_sleepthree=0;
	int count_of_livethree_1=0;
	int count_of_livethree_2=0;
	int count_of_livetwo_true=0;
	int count_of_livetwo_false=0;
	
	int ender=-1;			//最后一子颜色（1：黑棋，0：白棋） 
	bool forbidden_moves=false;//判断是否禁手 
	

	while(f){
		if(board1.gamemode.player==1&&(board1.gamemode.first==board1.number%2)){//如果轮到电脑下 
			best_point(&board1,&a,&b);
			put_board(a,b,&board1);		//将子落在点（a，b）上 
			//初始化数据 
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
			//计算 
			count_of_five=five(a,b,&board1);							//求形成5的个数
			count_of_overfive=overfive(a,b,&board1);
			four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
			three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
			count_of_livethree_1=livethree_1(a,b,&board1);
			count_of_livethree_2=livethree_2(a,b,&board1);
			count_of_livetwo_true=livetwo_true(a,b,&board1);
			count_of_livetwo_false=livetwo_false(a,b,&board1);
			//检测结果是否正确 
			printf("点(%d,%d)\n长连：%4d\n五  ：%4d\n活四：%4d\n冲四：%4d\n活三：%4d\n眠三：%4d\n",a,b,
			count_of_overfive,count_of_five,
			count_of_livefour,count_of_otherfour,
			count_of_livethree,count_of_sleepthree);
			printf("活三-1：%4d\n活三-2：%4d\n真活二：%4d\n假活二：%4d\n",
			count_of_livethree_1,
			count_of_livethree_2,
			count_of_livetwo_true,
			count_of_livetwo_false);
			
			if(board1.everypoint[a][b].number==board1.number){			//如果所点棋子是最后落下的 
			//判定胜负 
				if(count_of_five!=0){		//如果连成5个本局结束 
					ender=board1.number%2;
					f=false;	
				}else{						//如果没成五 
					if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//如果有禁手且是黑棋 
						if(	(count_of_livefour+count_of_otherfour)>1||		//如果"双四""双三"或"长连" 
							count_of_livethree>1||count_of_overfive>0){
							ender=0;
							forbidden_moves=true;
							f=false;						//白子胜利，且胜利方式为黑方禁手 
						}
					}
				}
			}
		}else{
		click(&x,&y);
		if(x<590&&y<590&&x>10&&y>10){									//点击到棋盘区域 
			a=((float)x-(25-(float)550/28))/((float)550/14);
			b=((float)y-(25-(float)550/28))/((float)550/14);
			put_board(a,b,&board1);		//将子落在点（a，b）上 
			
			//初始化数据 
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
			//计算 
			count_of_five=five(a,b,&board1);							//求形成5的个数
			count_of_overfive=overfive(a,b,&board1);
			four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
			three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
			count_of_livethree_1=livethree_1(a,b,&board1);
			count_of_livethree_2=livethree_2(a,b,&board1);
			count_of_livetwo_true=livetwo_true(a,b,&board1);
			count_of_livetwo_false=livetwo_false(a,b,&board1);
			//检测结果是否正确 
			printf("点(%d,%d)\n长连：%4d\n五  ：%4d\n活四：%4d\n冲四：%4d\n活三：%4d\n眠三：%4d\n",a,b,
			count_of_overfive,count_of_five,
			count_of_livefour,count_of_otherfour,
			count_of_livethree,count_of_sleepthree);
			printf("活三-1：%4d\n活三-2：%4d\n真活二：%4d\n假活二：%4d\n",
			count_of_livethree_1,
			count_of_livethree_2,
			count_of_livetwo_true,
			count_of_livetwo_false);
			
			if(board1.everypoint[a][b].number==board1.number){			//如果所点棋子是最后落下的 
			//判定胜负 
				if(count_of_five!=0){		//如果连成5个本局结束 
					ender=board1.number%2;
					f=false;	
				}else{						//如果没成五 
					if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//如果有禁手且是黑棋 
						if(	(count_of_livefour+count_of_otherfour)>1||		//如果"双四""双三"或"长连" 
							count_of_livethree>1||count_of_overfive>0){
							ender=0;
							forbidden_moves=true;
							f=false;						//白子胜利，且胜利方式为黑方禁手 
						}
					}
				}
			}
		}
		if(x>=630&&x<720){												//点击到按钮 
				if(y>=60&&y<90){							//提示 
					best_point(&board1,&a,&b);
					put_board(a,b,&board1);		//将子落在点（a，b）上 
					//初始化数据 
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
					//计算 
					count_of_five=five(a,b,&board1);							//求形成5的个数
					count_of_overfive=overfive(a,b,&board1);
					four(a,b,&board1,&count_of_livefour,&count_of_otherfour);
					three(a,b,&board1,&count_of_livethree,&count_of_sleepthree);
					count_of_livethree_1=livethree_1(a,b,&board1);
					count_of_livethree_2=livethree_2(a,b,&board1);
					count_of_livetwo_true=livetwo_true(a,b,&board1);
					count_of_livetwo_false=livetwo_false(a,b,&board1);
					//检测结果是否正确 
					printf("点(%d,%d)\n长连：%4d\n五  ：%4d\n活四：%4d\n冲四：%4d\n活三：%4d\n眠三：%4d\n",a,b,
					count_of_overfive,count_of_five,
					count_of_livefour,count_of_otherfour,
					count_of_livethree,count_of_sleepthree);
					printf("活三-1：%4d\n活三-2：%4d\n真活二：%4d\n假活二：%4d\n",
					count_of_livethree_1,
					count_of_livethree_2,
					count_of_livetwo_true,
					count_of_livetwo_false);
					
					if(board1.everypoint[a][b].number==board1.number){			//如果所点棋子是最后落下的 
					//判定胜负 
						if(count_of_five!=0){		//如果连成5个本局结束 
							ender=board1.number%2;
							f=false;	
						}else{						//如果没成五 
							if(board1.gamemode.forbidden_moves==1&&board1.everypoint[a][b].color==1){	//如果有禁手且是黑棋 
								if(	(count_of_livefour+count_of_otherfour)>1||		//如果"双四""双三"或"长连" 
									count_of_livethree>1||count_of_overfive>0){
									ender=0;
									forbidden_moves=true;
									f=false;						//白子胜利，且胜利方式为黑方禁手 
								}
							}
						}
					}
				}
				if(y>=120&&y<150){							//悔棋 
					huiqi();
				}
				if(y>=180&&y<210){							//保存 
					setfillcolor(WHITE);
					bar(600,0,750,600);					//清空右侧 
					setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
					setcolor(EGERGB(0x0, 0x0, 0x0));
					setfont(22, 0, "宋体");
					setbkmode(TRANSPARENT);
					settextjustify(1,1);
					bar(630, 60, 720, 90);
					outtextxy(675, 75, "存档1");
					bar(630, 120, 720, 150);
					outtextxy(675, 135, "存档2");
					bar(630, 180, 720, 210);
					outtextxy(675, 195, "存档3");
					bar(630, 240, 720, 270);
					outtextxy(675, 255, "存档4");
					bar(630, 300, 720, 330);
					outtextxy(675, 315, "存档5");
					bar(630, 360, 720, 390);
					outtextxy(675, 375, "存档6");
					
					while(c==0){				//点击直到按钮 
						click(&x,&y);
						if(x>=630&&x<720){
							if(y>=60&&y<90) {fp=fopen("save\\save1.dat","wb");c=1;}
							if(y>=120&&y<150) {fp=fopen("save\\save2.dat","wb");c=2;}
							if(y>=180&&y<210) {fp=fopen("save\\save3.dat","wb");c=3;}
							if(y>=240&&y<270) {fp=fopen("save\\save4.dat","wb");c=4;}
							if(y>=300&&y<330) {fp=fopen("save\\save5.dat","wb");c=5;}
							if(y>=360&&y<390) {fp=fopen("save\\save6.dat","wb");c=6;}
						}
					}							//点击结束
					c=0;
					fwrite(&board1,sizeof(board),1,fp);			//将棋局写入存档 
					fclose(fp);
					//还原界面 
					setfillcolor(WHITE);
					bar(600,0,750,600);					//清空右侧 
					setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
					setcolor(EGERGB(0x0, 0x0, 0x0));
					setfont(22, 0, "宋体");
					setbkmode(TRANSPARENT);
					settextjustify(1,1);
					bar(630, 60, 720, 90);
					outtextxy(675, 75, "提示");
					bar(630, 120, 720, 150);
					outtextxy(675, 135, "悔棋");
					bar(630, 180, 720, 210);
					outtextxy(675, 195, "保存");
					bar(630, 240, 720, 270);
					outtextxy(675, 255, "回主菜单");
				}
				if(y>=240&&y<270){							//回主菜单 
					return;				//棋局结束 
				}
			}
		}
	}
	

	//胜利动画 
	setcolor(0xFF0000);
	setfont(35,0,"宋体");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	if(ender==1)
		outtextxy(300, 300, "黑棋胜！");
	else if(forbidden_moves)
			outtextxy(300, 300, "黑方禁手，白棋胜！");
		else
			outtextxy(300, 300, "白棋胜！");
	setfillcolor(WHITE);
	//退出游戏 
	bar(600,0,750,600);					//清空右侧 
	setfillcolor(EGERGB(0xD3, 0xD3, 0xD3));
	setcolor(EGERGB(0x0, 0x0, 0x0));
	setfont(22, 0, "宋体");
	setbkmode(TRANSPARENT);
	settextjustify(1,1);
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "回主菜单");
	while(c==0){				//点击直到按钮 
		click(&x,&y);
		if(x>=630&&x<720){
			if(y>=60&&y<90) c=1;
		}
	}							//点击结束
}

void playing(menu mode){
	printf("start game!!!\n");
	//打印对局界面 
	print_game();
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "提示");
	bar(630, 120, 720, 150);
	outtextxy(675, 135, "悔棋");
	bar(630, 180, 720, 210);
	outtextxy(675, 195, "保存");
	bar(630, 240, 720, 270);
	outtextxy(675, 255, "回主菜单");
	//初始化棋局信息 
	if(mode.player!=-1){						//如果不是复盘 
		board1.gamemode=mode;
		board1.number=0;
		int i,j;
		for(j=0;j<15;j++)
			for(i=0;i<15;i++){
				board1.everypoint[j][i].color=-1;
				board1.everypoint[j][i].number=0;
			}
	}else{												//如果是复盘 
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
		//打印初始棋盘 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				if(board1.everypoint[i][j].color==1)
					putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//打印黑子
				if(board1.everypoint[i][j].color==0)
					putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//打印白子 
			}
	
	}
	
	//检测
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
		//打印初始棋盘 
		putimage(0,0,img_board);			// 在左侧打印棋盘 
		for(i=0;i<15;i++)
			for(j=0;j<15;j++){
				if(board_1.everypoint[i][j].color==1)
					putimage_transparent(NULL,img_black,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,BLACK);	//打印黑子
				if(board_1.everypoint[i][j].color==0)
					putimage_transparent(NULL,img_white,(25-(float)550/28)+((float)550/14)*i,(25-(float)550/28)+((float)550/14)*j,0xF6F6F6);//打印白子 
			}
	//设置字体信息 
	setcolor(0xFF0000);
	setfont(25,0,"宋体");
	setbkmode(TRANSPARENT);
	settextjustify(0,0);
	switch(n){
		case 1:	outtextxy(30,30,"左上角坐标为（0，0），右下角坐标为（14，1");
				outtextxy(30,60,"4），示例中所有坐标均以此为基准");
				outtextxy(30,90,"同色棋子连成5个以上（不包括5个）即为长连");
				break;
		case 2:	outtextxy(30,30,"同色棋子连成4个并且两端都能成5或长连（如");
				outtextxy(30,60,"果是禁手则不包括长连）的为活四。如果某点");
				outtextxy(30,90,"没形成活四，若其8个方向非同色子的8个最近");
				outtextxy(30,120,"点落下同色子能形成n个5或长连(如果是禁手");
				outtextxy(30,150,"则不包括长连）则称该点参与形成了n个冲四"); 
				outtextxy(30,180,"例：（7，6）形成了1个活四，（7，7）形成");
				outtextxy(30,210,"了一个冲四而（7，9）形成了2个冲四");
				break;
		case 3:	outtextxy(30,30,"如果某点在4个方向（左上到右下，上到下，左");
				outtextxy(30,60,"下到右上）中，n个方向的两端最近非同色点中");
				outtextxy(30,90,"有一个落下同色子后能形成活四，则称该点为参");
				outtextxy(30,120,"与形成了n个活三");
				outtextxy(30,150,"例：（7，7），（8，7）都形成了一个活三");
				break;
		case 4:	outtextxy(30,30,"落下后能形成长连的点为禁手点");
				outtextxy(30,60,"例：（7，7）为禁手点");
				break;
		case 5:	outtextxy(30,30,"落下后能同时形成两个活四或冲四的点为禁手点");
				outtextxy(30,60,"例：（7，7）为禁手点");
				break;
		case 6:	outtextxy(30,30,"落下后能同时形成两个活三的点为禁手点");
				outtextxy(30,60,"例：（7，7）为禁手点");
				break;
	}
}

void rule_of_forbidden_moves(){
	//打印对局界面 
	print_game();
	bar(630, 60, 720, 90);
	outtextxy(675, 75, "下一页");
	print_page(1);
	int page=1;
	int x,y;
	while(page<7){				//若点击到按钮，打印下一页 
		click(&x,&y);
		if(x>=630&&x<720)
			if(y>=60&&y<90){
				page++;
				if(page<7)
					print_page(page);
			}
	}							//当页数大于6回主界面 
}

int main(){
	initgraph(750,600,0);						//创建窗口 750*600 
	setcaption("五子棋");					//设置窗口标题
	setbkcolor(WHITE);							//设置背景为白色
	material();									//获取素材 
	//测试中 
	menu mode;	//储存游戏模式 
	int x=-20,y=-20;	//记录鼠标点击位置 
	int a=0;			//记录点击的按钮 
	do{
		//结构体初始化 
		mode.player=0;
		mode.forbidden_moves=-1;
		mode.first=-1;
		mode.level=0;
		beginning(&mode);
		//初始界面（双人对弈，人机对弈，读取记录，禁手规则，结束游戏） 
		a=0;			//a为按钮位置 
		while(a==0){				//点击直到按钮 
			click(&x,&y);
			if(x>=630&&x<720){
				if(y>=60&&y<90) a=1;
				if(y>=120&&y<150) a=2;
				if(y>=180&&y<210) a=3;
				if(y>=240&&y<270) a=4;
				if(y>=300&&y<330) a=5;
			}
		}							//点击结束
		//根据按钮操作
		if(a==5) end=1;	//游戏结束 
		//双人对弈 
		if(a==1){
			mode.player=2;			//棋局信息-玩家数更改为2 
			beginning(&mode);
			a=0;
			while(a==0){				//点击直到按钮 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//点击结束
			//设置有无禁手 
			if(a==1) mode.forbidden_moves=1;
			if(a==2) mode.forbidden_moves=0;
			playing(mode);				//模式选定后开始游戏 
			a=0;						//按钮a初始化 
		}
		//人机对弈 
		if(a==2){
			mode.player=1;			//棋局信息-玩家数更改为1
			beginning(&mode);
			a=0;
			while(a==0){				//点击直到按钮 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//点击结束
			//设置有无禁手 
			if(a==1) mode.forbidden_moves=1;
			if(a==2) mode.forbidden_moves=0;
			beginning(&mode);
			a=0;
			while(a==0){				//点击直到按钮 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//点击结束
			//设置先后手
			if(a==1) mode.first=1;
			if(a==2) mode.first=0;
			beginning(&mode);
			a=0;
			while(a==0){				//点击直到按钮 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
				}
			}							//点击结束
			//设置难度
			if(a==1) mode.level=1;
			if(a==2) mode.level=2;
			playing(mode);				//模式选定后开始游戏 
			a=0;						//按钮a初始化 
		}
		//读取记录 
		if(a==3){
			mode.player=-1;
			beginning(&mode);
			a=0;
			while(a==0){				//点击直到按钮 
				click(&x,&y);
				if(x>=630&&x<720){
					if(y>=60&&y<90) a=1;
					if(y>=120&&y<150) a=2;
					if(y>=180&&y<210) a=3;
					if(y>=240&&y<270) a=4;
					if(y>=300&&y<330) a=5;
					if(y>=360&&y<390) a=6;
				}
			}							//点击结束
			mode.level=a;
			playing(mode);
			a=0;
		}
		//禁手规则 
		if(a==4){
			rule_of_forbidden_moves();
		}
	}while(end==0);
	
	//关闭棋盘，黑子，白子图像
	delimage(img_board);
	delimage(img_black);
	delimage(img_white);
	//关闭窗口 
	closegraph();
}
