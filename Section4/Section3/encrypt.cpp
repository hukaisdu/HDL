#include<iostream>
#include<random>
#include<iomanip>
#include"permutation.h"
#include"endian.h"
#include<string>
#include<cmath>
#include<tuple>
#include<cstdio>
#include<set>
#include<cassert>
using namespace std;
typedef unsigned long long u64;

void printState( int S[][64] )
{
    for ( int i = 0; i < 5; i++ )
    {
        for ( int j = 0; j < 64; j++ )
            cout << S[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

int main( int argc, char * argv[] )
{
    state s { 0L, 0L, 0L, 0L, 0L };
    
    s.x2 = 0xffffffffffffffffL;
    s.x3 = 0xffffffffffffffffL;

    s.x0 ^= 1;
    s.x1 ^= 0;
    s.x2 ^= 0;
    s.x3 ^= 1;
    s.x4 ^= 1;


    P3();

    int S[5][64];
    for (int i = 0; i < 64; i++ )
    {
        S[0][i] = s.x0 >> ( 63 - i ) & 0x1;
        S[1][i] = s.x1 >> ( 63 - i ) & 0x1;
        S[2][i] = s.x2 >> ( 63 - i ) & 0x1;
        S[3][i] = s.x3 >> ( 63 - i ) & 0x1;
        S[4][i] = s.x4 >> ( 63 - i ) & 0x1;
     }

    printState( S );
}

