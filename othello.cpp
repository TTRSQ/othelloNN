#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <time.h>
#include "network.hpp"
#include "matplotlib.hpp"

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

class ban_hist{
public:
    //playerがbanの時にxyにさしてwin(bool)
    vector<double> myban;
    vector<double> myans;
    
    ban_hist(){
        myban.resize(64);
        myans.resize(64);
    }
    
    void bancpy(int col){
        for (int i = 1; i < 9; i++) {
            for (int j = 1; j < 9; j++) {
                int index = (i-1)*8 + (j-1);
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
    
    void anscpy(pair<int, int> p){
        int index = (p.second-1)*8 + (p.first-1);
        fill(myans.begin(), myans.end(), 0.0);
        myans[index] = 1.0;
    }
    
    void print(){
        for (int i = 0; i < 64; i++) {
            if (i != 0) cout << ' ';
            cout << myban[i];
        }
        cout << endl;
        for (int i = 0; i < 64; i++) {
            if (i != 0) cout << ' ';
            cout << myans[i];
        }
        cout << endl;
    }
    
    void printmyban(){
        for (int i = 0; i < 64; i++) {
            if (i%8 == 0) {
                cout << endl;
            }else{
                cout << ' ';
            }
            cout << myban[i];
        }
        cout << endl;
        for (int i = 0; i < 64; i++) {
            if (i%8 == 0) {
                cout << endl;
            }else{
                cout << ' ';
            }
            cout << myans[i];
        }
        cout << endl;
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
        vector<double> v;
        v.resize(64);
        for (int i = 1; i < 9; i++) {
            for (int j = 1; j < 9; j++) {
                if (ban[i][j] != yet) {
                    v[(i-1)*8+j-1] = (ban[i][j] == pcol)? 1.0: 2.0;
                }else{
                    v[(i-1)*8+j-1] = 0.0;
                }
            }
        }
        
        v = net.prediction(v);
        
        vector<pair<int, int> > plist = get_putList(pcol);
        vector<pair<pair<int ,int>, double > > pos;
        
        double sum = 0.0;
        
        for (int i = 0; i < plist.size(); i++) {
            int x = plist[i].first - 1;
            int y = plist[i].second - 1;
            pos.push_back(make_pair(plist[i], v[x + 8*y]));
            
            sum += exp(v[x + 8*y]);
        }
        
        for (int i = 0; i < pos.size(); i++) {
            pos[i].second = exp(pos[i].second)/sum;
        }
        
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<int> rand5(0,10000000);
        double rand01 = (rand5(mt))/10000000.0;
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
};

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
            cout << "put xy" << endl;
            bool flag = 1;
            while (flag) {
                int k;
                cin >> k;
                x = k/10;
                y = k%10;
                if (check_xy(x, y, pcolor)) {
                    flag = 0;
                }else{
                    cout << "you can't put there." << endl;
                }
            }
            
        }else{
            int select = rand()%v.size();
            x = v[select].first;
            y = v[select].second;
        }
        
        update_xy(x, y, player);
        disp_ban();
        cout << ((player == pcolor)? "you ": "cp ") << "put " << x << y << endl;
        player = (player == wht)? blk: wht;
    }
    
    winner_call();
}

int keta(int num,int k){
    if (num) {
        return keta(num/10, k+1);
    }else{
        return k;
    }
}

void lean_net(){
    string str;
    vector<vector<double> > data;
    vector<vector<double> > teacher;
    
    string ban_line = "";
    vector<string> ban_list;
    vector<int> ind_list;
    
    for (int i = 1; i <= 499; i++) {
        string s = "othellodata/game";
        
        for (int k = 0; k < 5 - keta(i, 0); k++) {
            s = s + '0';
        }
        
        s = s + std::to_string(i) + ".txt";
        
        int counter = 1;
        
        ifstream ifs(s);
        while (getline(ifs, str)){
            if (counter%9) {
                ban_line = ban_line + str;
            }else{
                int index = (str[1] - '0' - 1)*8 + (str[0] - '0' - 1);
                ind_list.push_back(index);
                ban_list.push_back(ban_line);
                ban_line = "";
            }
            counter++;
        }
        ifs.close();
    }
    
    data.resize(ban_list.size());
    teacher.resize(ind_list.size());
    for (int i = 0; i < ind_list.size(); i++) {
        teacher[i].resize(64);
        data[i].resize(64);
        for (int j = 0; j < 64; j++) {
            data[i][j] = ban_list[i][j] - '0';
            teacher[i][j] = (ind_list[i] == j)? 1:0;
        }
    }
    matrix mat_in(data);
    matrix mat_te(teacher);
    network n(64, 2, 100, 64, mat_in.h);
    matplotlib g;
    g.open();
    g.screen(0, 0, 200, 7);
    
    double prim = 0.0;
    for (int i = 0; i < 200; i++) {
        matrix inp = mat_in;
        matrix tep = mat_te;
        
        n.for_and_backward(inp, tep);
        n.leaning_adam(0.01);
        
        double err = n.calculate_error(mat_in, mat_te);
        cout << i+1 << " err = " << err << endl;
        g.line(i-1,prim,i,err);
        prim = err;
    }
    
    n.save_network("NNprams500");
}

void vs_NN(int pcolor){
    init_ban();
    disp_ban();
    nn_reader net("NNprams500");
    
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
            cout << "put xy" << endl;
            bool flag = 1;
            while (flag) {
                int k;
                cin >> k;
                x = k/10;
                y = k%10;
                if (check_xy(x, y, pcolor)) {
                    flag = 0;
                }else{
                    cout << "you can't put there." << endl;
                }
            }
            
        }else{
            std::pair<int, int> p = net.nnAnsor(player);
            x = p.first;
            y = p.second;
        }
        
        update_xy(x, y, player);
        disp_ban();
        cout << ((player == pcolor)? "you ": "cp ") << "put " << x << y << endl;
        player = (player == wht)? blk: wht;
    }
    
    winner_call();
}

bool rand_vs_nn(int randcolor){
    init_ban();
    nn_reader net("NNprams500");
    //disp_ban();
    
    int player = blk;
    
    while (!end_game()) {
        
        vector<pair<int, int> > v = get_putList(player);
        if (v.size() == 0) {
            //cout << ((player == wht)? "white ": "black ") << "pass." << endl;
            player = (player == wht)? blk: wht;
            continue;
        }
        
        int x,y;
        
        if (player == randcolor) {
            int select = rand()%v.size();
            x = v[select].first;
            y = v[select].second;
        }else{
            std::pair<int, int> p = net.nnAnsor(player);
            x = p.first;
            y = p.second;
        }
        
        update_xy(x, y, player);
        disp_ban();
        //cout << ((player == randcolor)? "rand ": "NN ") << "put " << x << y << endl;
        player = (player == wht)? blk: wht;
    }
    
    int counter[3] = {};
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            counter[ban[i][j]]++;
        }
    }
    
    return counter[randcolor] < counter[((randcolor == wht)? blk: wht)];
}

void nn_vs_nn(int sequence_num){
    //ゲームをAIにさせた結果を保存しつつ100イテレーション
    
    string nn_name = "sequence0";
    string nn_prename = nn_name;
    
    //無学習ネットワークの作成
    network n(64, 2, 100, 64, 3000);
    n.save_network(nn_name);
    
    nn_reader nr;
    
    for (int sequence = 0; sequence < sequence_num; sequence++) {
        nn_prename = nn_name;
        nn_name = "sequence" + to_string(sequence + 1);
        
        //preで読み込んでnameで保存
        nr.reload_network(nn_prename);
        
        vector<ban_hist> blackhist;
        vector<ban_hist> whitehist;
        
        while (blackhist.size() < 1400 && whitehist.size() < 1400) {
            
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
                
                pair<int, int> p = nr.nnAnsor(player);
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
            
            int counter[3] = {};
            
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    counter[ban[i][j]]++;
                }
            }
            
            if (counter[blk] != counter[wht]) {
                if (counter[blk] > counter[wht]) {
                    blackhist.insert(blackhist.end(), temp_blackhist.begin(), temp_blackhist.end());
                }else{
                    whitehist.insert(whitehist.end(), temp_whitehist.begin(), temp_whitehist.end());
                }
            }
        }
        
        vector<vector<double> > matban;
        vector<vector<double> > matans;
        
        for (int i = 0; i < blackhist.size(); i++) {
            matban.push_back(blackhist[i].myban);
            matans.push_back(blackhist[i].myans);
        }
        
        for (int i = 0; i < whitehist.size(); i++) {
            matban.push_back(whitehist[i].myban);
            matans.push_back(whitehist[i].myans);
        }
        
        matrix matrixban(matban);
        matrix matrixans(matans);
        
        matplotlib g;
        g.open();
        g.screen(0, 0, 50, 6);
        
        double prime = 0.0;
        for (int i = 0; i < 50; i++) {
            nr.net.for_and_backward(matrixban, matrixans);
            nr.net.leaning_adam(0.01);
            double err = nr.net.calculate_error(matrixban, matrixans);
            cout << "sequence:" << sequence << " " << i+1 << " err = " << err << endl;
            g.line(i-1,prime,i,err);
            prime = err;
        }
        g.close();
        nr.net.save_network(nn_name);
    }
}

int main(){
    srand((unsigned int)time(0));
    for (int i = 0; i < 5; i++) int temp = rand();
    clock_t start = clock();
    
    nn_vs_nn(5);
    
    clock_t end = clock();
    cout << "Execution time " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
    
    return 0;
}
