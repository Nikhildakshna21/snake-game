#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <array>
#include <time.h>
#include <mmsystem.h>
//#include <fstream>

#define WIDTH 100
#define HEIGHT 30
using namespace std;

char map[HEIGHT][WIDTH+1];
unsigned int f_pos[2];

enum Dir{
    UP,
    LEFT,
    RIGHT,
    DOWN,
};

class snake{
    public:
        static std::vector<array<int,3>>s;
        int i;
    snake(int x,int y,Dir dir){
            array<int,3> a = {x,y,dir};
            i = s.size();
            s.push_back(a);
    }
    std::vector<int> get_pos(){
        return {s[i][0],s[i][1]};
    }
    void set_pos(int x,int y){
        s[i][0] = x;
        s[i][1] = y;
    }
    void move_pos(int dx,int dy){
        s[i][0] += dx;
        s[i][1] += dy;
    }
    int get_dir(){
        return s[i][2];
    }
    void set_dir(Dir dir){
        s[i][2] = dir;
    }
};

void print();
void outline();
void pix(char,vector<int>);
void mov();
void draw();
void clear();
vector<array<int,3>>snake::s;
int counter=0;
int fps=100;
int score=0;
int main(){
    snake sn(10,10,UP);
    srand(rand()+50*sn.get_pos()[0]-130*sn.get_pos()[1]+time(0));
    f_pos[0]=rand()%WIDTH;
    f_pos[1]=rand()%HEIGHT;
    
    system("color 0C");
    while (true){
        counter=(counter+1)%fps;   
        if(sn.get_pos()[0]==f_pos[0]&&sn.get_pos()[1]==f_pos[1]){
            srand(rand()+50*sn.get_pos()[0]-130*sn.get_pos()[1]+time(0));
            //cout << '\a';
            PlaySoundA((LPCSTR)".\\collect.wav",NULL,SND_ASYNC);
            f_pos[0]=(rand()%(WIDTH-2))+1;
            f_pos[1]=(rand()%(HEIGHT-2))+1;
            array<int,3> temp = {};
            switch(snake::s[snake::s.size()-1][2]){
                case UP:
                temp = {snake::s[snake::s.size()-1][0],snake::s[snake::s.size()-1][1]-1,UP};
                break;
                case DOWN:
                temp = {snake::s[snake::s.size()-1][0],snake::s[snake::s.size()-1][1]+1,DOWN};
                break;
                case LEFT:
                temp = {snake::s[snake::s.size()-1][0]+1,snake::s[snake::s.size()-1][1],LEFT};
                break;
                case RIGHT:
                temp = {snake::s[snake::s.size()-1][0]-1,snake::s[snake::s.size()-1][1],RIGHT};
            }
            snake::s.push_back(temp);
            score++;
        }
        if((sn.get_pos()[0]==0||sn.get_pos()[0]==WIDTH-1)||(sn.get_pos()[1]==0||sn.get_pos()[1]==HEIGHT-1))break;
        if(GetKeyState(VK_SHIFT)&0x8000)break;
        if(GetKeyState('A')&0x8000&&sn.get_dir()!=RIGHT)sn.set_dir(LEFT);
        else if(GetKeyState('S')&0x8000&&sn.get_dir()!=UP)sn.set_dir(DOWN);
        else if(GetKeyState('W')&0x8000&&sn.get_dir()!=DOWN)sn.set_dir(UP);
        else if(GetKeyState('D')&0x8000&&sn.get_dir()!=LEFT)sn.set_dir(RIGHT);
        if(counter%5 == 0){
            outline();
            mov();
            draw();
            pix(177,{(int)f_pos[0],(int)f_pos[1]});
            print();   
            cout << "score: " << score <<"\n";
            clear(); 
        }
        for(int i=1;i<snake::s.size();i++){
            if(snake::s[i][0]==sn.get_pos()[0]&&snake::s[i][1]==sn.get_pos()[1])return 0;
        }
        Sleep(1000/fps);
    }
    
}

void print(){
    for(uint16_t x = 0;x < HEIGHT;x++){
        cout << map[x] << "\n";
    }
}

void outline(){
    
    for(uint16_t y = 0;y < HEIGHT;y++){
        for(uint16_t x = 0;x < WIDTH;x++){
            if(y == 0)map[y][x]=220;
            else if(y == HEIGHT-1)map[y][x]=223;
            else{
                map[y][x] = (x == 0 || x == WIDTH-1)?219:' ';
            }
        }
        map[y][WIDTH] = '\0';        
    }
}

void pix(char a,vector<int> pos){
    
    map[HEIGHT-pos[1]-1][pos[0]] = a;
}

void mov(){
    for(int k=snake::s.size()-1;k>=0;k--){
        switch(snake::s[k][2]){
            case UP:
                snake::s[k][1]++;
                break;
            case DOWN:
                snake::s[k][1]--;
                break;
            case LEFT:
                snake::s[k][0]--;
                break;
            case RIGHT:
                snake::s[k][0]++;
                break;
        }
        if(k!=0)snake::s[k][2]=snake::s[k-1][2];
    }
}

void draw(){
    for(array<int,3>a:snake::s){
        pix(219,{a[0],a[1]});//254
    }
}

void clear(){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(out, &ci);
    ci.bVisible = false;
    SetConsoleCursorInfo(out, &ci);
    COORD c;
    c.X = 0;
    c.Y = 0;
    SetConsoleCursorPosition(out,c);
}