#include <iostream>
#include <vector>
#include "matrix.hpp"
#include "network.hpp"
#include "matplotlib.hpp"

using namespace std;

int main(){
    /*
     00000000
     00000000
     00000000
     00021000
     00012000
     00000000
     00000000
     00000000
     43
     */
    
    ifstream ifs("othelloFile/game00001.txt");
    string str;
    vector<vector<double> > teacher;
    vector<vector<double> > in;
    
    string ban = "";
    vector<string> ban_list;
    vector<int> ind_list;
    
    int counter = 1;
    while (getline(ifs, str)){
        if (counter%9) {
            ban = ban + str;
        }else{
            int index = (str[1] - '0' - 1)*8 + (str[0] - '0' - 1);
            ind_list.push_back(index);
            ban_list.push_back(ban);
            ban = "";
        }
        counter++;
    }
    
    teacher.resize(ind_list.size());
    in.resize(ban_list.size());
    
    for (int i = 0; i < ind_list.size(); i++) {
        teacher[i].resize(64);
        in[i].resize(64);
        for (int j = 0; j < 64; j++) {
            in[i][j] = ban_list[i][j] - '0';
            teacher[i][j] = (ind_list[i] == j)? 1:0;
        }
    }
    
    matrix mat_in(in);
    matrix mat_te(teacher);
    
    network n(64, 4, 20, 64, mat_in.h);
    
    matplotlib g;
    g.open();
    
    g.screen(0, 4, 10000, 20);
    double prim = 0.0;
    
    for (int i = 0; i < 2000; i++) {
        matrix inp = mat_in;
        matrix tep = mat_te;
        
        n.for_and_backward(inp, tep);
        n.leaning(0.015);
        
        matrix ine = mat_in;
        matrix tee = mat_te;
        
        double err = n.calculate_error(ine, tee);
        g.line(i-1,prim,i,err);
        prim = err;
    }
    
    n.save_network();
    
    cout << "done" << endl;
    return 0;
}
