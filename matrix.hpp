#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<vector>
#include<cmath>

class matrix{
public:
    int h, w;
    std::vector<std::vector<double> > t;
    std::vector<std::vector<double> > sub_matrix;
    
    std::vector<double> get_vector_coln(int col){
        std::vector<double> v;
        v.resize(h);
        for (int i = 0; i < h; i++) {
            v[i] = t[i][col];
        }
        return v;
    }
    
    double dot_vec(std::vector<double> &v1, std::vector<double> &v2){
        double sum = 0;
        int dotSize = std::min(v1.size(), v2.size());
        
        for (int i = 0; i < dotSize; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }
    //基本演算
    matrix(){}
    
    matrix(std::vector<std::vector<double> > array){
        h = array.size();
        w = array[0].size();
        t = array;
    }
    
    void sizeinit(int _h, int _w){
        h = _h;
        w = _w;
        t.resize(h);
        for (int i = 0; i < h; i++) {
            t[i].resize(w);
            std::fill(t[i].begin(), t[i].end(), 0.0);
        }
    }
    
    void sizeinit_from_matrix(matrix &in){
        h = in.h;
        w = in.w;
        t.resize(h);
        for (int i = 0; i < h; i++) {
            t[i].resize(w);
            std::fill(t[i].begin(), t[i].end(), 0.0);
        }
    }
    
    void shape(){
        std::cout << "[" << h << ", " << w << "]" << std::endl;
    }
    
    void print(){
        std::cout << '[';
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if(j != 0) std::cout << ' ';
                std::cout << t[i][j];
            }
            if(i == h-1) std::cout << ']';
            std::cout << std::endl ;
        }
        std::cout << std::endl;
    }
    
    matrix transpose(){
        std::vector<std::vector<double> > v;
        v.resize(w);
        for (int i = 0; i < w; i++) {
            v[i].resize(h);
            for (int j = 0; j < h; j++) {
                v[i][j] = t[j][i];
            }
        }
        matrix m(v);
        return m;
    }
    
    void minus(matrix &mat){
        if (mat.h == h && mat.w == w) {
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    t[i][j] -= mat.t[i][j];
                }
            }
        }else{
            std::cout << "different shape minus" << std::endl;
        }
    }
    
    void dot(matrix &t2){
        if(w != t2.h){
            std::cout << "dot Shape error." << std::endl;
            return;
        }else{
            std::vector<std::vector<double> > t3;
            std::vector<std::vector<double> > dotT;
            w = t2.w;
            dotT.resize(w);
            for (int j = 0; j < w; j++) {
                std::vector<double> v = t2.get_vector_coln(j);
                dotT[j] = v;
            }
            
            t3.resize(h);
            for (int i = 0; i < h; i++) {
                t3[i].resize(t2.w);
                for (int j = 0; j < w; j++) {
                    std::vector<double> v = t2.get_vector_coln(j);
                    t3[i][j] = dot_vec(t[i], dotT[j]);
                }
            }
            t = t3;
        }
    }
    
    matrix sum_column(){
        std::vector<std::vector<double> > v;
        v.resize(1);
        v[0].resize(w);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                v[0][j] += t[i][j];
            }
        }
        matrix m(v);
        return m;
    }
};

#endif // MATRIX_HPP
