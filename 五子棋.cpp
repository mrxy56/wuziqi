#include<iostream>
#include<time.h>
#include<cstring>
#include<stdio.h>
#include<getscore.h>
#include<drawboard.h>
#include<map>
#include<algorithm>
using namespace std;

#define White 2
#define Black 1
#define AREA 0
#define INF 1e8
#define POINT pair <int,int>
#define STEP pair <int,pair<int,int> >

int COUNTS ;

GetScore Socre;
drawboard Draw;

void Easy_Sort(STEP step[],int index[],int len){
    for(int i=0;i<len;i++)
        while(i&&step[i].first>step[i-1].first){
            swap(step[i],step[i-1]);
            swap(index[i],index[i-1]);
        }
};

struct Game_Control{
private:
    int AI_RANK[3],WINER;
//====================���̵��ඨ��============================================

    struct Game_Board{
    private:
        int counts,TOT_PATH;
        int size;
        POINT path[40000];
    public:
        int board[15][15];
        int win;
        Game_Board(){};
        void Init(){
			size = 15;
            for(int i=0;i<size;i++)
                for(int j=0;j<size;j++) board[i][j] = AREA;
            counts = 0;
            TOT_PATH = 0;
            win = 3e5;
        }
        void Add_Path(int index,int d){
            path[TOT_PATH].first = index;
            path[TOT_PATH].second = d;
        }
        bool Is_Empty() {return !counts;}
        void Get_Board(int p[][15]){
            p = board;
        }
        int Size()  {return size;}
        void Set(int x,int y,int color){
            board[x][y] = color;
            counts ++;
        }
        void Re_Set(int x,int y){
            board[x][y] = AREA;
            counts --;
        };

        bool Is_Win(int x,int y,int color){
            POINT tmp = (Socre.getpointscore(x,y,color,board));
            return tmp.second >= 1e7;
        }

        void Return_Print(){        // ����һ������,�޸ķ�ֵ
            for(int i=TOT_PATH;i>0&&i>TOT_PATH-30;i--)
                Socre.changescore(path[i].first,path[i].second);
        }

        POINT Get_Point(int x,int y,int color){       // =======������=======
			POINT P1 = Socre.getpointscore(x,y,color,board);
			POINT P2 = Socre.getpointscore(x,y,(color==White?Black:White),board);
			P2.second /= 4;
			P1.second += P2.second;

			return P1;
        }

        bool Select(STEP step[],int index[],int &len,int color){        // ѡȡ12����ֵ���ĵ�
            const int LIM = 10;
            len = 0;
            for(int i=0;i<size;i++)
                for(int j=0;j<size;j++) if(board[i][j] == AREA){
                    POINT point = Get_Point(i,j,color);
                    if(len < LIM){
                        step[len].first = point.second;
                        index[len] = point.first;
                        step[len].second.first = i;
                        step[len++].second.second = j;
                    }
                    else if(point.second > step[len-1].first){
                        step[len-1].first = point.second;
                        index[len-1] = point.first;
                        step[len-1].second.first = i;
                        step[len-1].second.second = j;
                    }
                    int tmp = len-1;
                    while(tmp && step[tmp].first > step[tmp-1].first){
                        swap(step[tmp],step[tmp-1]);
                        swap(index[tmp],index[tmp-1]);
                        tmp --;
                    }
                }
            return len!=0;             //  �����ж��Ƿ���������
        }


		POINT TTT(int x,int y,int color){
			return Get_Point(x,y,color);
		}

    }BOARD,TRUE_BOARD;
// ===============================================================================

public:
    bool Openning(int color){       // �����������м�λ���������
        int x = rand()%(BOARD.Size()/2)+BOARD.Size()/4;
        int y = rand()%(BOARD.Size()/2)+BOARD.Size()/4;
        BOARD.Set(x,y,color);
		TRUE_BOARD.Set(x,y,color);
        return 1;
    }

    int MiniMax(int floor,int color,int lastbest,int last){       // ����Сֵ��������   ��ȡ��alpha-beta�Ż�
        STEP step[15];
        int len,index[15];
        if(!BOARD.Select(step,index,len,color))    return 0;    // ƽ�ַ���0
        if(!floor){         //  ��Ͳ�  ֱ�ӷ������ŵ÷�
            int Max = -INF;
            for(int i=0;i<len;i++)  if(step[i].first > Max)
                Max = step[i].first;
            return Max;
        }
        for(int i=0;i<len;i++)      //   �����ǰ�б�ʤ�߷�  �򷵻�
            if(step[i].first >= BOARD.win)  return step[i].first;
        int best = -INF;
        for( i=0;i<len;i++){
            BOARD.Set(step[i].second.first,step[i].second.second,color);
            int value = MiniMax(floor-1,(color==White?Black:White),best,step[i].first);
            BOARD.Re_Set(step[i].second.first,step[i].second.second);
            best = max(best,step[i].first - value);
            if(last-best <= lastbest)   return best;    // alpha-beta��ֽ
        }
        return best;
    }

    bool AI_Turn(int color){
        if(BOARD.Is_Empty()) {              //  �ж��Ƿ�Ϊ����   �ǵĻ�����ÿ��ֺ�������
            Openning(color);
            return 1;
        }
        STEP step[15];          // ���ӱ��� ��¼���ַ��� �����
        int len,index[15];      // index��¼·��for table2
        if(!BOARD.Select(step,index,len,color))   return 0;       // Ϊcolor��ѡ������  ����0��ʾ�����Ѿ��� ѡ��ʧ��

        //  ����Ƿ��б�Ӯ�߷�===========================================================================
        for(int i=0;i<len;i++)
            if(step[i].first >= BOARD.win){         // �߷��������� BOARD.win �б�ʤ�߷� �����ⲽ
                BOARD.Set(step[i].second.first,step[i].second.second,color);
				TRUE_BOARD.Set(step[i].second.first,step[i].second.second,color);
                if(BOARD.Is_Win(step[i].second.first,step[i].second.second,color))  WINER = color;  // �ж��Ƿ�ʤ�߲���
                return 1;
            }
        // ============================================================================================

        COUNTS = 0;

        for( i=0;i<len;i++){         // ģ��ÿ���߷��������
            BOARD.Set(step[i].second.first,step[i].second.second,color);
            step[i].first -= MiniMax(AI_RANK[color],(color==White?Black:White),INF,-INF);   // ���½���ģ��
            BOARD.Re_Set(step[i].second.first,step[i].second.second);                       // ����ģ��ۼ�
        }

        Easy_Sort(step,index,len);    // ���� ѡ������ߵ����ӷ���
        int flag = 0;
        BOARD.Set(step[flag].second.first,step[flag].second.second,color);
		TRUE_BOARD.Set(step[flag].second.first,step[flag].second.second,color);

        for(i=0;i<len;i++)
            if(i == flag)
                BOARD.Add_Path(index[i],-3);      // �������ѡ�񲽷����ȼ�
            else
                BOARD.Add_Path(index[i],1);
        return 1;
    }

    void Player_Turn(int color){
        int x,y;
        Draw.Get_Pos(x,y);

        BOARD.Set(x,y,color);
		TRUE_BOARD.Set(x,y,color);
        if(BOARD.Is_Win(x,y,color))  WINER = color;  // �ж��Ƿ�ʤ�߲���
    }

    void Running(int player){
        int turn;
        turn = Black;
        WINER = -1;
        while(WINER == -1){
            if(turn != player)
                AI_Turn(turn);   // ĳһ������ʧ��  ���ʾƽ��
            else    
			//	AI_Turn(turn);
				Player_Turn(player);
			Sleep(100);
            turn = (turn==White?Black:White);
		}
        if(WINER == White)
			MessageBox(0,"White Win","",MB_OK);
		if(WINER == Black)
			MessageBox(0,"Black Win","",MB_OK);
		if(WINER == player)
	        BOARD.Return_Print();     // ����ѧϰ
		if(MessageBox(0,"�Ƿ�����һ�֣�","",MB_YESNO)==IDYES){
			Begin();
		}else{
			exit(0);
		}
	
    }
    void Begin(){
		TRUE_BOARD.Init();
        BOARD.Init();
        
		int player;
		char s[5];
		if(MessageBox(cxvvc,"�����ƺ��ӣ�(��������)","",MB_YESNO)==IDYES){
			player = Black;
			while(1){
				InputBox(s, 5, "������AI�ȼ�(1~5)");
				AI_RANK[White] = atoi(s);
				if(AI_RANK[White]>=1 && AI_RANK[White]<=5)	break;
			}
		}else{
			player = White;
			while(1){
				InputBox(s, 5, "������AI�ȼ�(1~5)");
				AI_RANK[Black] = atoi(s);
				if(AI_RANK[Black]>=1 && AI_RANK[Black]<=5)	break;
			}
		}
        Running(player);
    };

    void TEST(){
            BOARD.Init();
			int (*p)[15]=BOARD.board;
		    Draw.init(&p);
			int t = 1;
			WINER = -1;
			while(t++){
				Player_Turn(1+t%2);
				if(WINER != -1)	break;
			}
			if(WINER == White)
				MessageBox(cxvvc,"White Win","",MB_OK);
			if(WINER == Black)
				MessageBox(cxvvc,"Black Win","",MB_OK);
    }
	void Star(){
		int (*p)[15]=TRUE_BOARD.board;
	    Draw.init(&p);
		Begin();
	}
}GAME;

int main(){
	GAME.Star();
    return 0;
}
