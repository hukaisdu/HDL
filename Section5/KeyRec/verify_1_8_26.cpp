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

const int N = (1 << 10) * THREAD;
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

    x[1] = x[65];
    x[8] = x[72];

    k[1] = 0;
    k[65] = 0;
    k[8] = 0;
    k[72] = 0;

x[103] = x[36]*k[36] ^ x[36]*k[100] ^ x[36] ^ x[39]*k[39] ^ x[39]*k[103] ^ x[39] ^ x[61]*k[61] ^ x[61]*k[125] ^ x[61] ^ x[100] ^ x[125] ^ k[36]*k[100] ^ k[36] ^ k[39]*k[103] ^ k[39] ^ k[61]*k[125] ^ k[61] ^ k[100] ^ k[103] ^ k[125];
x[12] = x[19] ^ x[22] ^ x[29] ^ x[52] ^ x[76] ^ x[83] ^ x[86]*k[22] ^ x[86] ^ x[93]*k[29] ^ x[93] ^ x[116]*k[52] ^ x[116] ^ k[12] ^ k[19] ^ k[22] ^ k[29] ^ k[52] ^ k[76] ^ k[83] ^ k[93];
x[96] = x[29]*k[29] ^ x[29]*k[93] ^ x[29] ^ x[32]*k[32] ^ x[32]*k[96] ^ x[32] ^ x[54]*k[54] ^ x[54]*k[118] ^ x[54] ^ x[93] ^ x[118] ^ k[29]*k[93] ^ k[29] ^ k[32]*k[96] ^ k[32] ^ k[54]*k[118] ^ k[54] ^ k[93] ^ k[96] ^ k[118] ^ 1;
x[68] = x[4]*k[4] ^ x[4]*k[68] ^ x[4] ^ x[26]*k[26] ^ x[26]*k[90] ^ x[26] ^ x[90] ^ k[4]*k[68] ^ k[4] ^ k[26]*k[90] ^ k[26] ^ k[68] ^ k[90];
x[59] = x[19] ^ x[26] ^ x[29] ^ x[83] ^ x[90] ^ x[93]*k[29] ^ x[93] ^ x[100]*k[36] ^ x[123]*k[59] ^ x[123] ^ k[19] ^ k[26] ^ k[59] ^ k[83] ^ k[90] ^ k[100] ^ 1;
x[75] = x[11]*k[11] ^ x[11]*k[75] ^ x[11] ^ x[33]*k[33] ^ x[33]*k[97] ^ x[33] ^ x[97] ^ k[11]*k[75] ^ k[11] ^ k[33]*k[97] ^ k[33] ^ k[75] ^ k[97];
x[87] = x[20]*k[20] ^ x[20]*k[84] ^ x[20] ^ x[23]*k[23] ^ x[23]*k[87] ^ x[23] ^ x[45]*k[45] ^ x[45]*k[109] ^ x[45] ^ x[84] ^ x[109] ^ k[20]*k[84] ^ k[20] ^ k[23]*k[87] ^ k[23] ^ k[45]*k[109] ^ k[45] ^ k[84] ^ k[87] ^ k[109] ^ 1;
x[31] = x[55] ^ x[62] ^ x[95]*k[31] ^ x[95] ^ x[119] ^ x[126] ^ k[31] ^ k[55] ^ k[62] ^ k[119] ^ k[126] ^ 1;
x[94] = x[27]*k[27] ^ x[27]*k[91] ^ x[27] ^ x[30]*k[30] ^ x[30]*k[94] ^ x[30] ^ x[52]*k[52] ^ x[52]*k[116] ^ x[52] ^ x[91] ^ x[116] ^ k[27]*k[91] ^ k[27] ^ k[30]*k[94] ^ k[30] ^ k[52]*k[116] ^ k[52] ^ k[91] ^ k[94] ^ k[116] ^ 1;
x[112] = x[24] ^ x[48] ^ x[55] ^ x[58] ^ x[88]*k[24] ^ x[88] ^ x[119] ^ x[122]*k[58] ^ x[122] ^ k[24] ^ k[48] ^ k[55] ^ k[58] ^ k[112] ^ k[119] ^ 1;
x[3] = x[10] ^ x[13] ^ x[20] ^ x[43] ^ x[67] ^ x[74] ^ x[77]*k[13] ^ x[77] ^ x[84]*k[20] ^ x[84] ^ x[107]*k[43] ^ x[107] ^ k[3] ^ k[10] ^ k[13] ^ k[20] ^ k[43] ^ k[67] ^ k[74] ^ k[84];
x[17] = x[10] ^ x[20] ^ x[50] ^ x[74] ^ x[81] ^ x[84]*k[20] ^ x[84] ^ x[91]*k[27] ^ x[114]*k[50] ^ x[114] ^ k[10] ^ k[17] ^ k[50] ^ k[74] ^ k[81] ^ k[91] ^ 1;

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


int main( int argc, char * argv[] )
{
    //ID = stoi( argv[1] );
    vector<int> S { 1, 8 };
    int pos = 26;

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

    if ( bias != 0 )
        cout << bias / KEY << " = " << log( abs( bias ) / KEY ) / log( 2 ) << endl;
    else
        cout << bias / KEY  << endl;
}

