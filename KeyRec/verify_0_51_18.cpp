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

     x[0] = x[64];
     x[51] = x[115];

     int id[7] = { 0 };
     for ( int i = 0; i < 7; i++ )
         id[i] = ID >> ( 6 - i ) & 0x1;

     k[0]   = id[0] ^ 0;
     k[64]  = id[1] ^ 0;
     k[51]  = id[2] ^ 0;
     k[115] = id[3] ^ 0;

     //if ( id[0] )
     //x[41]  = 1 ^ x[48] ^ x[58] ^ x[105] ^ x[112] ^ x[122] ^ k[41] ^ k[48] ^ k[58] ^ k[105] ^ k[112] ^ k[122];

     //if ( id[1] )
     x[67]  = id[4] ^ x[3]*k[3] ^ x[3]*k[67] ^ x[3] ^ x[25]*k[25] ^ x[25]*k[89] ^ x[25] ^ x[89] ^ k[3]*k[67] ^ k[3] ^ k[25]*k[89] ^ k[25] ^ k[67] ^ k[89] ^ 1;

     //if ( id[2] )
     //x[125] = 1 ^ x[19]*k[19] ^ x[19]*k[83] ^ x[19] ^ x[58]*k[58] ^ x[58]*k[122] ^ x[61]*k[61] ^ x[61]*k[125] ^ x[61] ^ x[83] ^ x[122] ^ k[19]*k[83] ^ k[19] ^ k[58]*k[122] ^ k[61]*k[125] ^ k[61] ^ k[83] ^ k[122] ^ k[125];

     //if ( id[3] )
     x[76]  = id[5] ^ 1 ^ x[12]*k[12] ^ x[12]*k[76] ^ x[12] ^ x[54]*k[54] ^ x[54]*k[118] ^ x[54] ^ x[118] ^ k[12]*k[76] ^ k[12] ^ k[54]*k[118] ^ k[54] ^ k[76] ^ k[118] ^ 1;

     //if ( id[4] )
     //x[11]  = x[18] ^ x[28] ^ x[75] ^ x[82] ^ x[92] ^ k[11] ^ k[18] ^ k[28] ^ k[75] ^ k[82] ^ k[92];

     //if ( id[5] )
     //x[95]  = x[28]*k[28] ^ x[28]*k[92] ^ x[28] ^ x[31]*k[31] ^ x[31]*k[95] ^ x[31] ^ x[53]*k[53] ^ x[53]*k[117] ^ x[53] ^ x[92] ^ x[117] ^ k[28]*k[92] ^ k[28] ^ k[31]*k[95] ^ k[31] ^ k[53]*k[117] ^ k[53] ^ k[92] ^ k[95] ^ k[117];
     
     //if ( id[6] )
     //x[2]   = id[6] ^ x[12] ^ x[42] ^ x[66] ^ x[76]*k[12] ^ x[76] ^ x[83]*k[19] ^ x[106]*k[42] ^ x[106] ^ k[2] ^ k[9] ^ k[12] ^ k[42] ^ k[66] ^ k[73] ^ k[83];

     //if ( id[7] )
     //x[30]  = 1 ^ x[39] ^ x[41] ^ x[48] ^ x[94]*k[30] ^ x[103]*k[39] ^ x[105] ^ x[112] ^ x[122]*k[58] ^ x[122] ^ k[30]*k[94] ^ k[39]*k[103] ^ k[39] ^ k[41] ^ k[48] ^ k[58]*k[122] ^ k[58] ^ k[94] ^ k[103] ^ k[105] ^ k[112];

     //if ( id[8] )
     //x[23]  = x[47] ^ x[54] ^ x[57] ^ x[87]*k[23] ^ x[87] ^ x[111] ^ x[118] ^ x[121]*k[57] ^ x[121] ^ k[23] ^ k[47] ^ k[54] ^ k[57] ^ k[111] ^ k[118];

     //if ( id[9] )
     //x[32]  = x[23] ^ x[30] ^ x[34] ^ x[39] ^ x[48] ^ x[87]*k[23] ^ x[94]*k[30] ^ x[96]*k[32] ^ x[98] ^ x[103]*k[39] ^ x[112] ^ x[122]*k[58] ^ x[122] ^ k[23]*k[87] ^ k[23] ^ k[30]*k[94] ^ k[32]*k[96] ^ k[32] ^ k[34] ^ k[39]*k[103] ^ k[39] ^ k[48] ^ k[58]*k[122] ^ k[58] ^ k[87] ^ k[94] ^ k[96] ^ k[98] ^ k[103] ^ k[112] ^ 1;
     
     //if ( id[10] )
     //x[108] = x[19]*k[19] ^ x[19]*k[83] ^ x[19] ^ x[22]*k[22] ^ x[22]*k[86] ^ x[22] ^ x[44]*k[44] ^ x[44]*k[108] ^ x[44] ^ x[83] ^ x[86] ^ k[19]*k[83] ^ k[19] ^ k[22]*k[86] ^ k[22] ^ k[44]*k[108] ^ k[44] ^ k[83] ^ k[86] ^ k[108];

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

        impose ( key0, key1, Nonce0, Nonce1);

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
    int s = 0;
    for ( int i = 0; i < 12; i++ )
        s += x >> ( 11 - i ) & 0x1;
    return s;
}

int main( int argc, char * argv[] )
{
    ID = stoi( argv[1] );


    vector<int> S { 0, 51 };
    int pos = 18;

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

    double ep = log( abs( bias ) / KEY ) / log(2);
    cout << ID << " " << ep << endl;
}
