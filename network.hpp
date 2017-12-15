#ifndef NN_HPP
#define NN_HPP
#include "matrix.hpp"
#include <cstdio>
#include <ctime>
#include <random>
#include <fstream>

class affine_relu{
public:
    matrix w_list;
    matrix prime_w_list;
    matrix velocity_matrix;
    matrix ada_grad;
    
    matrix x_transpose;
    
    matrix bias;
    matrix bias_prime;
    matrix bias_velocity_matrix;
    matrix bias_ada_grad;
    
    affine_relu(){}
    
    void print_size(){
        std::cout << "affine(" << w_list.h << ", " << w_list.w << ")" << std::endl;
        std::cout << "bias(" << bias.h << ", " << bias.w << ")" << std::endl;
    }
    
    void init_with_he(int h, int w, std::default_random_engine engine, std::normal_distribution<> dist){
        w_list.sizeinit(h, w);
        prime_w_list.sizeinit(h, w);
        velocity_matrix.sizeinit(h, w);
        ada_grad.sizeinit(h, w);
        
        double s = sqrt(2.0/h);
        
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                w_list.t[i][j] = s*dist(engine);
            }
        }
        
        
        bias.sizeinit(1, w);
        bias_prime.sizeinit(1, w);
        bias_velocity_matrix.sizeinit(1, w);
        bias_ada_grad.sizeinit(1, w);
    }
    
    void add_bias(matrix &in){
        for (int i = 0; i < in.h; i++) {
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] += bias.t[0][j];
            }
        }
    }
    
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
        add_bias(in);
        relu(in);
    }
    
    void backward(matrix &in){
        //relu
        for (int i = 0; i < in.h; i++) {
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] = (in.t[i][j] > 0)? in.t[i][j]: 0;
            }
        }
        //bias
        bias_prime = in.sum_column();
        for (int i = 0; i < bias_prime.w; i++) {
            bias_ada_grad.t[0][i] += bias_prime.t[0][i]*bias_prime.t[0][i];
        }
        //affine
        x_transpose.dot(in);
        prime_w_list = x_transpose;
        
        matrix wtrans;
        wtrans = w_list.transpose();
        in.dot(wtrans);
        
        for (int i = 0; i < prime_w_list.h; i++) {
            for (int j = 0; j < prime_w_list.w; j++) {
                ada_grad.t[i][j] += prime_w_list.t[i][j]*prime_w_list.t[i][j];
            }
        }
    }
};

class softmax_cee : public affine_relu{
public:
    matrix cross_entropy_list;
    matrix softmax_output;
    
    softmax_cee(){}
    
    void softmax(matrix &in){
        for (int i = 0; i < in.h; i++) {
            double sum = 0;
            for (int j = 0; j < in.w; j++) {
                sum += exp(in.t[i][j]);
            }
            for (int j = 0; j < in.w; j++) {
                in.t[i][j] = exp(in.t[i][j])/sum;
            }
        }
    }
    
    std::vector<double> cross_entropy(matrix &in, matrix &teacher){
        std::vector<double> v;
        v.resize(in.h);
        std::fill(v.begin(), v.end(), 0.0);
        for (int i = 0; i < in.h; i++) {
            for (int j = 0; j < in.w; j++) {
                v[i] += -teacher.t[i][j]*log(in.t[i][j]);
            }
        }
        return v;
    }
    
    void softmax_forward(matrix &in){
        x_transpose = in.transpose();
        in.dot(w_list);
        add_bias(in);
        //- relu
        softmax(in);
        softmax_output = in;
    }
    
    void softmax_backward(matrix &in){
        in.minus(softmax_output);
        backward(in);
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
        
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        std::normal_distribution<> dist(0.0, 1.0);
        
        affine.resize(hide);
        
        affine[0].init_with_he(input, hide_neuron, engine, dist);

        for (int i = 1; i < hide; i++) {
            affine[i].init_with_he(hide_neuron, hide_neuron, engine, dist);
        }

        softmax.init_with_he(hide_neuron, output, engine, dist);
    }
    
    void save_network(){
        std::ofstream outputfile("NNpram/NNprams.txt");
        
        outputfile << 7 << std::endl;
        time_t t = time(NULL);
        outputfile << ctime(&t);
        outputfile << "input " << input << std::endl;
        outputfile << "hide " << hide << std::endl;
        outputfile << "hide_neuron " << hide_neuron << std::endl;
        outputfile << "output " << output << std::endl;
        outputfile << "mini_badge " << mini_badge << std::endl << std::endl;
        
        outputfile << "affine " << affine.size() << std::endl;
        for (int i = 0; i < affine.size(); i++) {
            outputfile << affine[i].w_list.h << ' ' << affine[i].w_list.w << std::endl;
            for (int j = 0; j < affine[i].w_list.h; j++) {
                for (int k = 0; k < affine[i].w_list.w; k++) {
                    if (k != 0) outputfile << ' ';
                    outputfile << affine[i].w_list.t[j][k];
                }
                outputfile << std::endl;
                if (j == affine[i].w_list.h - 1) {
                    outputfile << "bias " << affine[i].bias.w << std::endl;
                    for (int bia = 0; bia < affine[i].bias.w; bia++) {
                        if(bia != 0) outputfile << ' ';
                        outputfile << affine[i].bias.t[0][bia];
                    }
                    outputfile << std::endl;
                }
            }
            outputfile << std::endl;
        }
        outputfile << "softmax" << std::endl;
        outputfile << softmax.w_list.h << ' ' << softmax.w_list.w << std::endl;
        for (int i = 0; i < softmax.w_list.h; i++) {
            for (int j = 0; j < softmax.w_list.w; j++) {
                if(j != 0) outputfile << ' ';
                outputfile << softmax.w_list.t[i][j];
            }
            outputfile << std::endl;
        }
        
        outputfile << "bias " << softmax.bias.w << std::endl;
        for (int bia = 0; bia < softmax.bias.w; bia++) {
            if(bia != 0) outputfile << ' ';
            outputfile << softmax.bias.t[0][bia];
        }
        
        outputfile.close();
    }
    
    std::vector<double> prediction(std::vector<double> in){
        std::vector<std::vector<double> > dvec;
        dvec.push_back(in);
        matrix matin(dvec);
        for(int i = 0; i < affine.size(); i++){
            affine[i].forward(matin);
        }
        return matin.t[0];
    }
    
    double calculate_error(matrix &in, matrix &teacher){
        for(int i = 0; i < affine.size(); i++){
            affine[i].forward(in);
        }
        
        softmax.softmax_forward(in);
        std::vector<double> v;
        v = softmax.cross_entropy(in, teacher);
        
        double mean = 0;
        for (int i = 0; i < v.size(); i++) {
            mean += v[i]/v.size();
        }
        return mean;
    }
    
    void for_and_backward(matrix &in, matrix &teacher){
        for(int i = 0; i < affine.size(); i++){
            affine[i].forward(in);
        }
        
        softmax.softmax_forward(in);
        softmax.softmax_backward(teacher);
        
        for(int i = affine.size() - 1; i >= 0; i--){
            affine[i].backward(teacher);
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
            adam_lite(leaning_rate, affine[i].bias_ada_grad, affine[i].bias_velocity_matrix, affine[i].bias_prime, affine[i].bias);
        }
        adam_lite(leaning_rate, softmax.ada_grad, softmax.velocity_matrix, softmax.prime_w_list, softmax.w_list);
        adam_lite(leaning_rate, softmax.bias_ada_grad, softmax.bias_velocity_matrix, softmax.bias_prime, softmax.bias);
    }
};

#endif // NN_HPP