#include <iostream>
#include <vector>
#include "matrix.hpp"

using namespace std;

int main(){
    
    vector<vector<double> > ar,br;
    double a[2][2] = {
        {2, 1},
        {4, 3}
    };
    
    ar.resize(2);
    
    for (int i = 0; i < 2; i++) {
        ar[i].resize(2);
        for (int j = 0; j < 2; j++) {
            ar[i][j] = a[i][j];
        }
    }
    matrix arm(ar);
    
    arm.print();
    arm.shape();
    
    arm.dot(arm);
    arm.print();
    
    
    
    return 0;
}
