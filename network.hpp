#ifndef NN_HPP
#define NN_HPP
#include "matrix.hpp"
#include <cstdio>
#include <ctime>

class affine_relu{
public:
    matrix w_list;
    matrix prime_w_list;
    matrix x_transpose;
    matrix ada_grad;
    matrix velocity_matrix;
    
    void relu(){
        for (int i = 0; i < w_list.h; i++) {
            for (int j = 0; j < w_list.w; j++) {
                w_list.t[i][j] = (w_list.t[i][j] > 0)? w_list.t[i][j]: 0;
            }
        }
    }
    
    matrix forward(vector<vector<double> > in){
        matrix in_matrix(in);
        
        return in_matrix;
    }
    
    matrix backward(){
        
    }
};

class softmax_cee{
public:
    matrix w_list;
    matrix prime_w_list;
    matrix x_transpose;
    matrix ada_grad;
    matrix velocity_matrix;
    
    void softmax(matrix &in){
        for (int i = 0; i < in.w; i++) {
            double sum = 0;
            for (int j = 0; j < in.w; j++) {
                sum += exp(in.t[i][j]);
            }
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] = exp(in.t[i][j])/sum;
            }
        }
    }
    
    matrix cross_entropy(vector<int> correct_vec, int out, int mini_badge){
        vector<vector<double> > v;
        v.resize(mini_badge);
        for (int i = 0; i < mini_badge; i++) {
            v[i].resize(out);
            fill(v[i].begin(), v[i].end(), 0.0);
            v[i][correct_vec[i]] = -log(t[i][correct_vec[i]]);
        }
        return v;
    }
};

class network{
public:
    vector<affine_relu> affine;
    softmax_cee softmax;
    int input, hide, hide_neuron, output, mini_badge;
    
    network(int _input, int _hide, int _hide_neuron, int _output, int _mini_badge){
        input = _input;
        hide = _hide;
        hide_neuron = _hide_neuron;
        output = _output;
        mini_badge = _mini_badge;
    }
    
    void save_network(){
        cout << 7 << endl;
        time_t t = time(NULL);
        cout << ctime(&t) << endl;
        cout << "input:" << input << endl;
        cout << "hide:" << hide << endl;
        cout << "hide_neuron" << hide_neuron << endl;
        cout << "output" << output << endl;
        cout << "mini_badge" << mini_badge << endl;
        
        for (int i = 0; i < affine.size(); i++) {
            cout << "W" << i+1 << endl;
            for (int j = 0; j < affine[i].w_list.h; j++) {
                for (int k = 0; k < affine[i].w_list.w; k++) {
                    if (k != 0) cout << ' ';
                    cout << affine[i].w_list.t[j][k];
                }
                cout << endl;
            }
            cout << endl;
        }
        
        
    }
    
    vector<double> predict(vector<double> in){
        vector<double> out;
        affine[0].
    }
    
    void adam_lite(double leaning_rate, matrix &ada_grad, matrix &velocity_matrix, matrix &prime, matrix w_list){
        for (int i = 0; i < prime.h; i++) {
            for (int j = 0; j < prime.w; j++) {
                ada_grad.t[i][j] += prime.t[i][j]*prime.t[i][j];
                velocity_matrix.t[i][j] = 0.9*velocity_matrix.t[i][j] - (leaning_rate/sqrt(ada_grad.t[i][j] + 0.0000001))*prime.t[i][j];
                w_list.t[i][j] += velocity_matrix.t[i][j];
            }
        }
    }
};

#endif // NN_HPP
