#ifndef NN_HPP
#define NN_HPP
#include "matrix.hpp"
#include <cstdio>
#include <ctime>
#include <fstream>

class affine_relu{
public:
    matrix w_list;
    matrix x_transpose;
    matrix prime_w_list;
    matrix ada_grad;
    matrix velocity_matrix;
    
    affine_relu(){}
    
    void relu(matrix &in){
        for (int i = 0; i < in.h; i++) {
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] = (in.t[i][j] > 0)? in.t[i][j]: 0;
            }
        }
    }
    
    void forward(matrix &in){
        x_transpose = in.transpose();
        in.dot(w_list);
        relu(in);
    }
    
    void backward(matrix &in){
        //relu
        for (int i = 0; i < in.h; i++) {
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] = (x_transpose.t[j][i] > 0)? in.t[i][j]: 0;
            }
        }
        //affine
        x_transpose.dot(in);
        
        prime_w_list = x_transpose;
        for (int i = 0; i < prime_w_list.h; i++) {
            for (int j = 0; j < prime_w_list.w; j++) {
                ada_grad.t[i][j] += prime_w_list.t[i][j]*prime_w_list.t[i][j];
            }
        }
    }
};

class softmax_cee{
public:
    matrix w_list;
    matrix prime_w_list;
    matrix x_transpose;
    matrix ada_grad;
    matrix velocity_matrix;
    matrix cross_entropy_list;
    
    softmax_cee(){}
    
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
    
    void cross_entropy(matrix &in){
        
    }
    
    void forward(matrix &in){
        x_transpose = in.transpose();
        in.dot(w_list);
        softmax(in);
        cross_entropy(in);
    }
    
    void backward(matrix &in){
        //affine
        x_transpose.dot(in);
        
        prime_w_list = x_transpose;
        for (int i = 0; i < prime_w_list.h; i++) {
            for (int j = 0; j < prime_w_list.w; j++) {
                ada_grad.t[i][j] += prime_w_list.t[i][j]*prime_w_list.t[i][j];
            }
        }
    }
};

class network{
public:
    std::vector<affine_relu> affine;
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
        std::ofstream outputfile("othelloFile/NNprams.txt");
        
        outputfile << 7 << std::endl;
        time_t t = time(NULL);
        outputfile << ctime(&t) << std::endl;
        outputfile << "input " << input << std::endl;
        outputfile << "hide " << hide << std::endl;
        outputfile << "hide_neuron " << hide_neuron << std::endl;
        outputfile << "output " << output << std::endl;
        outputfile << "mini_badge " << mini_badge << std::endl;
        
        outputfile << "affine " << affine.size() << std::endl;
        for (int i = 0; i < affine.size(); i++) {
            outputfile << affine[i].w_list.h << ' ' << affine[i].w_list.w << std::endl;
            for (int j = 0; j < affine[i].w_list.h; j++) {
                for (int k = 0; k < affine[i].w_list.w; k++) {
                    if (k != 0) outputfile << ' ';
                    outputfile << affine[i].w_list.t[j][k];
                }
                outputfile << std::endl;
            }
            outputfile << std::endl;
        }
        outputfile << "softmax" << std::endl;
        outputfile << softmax.w_list.h << ' ' << softmax.w_list.w << std::endl;
        for (int i = 0; i < softmax.w_list.h; i++) {
            for (int j = 0; j < softmax.w_list.w; j++) {
                
            }
        }
    }
    
    void forward_and_back(matrix &in){
        for(int i = 0; i < affine.size(); i++){
            affine[i].forward(in);
        }
        softmax.forward(in);
        softmax.backward(in);
        for(int i = affine.size() - 1; i >= 0; i--){
            affine[i].backward(in);
        }
    }
    
    void adam_lite(double leaning_rate, matrix &ada_grad, matrix &velocity_matrix, matrix &prime_w_list, matrix &w_list){
        for (int i = 0; i < prime_w_list.h; i++) {
            for (int j = 0; j < prime_w_list.w; j++) {
                ada_grad.t[i][j] += prime_w_list.t[i][j]*prime_w_list.t[i][j];
                velocity_matrix.t[i][j] = 0.9*velocity_matrix.t[i][j] - (leaning_rate/sqrt(ada_grad.t[i][j] + 0.0000001))*prime_w_list.t[i][j];
                w_list.t[i][j] += velocity_matrix.t[i][j];
            }
        }
    }
    
    void leaning(double leaning_rate){
        for (int i = 0; i < affine.size(); i++) {
            adam_lite(leaning_rate, affine[i].ada_grad, affine[i].velocity_matrix, affine[i].prime_w_list, affine[i].w_list);
        }
        adam_lite(leaning_rate, softmax.ada_grad, softmax.velocity_matrix, softmax.prime_w_list, softmax.w_list);
    }
};

#endif // NN_HPP
