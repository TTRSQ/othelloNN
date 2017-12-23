#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <random>
#include <time.h>
#include "network.hpp"
#include "matplotlib.hpp"

using namespace std;

#define yet 0
#define wht 1
#define blk 2

int direction[8][2] = {
    {0,1}, {0,-1}, {1,0}, {-1,0},
    {1,1}, {1,-1}, {-1,-1}, {-1,1}
};

std::map<int, string> color_string;

int ban[8][8] = {};

void init_ban(){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ban[i][j] = 0;
        }
    }
    ban[3][3] = wht;
    ban[4][4] = wht;
    ban[3][4] = blk;
    ban[4][3] = blk;
}

bool update(int x, int y, int col, int dir, int depth){
    x += direction[dir][0];
    y += direction[dir][1];
    
    if ((x < 0 || 7 < x) || (y < 0 || 7 < y) ) return false;
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
    
    if ((x < 0 || 7 < x) || (y < 0 || 7 < y) ) return false;
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

std::map<int, int> count(){
    std::map<int, int> m;
    m[wht] = 0;
    m[blk] = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m[ban[i][j]]++;
        }
    }
    return m;
}

std::vector<pair<int, int> > get_putList(int col){
    vector<pair<int, int> > V;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(check_xy(j, i, col)) V.push_back(make_pair(j, i));
        }
    }
    return V;
}

bool end_game(){
    return (get_putList(wht).size() == 0 && get_putList(blk).size() == 0);
}

void disp_ban(){
    std::cout << endl << ' ';
    
    for (int i = 0; i < 8; i++) {
        std::cout << ' ' << i;
    }
    
    std::cout << endl;
    
    for (int i = 0; i < 8; i++) {
        std::cout << i;
        for (int j = 0; j < 8; j++) {
            string s = (ban[i][j] == wht)? "◯":"◉";
            std::cout << ' ' << ((ban[i][j] != blk && ban[i][j] != wht)? "_":s);
        }
        std::cout << endl;
    }
}

class ban_hist{
public:
    vector<double> myban;
    vector<double> myans;
    
    ban_hist(){
        myban.resize(64);
        myans.resize(64);
    }
    
    void bancpy(int col){
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int index = i*8 + j;
                if (check_xy(j, i, col)) {
                    myban[index] = 3.0;
                }else{
                    if (ban[i][j] == yet) {
                        myban[index] = 0.0;
                    }else{
                        myban[index] = double((col == wht)? ban[i][j]: ((ban[i][j] == wht)? blk: wht));
                    }
                }
            }
        }
    }
    
    void bancpy_simple(int col){
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int index = i*8 + j;
                if (ban[i][j] == yet) {
                    myban[index] = 0.0;
                }else{
                    myban[index] = double((col == wht)? ban[i][j]: ((ban[i][j] == wht)? blk: wht));
                }
            }
        }
    }
    
    void anscpy(pair<int, int> p){
        int index = p.second*8 + p.first;
        fill(myans.begin(), myans.end(), 0.0);
        myans[index] = 1.0;
    }
    
    void print(){
        for (int i = 0; i < myban.size(); i++) {
            if (i != 0) std::cout << ' ';
            std::cout << myban[i];
        }
        std::cout << std::endl;
        for (int i = 0; i < myans.size(); i++) {
            if (i != 0) cout << ' ';
            std::cout << myans[i];
        }
        std::cout << endl;
    }
    
    void printmyset(){
        for (int i = 0; i < myban.size(); i++) {
            if (i%8 == 0) {
                std::cout << endl;
            }else{
                std::cout << ' ';
            }
            std::cout << myban[i];
        }
        std::cout << endl;
        for (int i = 0; i < myans.size(); i++) {
            if (i%8 == 0) {
                std::cout << endl;
            }else{
                std::cout << ' ';
            }
            std::cout << myans[i];
        }
        std::cout << std::endl;
    }
};

class nn_reader{
public:
    network net;
    std::random_device rd;
    
    nn_reader(){}
    
    nn_reader(string name){
        net.load_network(name);
    }
    
    void reload_network(string name){
        net.load_network(name);
    }
    
    pair<int, int> nnAnsor(int pcol){
        ban_hist b;
        b.bancpy(pcol);
        
        vector<double> v = net.prediction(b.myban);
        
        vector<pair<int, int> > plist = get_putList(pcol);
        vector<pair<pair<int ,int>, double > > pos;
        
        double sum = 0.0;
        
        for (int i = 0; i < plist.size(); i++) {
            int x = plist[i].first;
            int y = plist[i].second;
            pos.push_back(make_pair(plist[i], v[x + 8*y]));
            
            sum += exp(v[x + 8*y]);
        }
        
        for (int i = 0; i < pos.size(); i++) {
            pos[i].second = exp(pos[i].second)/sum;
        }
        
        double rand01 = (rand()%10000000)/10000000.0;
        sum = 0;
        int index = 0;
        for (int i = 0; i < pos.size()+5; i++) {
            sum += pos[i%pos.size()].second;
            if (sum > rand01) {
                index = i%pos.size();
                break;
            }
        }
        return pos[index].first;
    }
    
    pair<int, int> nnAnsorMax(int pcol){
        ban_hist b;
        b.bancpy(pcol);
        
        std::vector<double> v = net.prediction(b.myban);
        std::vector<std::pair<int, int> > plist = get_putList(pcol);
        
        std::priority_queue<pair<double, pair<int, int> > > pq;
        
        for (int i = 0; i < plist.size(); i++) {
            pq.push(std::make_pair(v[plist[i].first + plist[i].second*8], plist[i]));
        }
        
        return pq.top().second;
    }
};

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
    return "othellodata/game" + s + to_string(game_num) + ".txt";
}

void vs_random(int pcolor){
    init_ban();
    disp_ban();
    
    int player = blk;
    
    while (!end_game()) {
        
        vector<pair<int, int> > v = get_putList(player);
        if (v.size() == 0) {
            cout << ((player == wht)? "white ": "black ") << "pass." << endl;
            player = (player == wht)? blk: wht;
            continue;
        }
        
        int x,y;
        
        if (player == pcolor) {
            std::cout << "put xy" << std::endl;
            bool flag = 1;
            while (flag) {
                int k;
                std::cin >> k;
                x = k/10;
                y = k%10;
                if (check_xy(x, y, pcolor)) {
                    flag = 0;
                }else{
                    std::cout << "you can't put there." << std::endl;
                }
            }
            
        }else{
            int select = rand()%v.size();
            x = v[select].first;
            y = v[select].second;
        }
        
        update_xy(x, y, player);
        disp_ban();
        std::cout << ((player == pcolor)? "you ": "cp ") << "put " << x << y << std::endl;
        player = (player == wht)? blk: wht;
    }
}

int keta(int num,int k){
    if (num) {
        return keta(num/10, k+1);
    }else{
        return k;
    }
}

void vs_NN(int pcolor){
    init_ban();
    disp_ban();
    nn_reader net("Nsequence11");
    
    int player = blk;
    
    while (!end_game()) {
        
        vector<pair<int, int> > v = get_putList(player);
        if (v.size() == 0) {
            std::cout << ((player == wht)? "white ": "black ") << "pass." << std::endl;
            player = (player == wht)? blk: wht;
            continue;
        }
        
        int x,y;
        
        if (player == pcolor) {
            std::cout << "put xy" << std::endl;
            bool flag = 1;
            while (flag) {
                int k;
                std::cin >> k;
                x = k/10;
                y = k%10;
                if (check_xy(x, y, pcolor)) {
                    flag = 0;
                }else{
                    std::cout << "you can't put there." << std::endl;
                }
            }
            
        }else{
            std::pair<int, int> p = net.nnAnsorMax(player);
            x = p.first;
            y = p.second;
        }
        
        update_xy(x, y, player);
        disp_ban();
        std::cout << ((player == pcolor)? "you ": "cp ") << "put " << x << y << std::endl;
        player = (player == wht)? blk: wht;
    }
}

bool rand_vs_nn(int randcolor, string nn_name){
    init_ban();
    nn_reader net(nn_name);
    
    int player = blk;
    
    while (!end_game()) {
        
        std::vector<pair<int, int> > v = get_putList(player);
        if (v.size() == 0) {
            player = (player == wht)? blk: wht;
            continue;
        }
        
        int x,y;
        
        if (player == randcolor) {
            int select = rand()%v.size();
            x = v[select].first;
            y = v[select].second;
        }else{
            std::pair<int, int> p = net.nnAnsorMax(player);
            x = p.first;
            y = p.second;
        }
        
        update_xy(x, y, player);
        player = (player == wht)? blk: wht;
    }
    
    std::map<int, int> counter = count();
    
    return counter[randcolor] < counter[((randcolor == wht)? blk: wht)];
}

void nn_vs_nn(int start_num, int end_num){
    //ゲームをAIにさせた結果を保存しつつ100イテレーション
    
    std::string nn_name = "NNP" + std::to_string(start_num-1);
    std::string nn_prename = nn_name;
    
    nn_reader nr;
    
    for (int sequence = start_num; sequence <= end_num; sequence++) {
        clock_t start = clock();
        
        nn_prename = nn_name;
        nn_name = "NNP" + std::to_string(sequence);
        //preで読み込んでnameで保存
        nr.reload_network(nn_prename);
        vector<ban_hist> blackhist;
        vector<ban_hist> whitehist;
        while (blackhist.size() < 300 || whitehist.size() < 300) {
            vector<ban_hist> temp_blackhist;
            vector<ban_hist> temp_whitehist;
            
            init_ban();
            int player = blk;
            
            while (!end_game()) {
                vector<pair<int, int> > v = get_putList(player);
                if (v.size() == 0) {
                    player = (player == wht)? blk: wht;
                    continue;
                }
                
                ban_hist hist;
                pair<int, int> p;
                if (sequence != 0) {
                    p = nr.nnAnsor(player);
                }else{
                    p = v[rand()%v.size()];
                }
                hist.bancpy(player);
                hist.anscpy(p);
                update_xy(p.first, p.second, player);
                
                if (player == blk) {
                    temp_blackhist.push_back(hist);
                }else{
                    temp_whitehist.push_back(hist);
                }
                
                player = (player == wht)? blk: wht;
            }
            std::map<int, int> counter = count();
            
            //勝った方をデータセットに追加
            if (counter[blk] != counter[wht]) {
                if (counter[blk] > counter[wht]) {
                    blackhist.insert(blackhist.end(), temp_blackhist.begin(), temp_blackhist.end());
                }else{
                    whitehist.insert(whitehist.end(), temp_whitehist.begin(), temp_whitehist.end());
                }
            }
        }
        cout << "blackData = " << blackhist.size() << ", whiteData = " << whitehist.size() << endl;
        
        vector<vector<double> > matban;
        vector<vector<double> > matans;
        
        for (int i = 0; i < 300; i++) {
            matban.push_back(blackhist[i].myban);
            matans.push_back(blackhist[i].myans);
            matban.push_back(whitehist[i].myban);
            matans.push_back(whitehist[i].myans);
        }
        
        matrix matrixban(matban);
        matrix matrixans(matans);
        
        matplotlib g;
        g.open();
        g.screen(0, 0, 150, 3);
        
        double prime = 0.0;
        for (int i = 0; i < 150; i++) {
            nr.net.for_and_backward(matrixban, matrixans);
            nr.net.leaning_adam(0.01);
            double err = nr.net.calculate_error(matrixban, matrixans);
            std::cout << "sequence:" << sequence << " " << i+1 << " err = " << err << std::endl;
            g.line(i-1,prime,i,err);
            prime = err;
        }
        
        g.close();
        nr.net.save_network(nn_name);
        
        clock_t end = clock();
        std::cout << "sequence " << sequence+1 << " end in " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << std::endl;
    }
}

void init(){
    srand((unsigned int)time(0));
    
    color_string[wht] = "white";
    color_string[blk] = "black";
}

int main(){
    init();
    nn_vs_nn(1, 5);
    
    return 0;
}
