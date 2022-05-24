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
using namespace std;

mutex _mutex;
typedef unsigned long long u64;

const u64 DIM = 2;

const u64 DIFFNUM = ( 1L << DIM );

const int THREAD = 1;

u64 IV = 0x80400c0600000000;

const int N = (1 << 14) * THREAD;
const int KEY = 1 << 5;

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<uint64_t> dis( 0, u64(0UL - 1UL) );
std::uniform_int_distribution<int> disint( 0, 63 );

vector<int> S;

int ID;

int encrypt( const u64 key0, const u64 key1, const pair< u64, u64> & N, const int pos) 
{
    state s { IV, key0, key1, N.first, N.second };
    P5();
    return ( s.x0 >> ( 63 - pos ) & 0x1 ); 
}

void impose ( u64 & key0, u64 & key1, u64 & N0, u64 & N1 )
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

    int id[7] = { 0 };
    for ( int i = 0; i < 7; i++ )
        id[i] = ID >> ( 6 - i ) & 0x1;

     k[0] = 0;
     k[7] = 0; 
     k[64] = 0;
     k[71] = 0;

     x[0] = x[64];
     x[7] = x[71];

     x[16] = x[19] ^ x[49] ^ x[80] ^ x[83]*k[19] ^ x[83] ^ x[90]*k[26] ^ x[113]*k[49] ^ x[113] ^ k[9] ^ k[16] ^ k[19] ^ k[49] ^ k[73] ^ k[80] ^ k[90];
     x[67] = x[0] ^ x[3]*k[3] ^ x[3]*k[67] ^ x[3] ^ x[25]*k[25] ^ x[25]*k[89] ^ x[25] ^ x[64] ^ x[89] ^ k[3]*k[67] ^ k[3] ^ k[25]*k[89] ^ k[25] ^ k[67] ^ k[89] ^ 1;
     x[74] = x[10]*k[10] ^ x[10]*k[74] ^ x[10] ^ x[32]*k[32] ^ x[32]*k[96] ^ x[32] ^ x[96] ^ k[10]*k[74] ^ k[10] ^ k[32]*k[96] ^ k[32] ^ k[74] ^ k[96];
     // step 2
     x[30] = x[54] ^ x[61] ^ x[94]*k[30] ^ x[94] ^ x[118] ^ x[125] ^ k[54] ^ k[61] ^ k[118] ^ k[125] ^ 1;
     x[102] = x[35]*k[35] ^ x[35]*k[99] ^ x[35] ^ x[38]*k[38] ^ x[38]*k[102] ^ x[38] ^ x[60]*k[60] ^ x[60]*k[124] ^ x[60] ^ x[99] ^ x[124] ^ k[35]*k[99] ^ k[35] ^ k[38]*k[102] ^ k[38] ^ k[60]*k[124] ^ k[60] ^ k[99] ^ k[102] ^ k[124];

     if ( id[0] )
     x[2] = x[12] ^ x[42] ^ x[66] ^ x[76]*k[12] ^ x[76] ^ x[83]*k[19] ^ x[106]*k[42] ^ x[106] ^ k[2] ^ k[9] ^ k[12] ^ k[42] ^ k[66] ^ k[73] ^ k[83];
     if ( id[1] )
     x[11] = x[18] ^ x[21] ^ x[51] ^ x[75] ^ x[82] ^ x[85]*k[21] ^ x[85] ^ x[92]*k[28] ^ x[115]*k[51] ^ x[115] ^ k[11] ^ k[18] ^ k[51] ^ k[75] ^ k[82] ^ k[92] ^ 1;
     if ( id[2] )
     x[95] = x[28]*k[28] ^ x[28]*k[92] ^ x[28] ^ x[31]*k[31] ^ x[31]*k[95] ^ x[31] ^ x[53]*k[53] ^ x[53]*k[117] ^ x[53] ^ x[92] ^ x[117] ^ k[28]*k[92] ^ k[28] ^ k[31]*k[95] ^ k[31] ^ k[53]*k[117] ^ k[53] ^ k[92] ^ k[95] ^ k[117];
     if ( id[3] )
     x[93] = x[26]*k[26] ^ x[26]*k[90] ^ x[26] ^ x[29]*k[29] ^ x[29]*k[93] ^ x[29] ^ x[51]*k[51] ^ x[51]*k[115] ^ x[51] ^ x[90] ^ x[115] ^ k[26]*k[90] ^ k[26] ^ k[29]*k[93] ^ k[29] ^ k[51]*k[115] ^ k[51] ^ k[90] ^ k[93] ^ k[115] ^ 1;
     if ( id[4] )
     x[58] = x[18] ^ x[25] ^ x[28] ^ x[82] ^ x[89] ^ x[92]*k[28] ^ x[92] ^ x[99]*k[35] ^ x[122]*k[58] ^ x[122] ^ k[18] ^ k[25] ^ k[28] ^ k[58] ^ k[82] ^ k[89] ^ k[99] ^ 1;
     if ( id[5] )
     x[23] = x[47] ^ x[54] ^ x[57] ^ x[87]*k[23] ^ x[87] ^ x[111] ^ x[118] ^ x[121]*k[57] ^ x[121] ^ k[23] ^ k[47] ^ k[54] ^ k[57] ^ k[111] ^ k[118];
     if ( id[6] )
     x[86] = x[19]*k[19] ^ x[19]*k[83] ^ x[19] ^ x[22]*k[22] ^ x[22]*k[86] ^ x[22] ^ x[44]*k[44] ^ x[44]*k[108] ^ x[44] ^ x[83] ^ x[108] ^ k[19]*k[83] ^ k[19] ^ k[22]*k[86] ^ k[22] ^ k[44]*k[108] ^ k[44] ^ k[83] ^ k[86] ^ k[108];


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

double Accumulate( u64 key0, u64 key1, const vector<u64> & diff, int pos ) 
{
    double myaccum = 0;

    for ( int data = 0; data < N; data++ )
    {
        u64 Nonce0 = dis( gen );
        u64 Nonce1 = dis( gen );

        impose ( key0, key1, Nonce0, Nonce1 );

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

int weight( int x )
{
    int w = 0;
    for ( int i = 0; i < 9; i++ )
        w += x >> ( 8 - i ) & 0x1;
    return w;
}

int main( int argc, char * argv[] )
{
    ID = stoi ( argv[1] );

    if ( weight( ID ) != 1 )
        return -1;

    vector<int> S { 0, 7 };
    int pos = 25;

    vector<u64> diff;

    for ( auto & it : S )
        diff.push_back( 1L << ( 63 -  it ) );

    double bias = 0;
    for ( int test = 0; test < KEY; test++ )
    {
        u64 key0 = dis( gen ); 
        u64 key1 = dis( gen ); 

        double accm = Accumulate( key0, key1, diff, pos );

        bias += ( accm / (N) ) - 0.5;
    }

    cout << ID << " ";
    if ( bias != 0 )
        cout << bias / KEY << " = " << log( abs( bias ) / KEY ) / log( 2 ) << endl;
    else
        cout << bias / KEY  << endl;
}

