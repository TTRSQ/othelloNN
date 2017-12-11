#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<vector>
#include<cmath>

using namespace std;


class matrix{
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
    
    
public:
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
    
    //
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
    
    void relu(){
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                t[i][j] = max(t[i][j], 0);
            }
        }
    }
    
    
    
    void adam_lite(double leaning_rate, vector<vector<double> > &ada_grad, vector<vector<double> > &velocity_matrix, vector<vector<double> > &prime){
        for (int i = 0; i < prime.size(); i++) {
            for (int j = 0; j < prime[0].size(); j++) {
                ada_grad[i][j] += prime[i][j]*prime[i][j];
                velocity_matrix[i][j] = 0.9*velocity_matrix[i][j] - (leaning_rate/sqrt(ada_grad + 0.0000001))*prime[i][j];
                t[i][j] += velocity_matrix[i][j];
            }
        }
    }
};

#endif // MATRIX_HPP
