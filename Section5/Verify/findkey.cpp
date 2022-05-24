#include<iostream>
#include<random>
#include<vector>
#include<cstdio>

using namespace std;

int OFF0 = 7;
int OFF1 = 14;
int OFF2 = 51;
int OFF3 = 57;

int next( int i )
{
    return ( 0 + i * 7 ) % 64;
}

int cal( const vector<int> & A, int length )
{
    if ( ( A[0] == 1 ) && ( A[length - 1] == 1 )  )
    {
        int i = 0;
        while ( A[i] == 1 )
            i++;

        int t = 0;
        for ( ; ;  )
        {
            if ( A[i] == 1 ) // find 1
            {
                int c = 0;
                for ( int j = i; ; j++ )
                {
                    c ++;
                    if ( A[ ( j + 1 ) % length ] == 0 )
                        break;
                }

                t += c - 1;

                i = ( i + c );

            }
            else
                i++;

            if ( i > length )
                break;
        }

        return t;

    }
    else
    {
        int t = 0;
        for ( int i = 0; ;  )
        {
            if ( A[i] == 1 ) // find 1
            {
                int c = 0;
                for ( int j = i; ; j++ )
                {
                    c ++;
                    if ( A[ ( j + 1 ) % length ] == 0 )
                        break;
                }

                t += c - 1;

                i = ( i + c );

            }
            else
                i++;

            if ( i > length )
                break;
        }

        return t;
    }
}

int LEN = 64;

int main()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> disint( 0, 1 );


    int s = 0;
    for ( int test = 0; test < ( 1 << 20 ); test++ )
    {
        vector<int> vecx ( LEN, 0 );

        vector<int> vec;

        for ( int i = 0; i < 256; i++ )
            vec.push_back( disint( gen )  );

        int ss = 0;
        
        for ( int i = 0; i < LEN; i++ )
        {
            int a = next(i);
            int b = next( i + 1 );
            //cout << i << " " << ( 64 + i ) << " " << ( i + OFF0 ) % 64 << " " << 64 + ( i + OFF0 ) % 64 << endl;
            //cout << a << " " << b << endl;
            if ( ( vec[a] | vec[ a + 64 ] | vec[b] | vec[b + 64] ) == 0 )
            {
                ss += 1;
                vecx[i] = 1;
                vecx[i + 1] = 1;
            }

            
            //if ( ( vec[i] | vec[ ( i + OFF1 ) % 64 ] | vec[i + 64] | vec[64 + ( i + OFF1 ) % 64] ) == 0 )
            //    ss += 1;
            //if ( ( vec[i] | vec[ ( i + OFF2 ) % 64 ] | vec[i + 64] | vec[64 + ( i + OFF2 ) % 64] ) == 0 )
            //    ss += 1;
            //if ( ( vec[i] | vec[ ( i + OFF3 ) % 64 ] | vec[i + 64] | vec[64 + ( i + OFF3 ) % 64] ) == 0 )
            //    ss += 1;
        }
        //cout << ss << endl;
       //for ( int i = 0; i < 64; i++ )
       //     cout << vecx[i] << ' ';
       // cout << endl;
       // getchar();

        //s += ss;
        //cout << ss << endl;
        //for ( int i = 0; i < 64; i++ )
        //    printf( "%2d ", i );
        //cout << endl;
        //for ( int i = 0; i < 2; i++ )
       // {
       //     for ( int j = 0; j < 64; j++ )
       //         printf( "%2d ", vec[ 64 * i + j ] );
       ///     cout << endl;
       // }
       s += cal( vecx, LEN );

        //getchar();
    }

    cout << 1.0 * s / ( 1 << 20 ) << endl;
}
