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
    u64 xorsum0 = 0;
    u64 xorsum1 = 0;
    u64 xorsum2 = 0;
    u64 xorsum3 = 0;
    u64 xorsum4 = 0;

    u64 X[5] =     { 0, 0, 1, 1, 0 }; //0x6
    u64 Delta[5] = { 1, 0, 0, 1, 1 }; //0x13

    // load \Delta
    for ( u64 x = 0; x < ( 1 << 23 ); x++ )
    {
        // initialialstate
        u64 S[5][64] = { 0L };

        // load X and Delta
        for ( int i = 0; i < 64; i++ )
        {
            if ( ( x >> ( 63 - i ) & 0x1 ) == 1 ) // active
                for( int j = 0; j < 5; j++ )
                    if ( Delta[j] == 1 )
                        S[j][i] = X[j] ^ 1;
                    else
                        S[j][i] = X[j];
            else
                for( int j = 0; j < 5; j++ )
                    S[j][i] = X[j];
        }

        state s { 0L, 0L, 0L, 0L, 0L };

        for ( int i = 0; i < 64; i++ )
        {
            s.x0 ^= S[0][i] << ( 63 - i ) ;
            s.x1 ^= S[1][i] << ( 63 - i ) ;
            s.x2 ^= S[2][i] << ( 63 - i ) ;
            s.x3 ^= S[3][i] << ( 63 - i ) ;
            s.x4 ^= S[4][i] << ( 63 - i ) ;
        }

        P7();

        xorsum0 ^= s.x0;
        xorsum1 ^= s.x1;
        xorsum2 ^= s.x2;
        xorsum3 ^= s.x3;
        xorsum4 ^= s.x4;
    }

    cout << hex << xorsum0 << endl;
    cout << hex << xorsum1 << endl;
    cout << hex << xorsum2 << endl;
    cout << hex << xorsum3 << endl;
    cout << hex << xorsum4 << endl;
}

