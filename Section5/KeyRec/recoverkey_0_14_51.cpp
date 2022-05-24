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
#include<cassert>
#include<algorithm>

using namespace std;

mutex _mutex;
typedef unsigned long long u64;

const u64 DIM = 2;

const u64 DIFFNUM = ( 1L << DIM );

const int THREAD = 1;

u64 IV = 0x80400c0600000000;
//u64 IV = 0x0;

//const int N = 982;
const int N = 778;

std::random_device rd;
std::mt19937_64 gen( rd() );
std::uniform_int_distribution<uint64_t> dis( 0, u64(0UL - 1UL) );
std::uniform_int_distribution<int> disint( 0, 63 );


int encrypt( const u64 key0, const u64 key1, const pair< u64, u64> & N, const int pos) 
{
    state s { IV, key0, key1, N.first, N.second };
    P5();
    return ( s.x0 >> ( 63 - pos ) & 0x1 ); 
}


void guess ( u64 K0, u64 K1, u64 & N0, u64 & N1, const u64 value, int off )
{
    u64 x[128] = { 0 };
    for ( int i = 0; i < 64; i++ )
        x[i] = N0 >> (63 - i ) & 0x1;
    for ( int i = 0; i < 64; i++ )
        x[i + 64] = N1 >> (63 - i ) & 0x1;
    u64 k[128] = { 0 };
    for ( int i = 0; i < 64; i++ )
        k[i] = K0 >> (63 - i ) & 0x1;
    for ( int i = 0; i < 64; i++ )
        k[i + 64] = K1 >> (63 - i ) & 0x1;

     int ID[3] = { 0 };
     for ( int i = 0; i < 3; i++ )
         ID[i] = value >> ( 2 - i ) & 0x1;

     // set the type 0 condition
     x[0] = x[64];
     x[14] = x[64 + 14];

     x[33] = 0;  
     x[36] = 0; 
     x[58] = 0;
     x[97] = 0;
     x[122] = 0;

     x[28] = 0;
     x[31] = 0;
     x[53] = 0;
     x[92] = 0;
     x[117] = 0;

     x[23] = 0;
     x[54] = 0;
     x[57] = 0;
     x[87] = 0;
     x[111] = 0;
     x[118] = 0;
     x[121] = 0;

     x[100] = ID[0];
     x[95] = ID[1];
     x[47] = ID[2];

     //cout << hex << value << " " << a << b << c << endl;
    
    N0 = 0;
    for ( int i = 0; i < 64; i++ )
        N0 ^= x[i] << ( 63 - i );

    N1 = 0;
    for ( int i = 0; i < 64; i++ )
        N1 ^= x[i + 64] << ( 63 - i );
}

/*
void impose ( u64 & key0, u64 & key1, u64 & N0, u64 & N1, int indictor )
{
    u64 x[128] = { 0 };
    u64 k[128] = { 0 };

    for ( int i = 0; i < 64; i++ )
        x[i] = N0 >> (63 - i ) & 0x1;
    for ( int i = 0; i < 64; i++ )
        x[i + 64] = N1 >> (63 - i ) & 0x1;

    for ( int i = 0; i < 64; i++ )
        k[i] = key0 >> (63 - i ) & 0x1;
    for ( int i = 0; i < 64; i++ )
        k[i + 64] = key1 >> (63 - i ) & 0x1;

     x[0] = x[64];
     x[7] = x[71];

    x[16] = x[19] ^ x[49] ^ x[80] ^ x[83]*k[19] ^ x[83] ^ x[90]*k[26] ^ x[113]*k[49] ^ x[113] ^ k[9] ^ k[16] ^ k[19] ^ k[49] ^ k[73] ^ k[80] ^ k[90];

    x[67] = x[3]*k[3] ^ x[3]*k[67] ^ x[3] ^ x[25]*k[25] ^ x[25]*k[89] ^ x[25] ^ x[89] ^ k[3]*k[67] ^ k[3] ^ k[25]*k[89] ^ k[25] ^ k[67] ^ k[89] ^ 1;

    x[74] = x[10]*k[10] ^ x[10]*k[74] ^ x[10] ^ x[32]*k[32] ^ x[32]*k[96] ^ x[32] ^  x[96] ^ k[10]*k[74] ^ k[10] ^ k[32]*k[96] ^ k[32] ^ k[74] ^ k[96];

    N0 = 0;
    for ( int i = 0; i < 64; i++ )
        N0 ^= x[i] << ( 63 - i );

    N1 = 0;
    for ( int i = 0; i < 64; i++ )
        N1 ^= x[i + 64] << ( 63 - i );

    key0 = 0;
    for ( int i = 0; i < 64; i++ )
        key0 ^= k[i]  << ( 63 - i );

    key1 = 0;
    for ( int i = 0; i < 64; i++ )
        key1 ^= k[i + 64]  << ( 63 - i );
}
*/


double Accumulate( u64 key0, u64 key1, const vector<u64> & diff, int pos, int indicator, int off  ) 
{
    double myaccum = 0;

    for ( int data = 0; data < N; data++ )
    {
        u64 Nonce0 = dis( gen );
        u64 Nonce1 = dis( gen );

        // type 0 condition

        //impose( key0, key1, Nonce0, Nonce1, indicator );
        guess ( key0, key1,  Nonce0, Nonce1, indicator, off);

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
            myaccum ++;
    }

    return myaccum;
}

bool eq( int x0, int x1, int x2, int x3, int y0, int y1, int y2, int y3 )
{
    if ( x0 != y0 ) return false;
    if ( x1 != y1 ) return false;
    if ( x2 != y2 ) return false;
    if ( x3 != y3 ) return false;

    return true;
}


int main( int argc, char * argv[] )
{
    int test = 0;
    int success = 0;
    int failure = 0;

    int SUC = 0;
    int FAIL = 0;

    while ( test++ < 10000 )
    {
        u64 key0 = dis( gen ); 
        u64 key1 = dis( gen ); 

        //key0 &= 0x8fffbfffffffffff;
        //key1 &= 0x7fffbfffffffffff;

        //key0 |= 0x8100000000000000;
        //key1 |= 0x8100000000000000;

        bool flag = false;
        for ( int off = 0; off < 1; off++ )
        {
            for ( int indicator = 0; indicator < 8; indicator++ )
            {
                vector<int> S;
                S.push_back( 0 );
                S.push_back( 14 );

                int pos = 51;

                vector<u64> diff;

                for ( auto & it : S )
                    diff.push_back( 1L << ( 63 -  it ) );

               auto res = Accumulate( key0, key1, diff, pos, indicator, off) ;

               //if ( res >= 572 ) // said correct
               if ( res >= 452 ) // said correct
                   flag = true;
            }
        }
        int a = key0 >> (63 - 0) & 0x1;
        int b = key0 >> (63 - 14) & 0x1;
        int c = key1 >> (63 - 0) & 0x1;
        int d = key1 >> (63 - 14) & 0x1;

        //if ( ( ( a + b + c + d ) == 0 ) || ( ( a + b + c + d == 1 ) && ( d == 1 ) ) )
        //if ( eq( a, b, c, d, 0, 0, 0, 0) || eq( a, b, c, d, 0, 0, 0, 1) )
        if ( eq( a, b, c, d, 0, 0, 0, 0) )
            SUC += 1;
        else
            FAIL += 1;

        if ( flag )
        {
        if ( eq( a, b, c, d, 0, 0, 0, 0) )
                success += 1;
            else
                failure += 1;
        }
        //if ( flag )
        //    cout << a << b << c << d << endl;

    }

    cout << success * 1.0 / SUC << " " << failure * 1.0 / FAIL << endl;
}

