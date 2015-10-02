#include <windows.h>
#include <easyx.h>
#include <math.h>
#include "watereffect.h"
#include "resource.h"
#ifndef drawboard_h
#define drawboard_h
HWND cxvvc;
class drawboard{
public:
	int (*luozi)[15];
	IMAGE back;
	IMAGE a1;
	IMAGE a2;
	IMAGE src;
	watereffect water;
	int flag;
	int pos_x;
	int pos_y;
	void init(int (**a)[15]){
			src.Resize(600,600);
			luozi=*a;
			int dx,dy;
			dx=40;
			dy=40;
			cxvvc=initgraph(600,600);
			//初始化棋盘
			//back为棋盘加横竖线的图
			loadimage(&back,"jpg",MAKEINTRESOURCE(IDR_JPG3),600,600);
			loadimage(&a1,"jpg",MAKEINTRESOURCE(IDR_JPG4),40,40);
			loadimage(&a2,"jpg",MAKEINTRESOURCE(IDR_JPG5),40,40);
			SetWorkingImage(&back);
			setlinestyle(PS_SOLID,NULL,2);
			setlinecolor(RGB(0,0,0));
			for(int i=0;i<=14;i++){
				line(20+dx*i,20,20+dx*i,580);
			}
			for( i=0;i<=14;i++){
				line(20,20+dy*i,580,20+dy*i);
			}
			setlinestyle(PS_SOLID,NULL,1);
			setlinecolor(RGB(0,0,0));
			setfillcolor(RGB(0,0,0));
			for( i=3;i<15;i+=4){
				for(int j=3;j<15;j+=4){
					fillrectangle(i*dx+20-4,j*dy+20-4,i*dx+20+3,j*dy+20+3);
				}
			}
			//初始化水波纹特效
			water.init(src,600,600,11);
			CreateThread(NULL,0,show,this,0,NULL);
			CreateThread(NULL,0,getmsg,this,0,NULL);

	}
	int Get_Pos(int &x,int &y){
		flag=0;
		while(1){
			if(flag==1){
				x=absaa(pos_x);
				y=absaa(pos_y);
				flag=0;
				
				if(luozi[x][y]==0){
					water.disturb(x*40+20,y*40+20,3,400);
					return 1;
				}
			}
			Sleep(10);
		}
	}

private:
	int absaa(int a){//除以40取整
		return(a/40);
	}
	void drawchess(DWORD *dest,DWORD *qizi,double x,double y){
		//显存中画棋子，只画圆形的部分，去掉白色的
		for(int i=0;i<40;i++){
			for(int j=0;j<40;j++){
				if(sqrt((i-19.5)*(i-19.5)+(j-19.5)*(j-19.5))<19){
					dest[(int)((y+j)*600+x+i)]=qizi[(int)(j*40+i)];
				}
			}
		}
	}
	void refresh(){
		//刷新棋盘状态，画出整个棋盘
		SetWorkingImage(&src);
		putimage(0,0,&back);
		DWORD *dest;
		DWORD *qizi;
		dest=GetImageBuffer(&src);
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
				if(luozi[i][j]==1){
					qizi=GetImageBuffer(&a1);
					drawchess(dest,qizi,i*40,j*40);
				}else if(luozi[i][j]==2){
					qizi=GetImageBuffer(&a2);
					drawchess(dest,qizi,i*40,j*40);
				}
			}
		}
	}
	static DWORD   WINAPI   show(LPVOID   lpParameter)
	{
		drawboard *p=(drawboard*)lpParameter;
		while(1){
			Sleep(1);
			p->refresh();
			SetWorkingImage();
			//画出棋盘后对棋盘应用水波纹特效
			p->water.src_img=&p->src;
			p->water.nextFrame();
			p->water.Render();
			//画出最终的图
			putimage(0,0,p->water.getcurrentimage());
			//putimage(0,0,&src);
		}
	}
	static DWORD   WINAPI getmsg(LPVOID   lpParameter){
		
		drawboard *p=(drawboard*)lpParameter;
		MOUSEMSG aaa;
		while(1){
			aaa=GetMouseMsg();
			if(aaa.uMsg==WM_MOUSEMOVE){
				char a[10];
				
				p->water.disturb(aaa.x,aaa.y,3,50);
				p->water.disturb(aaa.x,aaa.y,2,-100);
				p->water.centre_x=aaa.x;
				p->water.centre_y=aaa.y;
				FlushMouseMsgBuffer();
			}
			if(aaa.uMsg==WM_LBUTTONDOWN){
			//	char a[10];
			//	itoa(aaa.x,a,10);
			//	MessageBox(0,a,"aa",MB_OK);
				p->flag=1;
				p->pos_x=aaa.x;
				p->pos_y=aaa.y;
				FlushMouseMsgBuffer();
			}
		}

	}

};
#endif
