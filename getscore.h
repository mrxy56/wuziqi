#include <utility>
#include <windows.h>
using namespace std;
class GetScore{
public:
	int POW4[8],POW10[8];
	GetScore(){
		POW4[0] = 1;
		for(int i=1;i<8;i++)	POW4[i] = POW4[i-1] * 4;
		POW10[0] = 1;
		for(i=1;i<6;i++)	POW10[i] = POW10[i-1] * 10;
		loadtable1("table1.txt");
		loadtable2("table2.txt");
		size_x=15;
		size_y=15;
	}
	pair<int,int>  getpointscore(int x ,int y,int type,int table[][15]){//获取某一点的索引和对应的优先级
		int i;
		int sco=0;
		int temp=0;
		int tempcount=0;
		int index=0;
		int dy[4]={0,1,1,-1};
		int dx[4]={1,0,1,1};
		for(int t=0;t<4;t++){
			index=0;
			if(type==1){
				for(i=-4;i<0;i++){
					if(x+i*dx[t]>=0 && x+i*dx[t]<size_x && y+i*dy[t]>=0 && y+i*dy[t]<size_y){
						index+=table[x+i*dx[t]][y+i*dy[t]]*POW4[-i+3];
					}else{
						index+=getoppose(type)*POW4[-i+3];								
					}
				}
				for(i=1;i<5;i++){
					if(x+i*dx[t]>=0 && x+i*dx[t]<size_x && y+i*dy[t]>=0 && y+i*dy[t]<size_y){
						index+=table[x+i*dx[t]][y+i*dy[t]]*POW4[-i+4];
					}else{
						index+=getoppose(type)*POW4[-i+4];
					}
				}
			}else if(type==2){
				for(i=-4;i<0;i++){
					if(x+i*dx[t]>=0 && x+i*dx[t]<size_x && y+i*dy[t]>=0 && y+i*dy[t]<size_y){
						index+=getoppose(table[x+i*dx[t]][y+i*dy[t]])*POW4[-i+3];
					}else{
						index+=type*POW4[-i+3];
					}
				}
				for(i=1;i<5;i++){
					if(x+i*dx[t]>=0 && x+i*dx[t]<size_x && y+i*dy[t]>=0 && y+i*dy[t]<size_y){
						index+=getoppose(table[x+i*dx[t]][y+i*dy[t]])*POW4[-i+4];
					}else{
						index+=type*POW4[-i+4];
					}
				}
			}
			temp+=(matchtable[index]-96)*POW10[tempcount];
			tempcount++;
		}		
		return pair<int,int>(temp,matchtable2[temp]);
	}
	int getindexscore(int index){//获取索引对应的优先级
		return matchtable2[index];
	}
	void changescore(int index,int d){//改变table2里的优先级
		int x=matchtable2[index]+d;
		matchtable2[index]+=d;
	}
	void re_print(){//重新写回table2
		FILE * fp =fopen("table2.txt","w");
		if(fp!=NULL){
			for(int i=0;i<10000;i++){
				fprintf(fp,"%d,",matchtable2[i]);
			}
			fclose(fp);
		}
	}
	
private:
	int getoppose(int x){//12互换
		if(x==1){
			return 2;
		}else if(x==2){
			return 1;
		}else{
			return x;
		}
	}
	void loadtable1(char path[]){
		FILE * fp =fopen(path,"r");
		if(fp!=NULL){
			fscanf(fp,"%s",matchtable);
			fclose(fp);
		}else{
			MessageBox(0,"加载匹配表1失败！","五子棋",MB_OK);
			exit(0);
		}
	}
	void loadtable2(char path[]){
		FILE * fp =fopen(path,"r");
		if(fp!=NULL){
			for(int i=0;i<10000;i++){
				fscanf(fp,"%d,",&matchtable2[i]);
			}
			fclose(fp);
		}else{
			MessageBox(0,"加载匹配表2失败！","五子棋",MB_OK);
			exit(0);
		}
	};
	int size_x,size_y;
	char matchtable[65536];
	int matchtable2[10000];
};