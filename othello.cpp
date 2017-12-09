#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

#define yet 0
#define wht 1
#define blk 2

typedef long long ll;

int ban[9][9];
int direction[8][2] = {
    {0,1}, {0,-1}, {1,0}, {-1,0},
    {1,1}, {1,-1}, {-1,-1}, {-1,1}
};

class ban_hist{
public:
    //playerがbanの時にxyにさしてwin(bool)
    int player;
    int myban[8][8];
    int x,y;
    bool win;
    
    ban_hist(int _p, int _x, int _y){
        player = _p;
        x = _x;
        y = _y;
        win = false;
    }
    
    void result(bool _w){
        win = _w;
    }
    
    void copy(){
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                myban[i][j] = ban[i+1][j+1];
            }
        }
    }
    
    void disp_myban(){
        //whtがxyに打って勝った
        cout << endl;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (myban[i][j] == yet) {
                    cout << myban[i][j];
                }else{
                    int val;
                    if(player == wht){
                        val = (myban[i][j] == wht)? wht: blk;
                    }else{
                        val = (myban[i][j] == wht)? blk: wht;
                    }
                    cout << val;
                }
            }
            cout << endl;
        }
        cout << x << y << endl;
    }
    
    void save_file(string file_name){
        ofstream outputfile(file_name, ios::app);
        //whtがxyに打って勝った
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (myban[i][j] == yet) {
                    outputfile << myban[i][j];
                }else{
                    int val;
                    if(player == wht){
                        val = (myban[i][j] == wht)? wht: blk;
                    }else{
                        val = (myban[i][j] == wht)? blk: wht;
                    }
                    outputfile << val;
                }
            }
            outputfile << endl;
        }
        outputfile << x << y << endl;
    }
};

bool update(int x, int y, int col, int dir, int depth){
    x += direction[dir][0];
    y += direction[dir][1];
    
    if ((x < 1 || 8 < x) || (y < 1 || 8 < y) ) return false;
    if (ban[y][x] == yet) return false;
    
    if (ban[y][x] == col) {
        if(depth == 0) return false;
        else return true;
    }else{
        if(update(x, y, col, dir, depth+1)){
            ban[y][x] = col;
            return true;
        }else{
            return false;
        }
    }
}

bool check(int x, int y, int col, int dir, int depth){
    x += direction[dir][0];
    y += direction[dir][1];
    
    if ((x < 1 || 8 < x) || (y < 1 || 8 < y) ) return false;
    if (ban[y][x] == yet) return false;
    
    if (ban[y][x] == col) {
        if(depth == 0) return false;
        else return true;
    }else{
        if(check(x, y, col, dir, depth+1)){
            return true;
        }else{
            return false;
        }
    }
}

bool check_xy(int x, int y, int col){
    if (ban[y][x] != 0) return false;
    bool pos = false;
    for (int i = 0; i < 8; i++) {
        pos |= check(x, y, col, i, 0);
    }
    return pos;
}

void update_xy(int x, int y, int col){
    if (ban[y][x] != 0) return;
    bool pos = false;
    for (int i = 0; i < 8; i++) {
        pos |= update(x, y, col, i, 0);
    }
    if (pos) ban[y][x] = col;
}

void winner_call(){
    int count[2] = {};
    string winner[3] = {" The white won!", " The black won!", " Draw!"};
    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 9; j++) {
            if(ban[j][i] != yet) count[ban[j][i]-1]++;
        }
    }
    
    int win = 0;
    if (count[blk-1] == count[wht-1]) {
        win = 2;
    }else if(count[blk-1] > count[wht-1]){
        win = 1;
    }else{
        win = 0;
    }
    
    if(count[blk-1]/10 < 1) cout << 0;
    cout << count[blk-1] << "-";
    if(count[wht-1]/10 < 1) cout << 0;
    cout << count[wht-1];
    cout << winner[win] << endl;
}

void init_ban(){
    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 9; j++) {
            ban[j][i] = yet;
        }
    }
    ban[4][4] = wht;
    ban[5][5] = wht;
    ban[4][5] = blk;
    ban[5][4] = blk;
}

void get_data(){
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++) {
        string col;
        int x,y,col_int;
        cin >> col >> x >> y;
        col_int = (col[0] == 'W')? wht: blk;
        
        update_xy(x, y, col_int);
    }
}

vector<pair<int, int> > get_putList(int col){
    vector<pair<int, int> > V;
    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 9; j++) {
            if(check_xy(j, i, col)) V.push_back(make_pair(j, i));
        }
    }
    
    return V;
}

void printPosList(vector<pair<int, int> > V){
    for (int i = 0; i < V.size(); i++) {
        printf("[ %d, %d]\n",V[i].first, V[i].second);
    }
}

bool end_game(){
    return (get_putList(wht).size() == 0 && get_putList(blk).size() == 0);
}

void disp_ban(){
    cout << endl << ' ';
    
    for (int i = 1; i < 9; i++) {
        cout << ' ' << i;
    }
    
    cout << endl;
    
    for (int i = 1; i < 9; i++) {
        cout << i;
        for (int j = 1; j < 9; j++) {
            string s = (ban[i][j] == wht)? "◯":"◉";
            cout << ' ' << ((ban[i][j] != blk && ban[i][j] != wht)? "_":s);
        }
        cout << endl;
    }
}

int digitsNumber(int num){
    int dNum = 0;
    while (num > 0) {
        num /= 10;
        dNum++;
    }
    return dNum;
}

string makeName(int game_num){
    string s;
    for (int i = 0; i < 5 - digitsNumber(game_num); i++) {
        s = s + "0";
    }
    return "othelloFile/game" + s + to_string(game_num) + ".txt";
}

int rand_game(int game_num){
    init_ban();
    //disp_ban();
    
    int player = wht;
    vector<ban_hist> game_hist;
    
    while (!end_game()) {
        player = (player == wht)? blk: wht;
        string plstring = (player == wht)? "white": "black";
        int select;
        vector<pair<int, int> > V = get_putList(player);
        if (V.size() != 0) {
            select = rand() % V.size();
            //cout << plstring << " put " << V[select].first << V[select].second << endl;
            ban_hist bh(player, V[select].first, V[select].second);
            bh.copy();
            game_hist.push_back(bh);
            update_xy(V[select].first, V[select].second, player);
            //disp_ban();
        }else{
            //cout << plstring << " pass" << endl;
        }
    }
    int count[3] = {};
    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 9; j++) {
            count[ban[i][j]]++;
        }
    }
    
    int winner = (count[wht] == count[blk])? yet: ((count[wht] > count[blk])? wht: blk);
    
    string file_name = makeName(game_num);
    
    for (int i = 0; i < game_hist.size(); i++) {
        if(game_hist[i].player == winner) {
            game_hist[i].result(true);
            game_hist[i].save_file(file_name);
        }
        
    }
    //cout << file_name << " end." << endl;
    return winner;
}

int main(){
    srand((unsigned int)time(0));
    for (int i = 0; i < 5; i++) int temp = rand();
    
    int game_num = 1;
    
    clock_t start = clock();
    for (int i = 0; i < 30000; i++) {
        int draw_flag = yet;
        while (draw_flag == yet) {
            draw_flag = rand_game(game_num);
        }
        game_num++;
    }
    clock_t end = clock();
    cout << "Execution time " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
    
    return 0;
}