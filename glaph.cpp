#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
#include<cstring>
#include<vector>
#include<cmath>
#include<queue>
#include<stack>
#include<unistd.h>
#include"matplotlib.hpp"

using namespace std;
#define INT(x) int x; scanf("%d",&x)
#define LL(x) ll x; scanf("%lld",&x)
#define INPUT(x) cin >> x
#define REP1(x,n) for(int x = 0; x < n; x++)
#define REP2(x,s,e) for(int x = s; x <= e; x++)
#define RREP1(x,n) for(int x = n-1; x >= 0; x--)
#define RREP2(x,s,e) for(int x = s; x >= e; x--)
#define BR printf("\n")
#define INF_INT 2147483647
#define INF_LL 9223372036854775807

typedef long long ll;

int main(){
    matplotlib g;
    g.open();
    
    g.screen(0, 0, 10, 10);
    int x = 0, y = 0;
    
    for (int i = 0; i < 10; i++) {
        g.line(x,y,x+1,y+1);
        usleep(5 * 100000);
        x++;
        y++;
    }
    
    g.save("test.pdf");
    while (1) {
    }
    return 0;
}
