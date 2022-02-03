#include<iostream>
#include<random>
#include<iomanip>
#include"permutation.h"
#include"endian.h"
#include<string>
#include<cmath>
#include<tuple>
#include<mutex>
#include<thread>
#include<cstdio>
#include<set>
using namespace std;

mutex _mutex;
typedef unsigned long long u64;

const u64 DIM = 10;
const u64 DIFFNUM = 1L << DIM;

const int THREAD = 1;

typedef array< pair<u64, u64>, DIFFNUM > HD;

u64 IV = 0x80400c0600000000;

const int N = 1 << 16;
const int KEY = 1 << 10;

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<uint64_t> dis( 0, u64(0UL - 1UL) );
std::uniform_int_distribution<int> disint( 0, 63 );

int encrypt( const u64 key0, const u64 key1, const pair< u64, u64> & N, const int pos) 
{
    state s { IV, key0, key1, N.first, N.second };
    P5();
    //return ( ( s.x0 >> ( 63 - pos ) & 0x1 ) << 4 ) |
    //       ( ( s.x1 >> ( 63 - pos ) & 0x1 ) << 3 ) |
    //       ( ( s.x2 >> ( 63 - pos ) & 0x1 ) << 2 ) |
    //       ( ( s.x3 >> ( 63 - pos ) & 0x1 ) << 1 ) |
    //       ( ( s.x4 >> ( 63 - pos ) & 0x1 ) << 0 ); 
    return s.x0 >> (63 - pos ) & 0x1;
}

int Accumulate( u64 key0, u64 key1, const vector<u64> & diff, int pos ) 
{
    int accum = 0;
    for ( int data = 0; data < N; data++ )
    {
        u64 Nonce0 = dis( gen );
        u64 Nonce1 = dis( gen );

        for ( auto d : diff )
        {
            Nonce0 &= 0xffffffffffffffffUL ^ d;
            Nonce1 &= 0xffffffffffffffffUL ^ d;
            /*
            for ( int i = 0; i < 64; i++ )
                if ( d >> (63 - i) & 0x1 )
                {
                    u64 a = Nonce0 >> ( 63 - i ) & 0x1; 
                    Nonce1 &= 0xffffffffffffUL ^ d; 
                    Nonce1 ^= a << (63 - i); 
                }
            */
            //key0 &= 0xffffffffffffffffUL ^ d;
            //key1 &= 0xffffffffffffffffUL ^ d;
        }

        int res = 0;
        for ( int j = 0; j < DIFFNUM; j++ )
        {
            u64 dv = 0L;
            for ( int k = 0; k < DIM; k++ )
            {
                if ( j >> ( DIM - 1 - k ) & 0x1 )
                    dv ^= diff[k];
            }

            res ^= encrypt( key0, key1, make_pair( Nonce0^dv, Nonce1^dv ) , pos );
        }

        if ( res == 0 )
            accum += 1;
    }

    return accum;
}


int main( int argc, char * argv[] )
{
    vector<int> dim7  { 5,  9,  10, 18, 27, 52, 60, 36 }; 
    vector<int> dim8  { 0,  3,  10, 11, 26, 28, 31, 55, 60 }; 
    vector<int> dim9  { 8,  13, 14, 16, 21, 25, 39, 42, 46, 12 }; 
    vector<int> dim10 { 4,  14, 23, 27, 35, 39, 41, 49, 51, 55, 0 }; 
    vector<int> dim11 { 19, 24, 33, 35, 36, 48, 54, 57, 59, 62, 63, 27 }; 

    cout << "DIM: " << DIM << endl;
    set<int> S;
    int pos;

    if ( DIM == 7 )
    {
        for ( int i = 0; i < 7; i++ )
            S.insert( dim7[i] );
        pos = dim7[7];
    }
    else if ( DIM == 8 )
    {
        for ( int i = 0; i < 8; i++ )
            S.insert( dim8[i] );
        pos = dim8[8];
    }
    else if ( DIM == 9 )
    {
        for ( int i = 0; i < 9; i++ )
            S.insert( dim9[i] );
        pos = dim9[9];
    }
    else if ( DIM == 10 )
    {
        for ( int i = 0; i < 10; i++ )
            S.insert( dim10[i] );
        pos = dim10[10];
    }
    else if ( DIM == 11 )
    {
        for ( int i = 0; i < 11; i++ )
            S.insert( dim11[i] );
        pos = dim11[11];
    }

    vector<u64> diff;
    for ( auto & it : S )
        diff.push_back( 1L << ( 63 -  it ) );

    double bias = 0;

    for ( int test = 0; test < KEY; test++ )
    {
        u64 key0 = dis( gen ); 
        u64 key1 = dis( gen ); 

        auto accum = Accumulate( key0, key1, diff, pos ); 

        cout << static_cast<double> (accum) / N - 0.5 << endl; 

        bias += static_cast<double>( accum ) / N - 0.5;
    }

    auto tb = bias / KEY;

    cout << tb << " " << log(tb) / log(2) << endl;
}

