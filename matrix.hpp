#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<vector>
#include<cmath>

using namespace std;


class matrix{
public:
    int h, w;
    vector<vector<double> > t;
    vector<vector<double> > sub_matrix;
    
    vector<double> get_vector_coln(int col){
        vector<double> v;
        v.resize(h);
        for (int i = 0; i < h; i++) {
            v[i] = t[i][col];
        }
        return v;
    }
    
    double dot_vec(vector<double> &v1, vector<double> &v2){
        double sum = 0;
        int dotSize = min(v1.size(), v2.size());
        
        for (int i = 0; i < dotSize; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }
    //基本演算
    matrix(){}
    
    matrix(vector<vector<double> > array){
        h = array.size();
        w = array[0].size();
        t = array;
    }
    
    void shape(){
        cout << "[" << h << ", " << w << "]" << endl;
    }
    
    void print(){
        cout << '[';
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if(j != 0) cout << ' ';
                cout << t[i][j];
            }
            if(i == h-1) cout << ']';
            cout << endl ;
        }
        cout << endl;
    }
    
    void transpose(){
        vector<vector<double> > v;
        v.resize(w);
        for (int i = 0; i < w; i++) {
            v[i].resize(h);
            for (int j = 0; j < h; h++) {
                v[i][j] = t[j][i];
            }
        }
        t = v;
    }
    
    void dot(matrix &t2){
        if(w != t2.h){
            cout << "dot Shape error." << endl;
            return;
        }else{
            vector<vector<double> > t3;
            vector<vector<double> > dotT;
            w = t2.w;
            dotT.resize(w);
            for (int j = 0; j < w; j++) {
                vector<double> v = t2.get_vector_coln(j);
                dotT[j] = v;
            }
            
            t3.resize(h);
            for (int i = 0; i < h; i++) {
                t3[i].resize(t2.w);
                for (int j = 0; j < w; j++) {
                    vector<double> v = t2.get_vector_coln(j);
                    t3[i][j] = dot_vec(t[i], dotT[j]);
                    print();
                }
            }
            t = t3;
        }
    }
};

#endif // MATRIX_HPP
