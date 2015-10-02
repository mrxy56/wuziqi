#include <graphics.h> 
#ifndef watereffect_h 
#define watereffect_h 
class watereffect{
public:
	int PIC_HEIGHT;
	int PIC_WIDTH;
	IMAGE *src_img;										
	IMAGE *dest_img;	
	DWORD *img_ptr1;								
	DWORD *img_ptr2;
	short *buf;
	short *buf2;
	int centre_x;
	int centre_y;
	int dec;
	void init(IMAGE &b,int height,int width,int d){
		PIC_WIDTH=width;
		PIC_HEIGHT=height;
		dest_img=new IMAGE(width,height);
		buf  = new short[PIC_HEIGHT*PIC_WIDTH+PIC_WIDTH];
		buf2 = new short[PIC_HEIGHT*PIC_WIDTH+PIC_WIDTH];
		src_img=new IMAGE(b);
		dec=d;
		centre_x=height/2;
		centre_y=width/2;
	}
	void nextFrame()
	{
		for(int i = 0; i < PIC_HEIGHT*(PIC_WIDTH); i++)
		{
			int x1,x2,x3,x4;
			if(i%PIC_WIDTH==0){
				x1=buf[i];
			}else{
				x1=buf[i-1];
			}
			if(i%PIC_WIDTH==PIC_WIDTH-1){
				x2=buf[i];
			}else{
				x2=buf[i+1];
			}
			if(i<PIC_WIDTH){
				x3=buf[i];
			}else{
				x3=buf[i-PIC_WIDTH];
			}
			if(i>(PIC_HEIGHT-1)*PIC_WIDTH){
				x4=buf[i];
			}else{
				x4=buf[i+PIC_WIDTH];
			}
			buf2[i] = ((x1+x2+x3+x4) >> 1) - buf2[i];	
			buf2[i] -= buf2[i] >> dec;
		}
		short *ptmp = buf;
		buf  = buf2;
		buf2 = ptmp;
	}
	void Render()
	{
		int i = 0;
		img_ptr1 = GetImageBuffer(src_img);
		img_ptr2 = GetImageBuffer(dest_img);
		for (int y = 0; y < PIC_HEIGHT; y++) 
		{
			for (int x = 0; x < PIC_WIDTH; x++) 
			{
				short data = 1024 - buf[i];
				int a = ((x - centre_x) * data / 1024) + centre_x;
				int b = ((y - centre_y) * data / 1024) + centre_y;
				if(b<PIC_HEIGHT&&b>=0&&a<PIC_WIDTH&&a>=0){
					img_ptr2[i] = img_ptr1[a + (b * PIC_WIDTH)];//Í¸¾µÐ§¹û£¡
				}else{
					img_ptr2[i]=RGB(255,255,255);
				}

				i++;
				
			}
		}
	}
	void disturb(int x, int y, int stonesize, int stoneweight) 
	{
		if ((x >= PIC_WIDTH - stonesize) ||
			(x < stonesize) ||
			(y >= PIC_HEIGHT - stonesize) ||
			(y < stonesize))
			return;
		
		for (int posx=x-stonesize; posx<x+stonesize; posx++)
		{
			for (int posy=y-stonesize; posy<y+stonesize; posy++)
			{
				if ((posx-x)*(posx-x) + (posy-y)*(posy-y) < stonesize*stonesize)
				{
					buf[PIC_WIDTH*posy+posx] += stoneweight;
				}
			}
		}
	}
	IMAGE* getcurrentimage(){
		return dest_img;
	}
};
#endif