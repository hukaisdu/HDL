#include"XoodooHATF.h"
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include"polynomial.h"
#include"quadratic.h"
#include<cassert>
#include<string>
#include"HATF.h"

using namespace std;

void getP( Polynomial *** A, int i, Polynomial ** B )
{
    for ( int x = 0; x < 4; x++ )
        for ( int z = 0; z < 32; z++ )
            B[x][z] = A[x][i][z];
}

void setP( Polynomial *** A, int i, Polynomial ** B )
{
    for ( int x = 0; x < 4; x++ )
        for ( int z = 0; z < 32; z++ )
            A[x][i][z] = B[x][z]; 
}

void Rotate( Polynomial ** A, int index1, int index2, Polynomial ** B )
{
    for ( int z = 0; z < 32; z++ )
        for ( int x = 0; x < 4; x++ )
            B[x][z] = A[ ( x + 4 - index1 ) % 4 ] [ ( z + 32 - index2 ) % 32 ];
}

void Add( Polynomial ** A, Polynomial ** B, Polynomial ** C )
{
    for ( int x = 0; x < 4; x++ )
        for ( int z = 0; z < 32; z++ )
            C[x][z] = A[x][z] + B[x][z]; 
}

Polynomial** newLane()
{
    Polynomial ** A0 = new Polynomial* [4];
    for ( int i = 0; i < 4; i++ )
        A0[i] = new Polynomial[32];
    return A0;
}

void freeLane( Polynomial ** A )
{
    for ( int i = 0; i < 4; i++ )
        delete [] A[i];
    delete [] A;
}

void theta( Polynomial*** A )
{
    // get 0
    Polynomial ** A0 = newLane();
    Polynomial ** A1 = newLane();
    Polynomial ** A2 = newLane();
    Polynomial ** P = newLane();

    getP( A, 0, A0 );
    getP( A, 1, A1 );
    getP( A, 2, A2 );

    Add( P, A0, P ); 
    Add( P, A1, P ); 
    Add( P, A2, P );

    auto P15 = newLane();
    auto P114 = newLane();

    Rotate( P, 1, 5, P15 ); Rotate( P, 1, 14, P114 );

    auto E = newLane();

    Add( P15, P114, E );

    Add( A0, E, A0 );
    setP( A, 0, A0 ); 

    Add( A1, E, A1 );
    setP( A, 1, A1 ); 

    Add( A2, E, A2 );
    setP( A, 2, A2 ); 

    freeLane( A0 );
    freeLane( A1 );
    freeLane( A2 );
    freeLane( P );
    freeLane( E );
    freeLane( P15 );
    freeLane( P114 );
}

void pwest( Polynomial *** A )
{
    auto A1 = newLane();
    auto A2 = newLane();

    auto A1r = newLane();
    auto A2r = newLane();

    getP( A, 1, A1 );
    getP( A, 2, A2 );

    Rotate( A1, 1, 0,  A1r );
    Rotate( A2, 0, 11, A2r );
    
    setP( A, 1, A1r );  
    setP( A, 2, A2r );  

    freeLane( A1 );
    freeLane( A2 );
    freeLane( A1r );
    freeLane( A2r );
}

void iota( Polynomial***A, unsigned int C0 )
{
    int L[32] = { 0 };

    for ( int i = 0; i < 32; i++ )
        L[i] = C0 >> ( 31 - i ) & 0x1;

    for ( int z = 0; z < 32; z++ ) 
    {
        if ( L[z] == 1 )
            A[0][0][z] = A[0][0][z] + Polynomial( "1" );
        else
            A[0][0][z] = A[0][0][z] + Polynomial( "0" );
    }
}


void chi( Polynomial *** A )
{
    Polynomial *** B = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        B[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            B[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
        for (  int z = 0; z < 32; z++ )
        {
            B[x][0][z] = ( A[x][1][z] + Polynomial( "1" ) ) * A[x][2][z];
            B[x][1][z] = ( A[x][2][z] + Polynomial( "1" ) ) * A[x][0][z];
            B[x][2][z] = ( A[x][0][z] + Polynomial( "1" ) ) * A[x][1][z];
        }

    auto Ay = newLane();
    auto By = newLane();
    for ( int y = 0; y < 3; y++ )
    {
        getP( A, y, Ay );

        getP( B, y, By );

        Add( Ay, By, Ay );

        setP( A, y, Ay );
    }

    freeLane( Ay );
    freeLane( By );

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] B[i][j];

        delete [] B[i];
    }
    delete [] B;
}

void peast( Polynomial *** A )
{
    auto A1 = newLane();
    auto A2 = newLane();

    auto A1r = newLane();
    auto A2r = newLane();

    getP( A, 1, A1 );
    getP( A, 2, A2 );

    Rotate( A1, 0, 1,  A1r );
    Rotate( A2, 2, 8, A2r );
    
    setP( A, 1, A1r );  
    setP( A, 2, A2r );  

    freeLane( A1 );
    freeLane( A2 );
    freeLane( A1r );
    freeLane( A2r );
}

void RF( Polynomial *** A, unsigned int C )
{
    theta( A );
    pwest( A );
    iota( A, C );
    chi( A );
    peast( A );
}

void Enc( Polynomial *** A, int ROUND )
{
    unsigned int C[12] = { 0x58, 0x38, 0x3C0, 0xD0, 0x120, 0x14, 0x60, 0x2C, 0x380, 0xF0, 0x1A0, 0x12 };
    for ( int r = 0; r < ROUND - 1; r++ )
        RF( A, C[ r + 12 - ROUND] );
    chi( A );
}

void I2C( int index, int & x, int & y, int & z )
{
    z = index % 32;
    int tmp = ( index - z ) / 32;
    x = tmp % 4;
    y = ( tmp - x ) / 4;
}

int C2I( int x, int y, int z )
{
    return z + 32 * ( x + 4 * y );
}

XoodooHATF::XoodooHATF( int ORDER, int size, int ROUND, int level ):
    HATF ( ORDER, size, ROUND, level )
{
}

void XoodooHATF::initV( 
        unsigned long diff0, 
        unsigned long diff1, 
        unsigned long diff2, 
        unsigned long diff3, 
        unsigned long diff4, 
        unsigned long diff5
        )
{
    for ( int x = 0; x < 4; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            if ( y == 0 )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "k", C2I( x, y, z ) );
            else
                for ( int z = 0; z < 32; z++ )
                {
                    _S[ C2I(x, y, z) ] = createVar( "u", C2I( x, y - 1, z ) );
                }
        }
    }

    // set the difference
    //for ( size_t index = 0; index < D.size(); index++ )
    for ( int i = 0; i < 64; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff0 >> ( 63 - i ) & 0x1 )
        {
            _S[ C2I(x, 0, z) ] = _S[ C2I(x, 0, z) ] + _x[ 1];
        }
    }

    for ( int i = 64; i < 128; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff1 >> ( 63 - (i-64) ) & 0x1 )
        {
            _S[ C2I(x, 0, z) ] = _S[ C2I(x, 0, z) ] + _x[1];
        }
    }

    for ( int i = 0; i < 64; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff2 >> ( 63 - i ) & 0x1 )
        {
            _S[ C2I(x, 1, z) ] = _S[ C2I(x, 1, z) ] + _x[ 1];
        }
    }

    for ( int i = 64; i < 128; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff3 >> ( 63 - (i-64) ) & 0x1 )
        {
            _S[ C2I(x, 1, z) ] = _S[ C2I(x, 1, z) ] + _x[1];
        }
    }

    for ( int i = 0; i < 64; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff4 >> ( 63 - i ) & 0x1 )
        {
            _S[ C2I(x, 2, z) ] = _S[ C2I(x, 2, z) ] + _x[ 1];
        }
    }

    for ( int i = 64; i < 128; i++ )
    {
        auto x = i / 32;
        auto z = i % 32;
        
        if ( diff5 >> ( 63 - (i-64) ) & 0x1 )
        {
            _S[ C2I(x, 2, z) ] = _S[ C2I(x, 2, z) ] + _x[1];
        }
    }

    /*
    for ( int i = 0; i < 384; i++ )
        cout << _S[i] << endl;
    */

    for ( int x = 0; x < 4; x++ )
        for ( int y = 0; y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
            {
        //_BM[ getNewVar( 0, C2I(0, 0, i ), 0 ).getPoly() ] = 0;
                _BM[ createVar( "k", C2I(x, y, z) ).getPoly() ] = 0;
                _BM[ createVar( "u", C2I(x, y, z) ).getPoly() ] = 0;
            }
}

void XoodooHATF::initWithString(string diff[3])
{
    for ( int x = 0; x < 4; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            if ( y == 0 )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "k", C2I( x, y, z ) );
            else
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "u", C2I( x, y - 1, z ) );
        }
    }

    for ( int i = 0; i < 128; i++ )
    {
        int x = i / 32;
        int z = i % 32;

        if ( diff[0][i] == '1' )
           _S[ C2I(x, 0, z) ] = _S[ C2I(x, 0, z) ] + _x[1];
    }

    for ( int i = 0; i < 128; i++ )
    {
        int x = i / 32;
        int z = i % 32;

        if ( diff[1][i] == '1' )
            _S[ C2I(x, 1, z) ] =_S[ C2I(x, 1, z) ] + _x[1];
    }

    for ( int i = 0; i < 128; i++ )
    {
        int x = i / 32;
        int z = i % 32;

        if ( diff[2][i] == '1' )
            _S[ C2I(x, 2, z) ] = _S[ C2I(x, 2, z) ] + _x[1];
    }

    for ( int x = 0; x < 4; x++ )
        for ( int y = 0; y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
            {
                _BM[ createVar( "k", C2I(x, y, z) ).getPoly() ] = 0;
                _BM[ createVar( "u", C2I(x, y, z) ).getPoly() ] = 0;
            }
}

void XoodooHATF::initKey( const vector<int> & D, int KEY )
{
    for ( int x = 0; x < 4; x++ )
    {
        for ( int y = 0; y < 3; y++ )
        {
            if ( y == 0 )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "k", C2I( x, y, z ) );
            else
                for ( int z = 0; z < 32; z++ )
                {
                    _S[ C2I(x, y, z) ] = createVar( "u", C2I( x, y - 1, z ) );
                }
        }
    }
//K(11) + K(102) + K(125) + X(102) + X(125) + X(230) + X(253)
//K(70) + K(93) + X(70) + X(93) + X(107) + X(198) + X(221) + 1
//K(18) + K(27) + K(32) + X(18) + X(27) + X(146) + X(155) + 1
//K(7) + K(16) + X(7) + X(16) + X(135) + X(144) + X(181)
    /*
    vector<int> KEY { 11, 102, 125, 70, 93, 18, 27, 32, 7, 16 };
    vector<int> N { 102, 125, 230, 253, 70, 93, 107, 198, 221, 18, 27, 146, 155, 7, 16, 135, 144, 181 };
    
    for ( int i = 0; i < 384; i++ )
    {
        for ( auto it : KEY )
        {
            if ( createVar( "k", it ) == _S[i] )
            {
                if ( it == 70 )
                    _S[i] = createVar( "0" );
                else if ( it == 18 )
                    _S[i] = createVar( "1" );
                else
                    _S[i] = createVar( "0" );
            }
        }
        for ( auto it : N )
        {
            if ( createVar( "u", it ) == _S[i] )
                _S[i] = createVar( "0" );
        }
    }
    */

    // set the difference
    for ( size_t index = 0; index < D.size(); index++ )
    {
        auto x = D[index] / 32;
        auto z = D[index] % 32;

        if ( KEY >= 0 ) 
            _S[ C2I(x, 0, z) ] =  createVar( to_string( KEY >> ( _ORDER - 1 - index ) & 0x1 ) );
        
        _S[ C2I(x, 1, z) ] = _x[ 1 << index ];
        _S[ C2I(x, 2, z) ] = _x[ 1 << index ];
    }

    /*
    for ( int i = 0; i < 384; i++ )
        cout << _S[i] << endl;
    */

    for ( int x = 0; x < 4; x++ )
        for ( int y = 0; y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
            {
        //_BM[ getNewVar( 0, C2I(0, 0, i ), 0 ).getPoly() ] = 0;
                _BM[ createVar( "k", C2I(x, y, z) ).getPoly() ] = 0;
                _BM[ createVar( "u", C2I(x, y, z) ).getPoly() ] = 0;
            }
}

void XoodooHATF::initVec(int UV[], int U[], int V[] )
{
    for ( int i = 0; i < 384; i++ )
        _S[i] = createVar( "u", i );

    for ( int i = 0; i < 384; i++ )
    {
        if ( UV[i] == 1 )
            _S[i] = _S[i] + _x[3];

        if ( U[i] == 1 )
            _S[i] = _S[i] + _x[1];

        if ( V[i] == 1 )
            _S[i] = _S[i] + _x[2];

    }

    for ( int i = 0; i < 384; i++ )
        _BM[ createVar( "u", i ).getPoly() ] = 0;

}

void XoodooHATF::initRelated( const vector<int> & D )
{
    for ( int y = 0; y < 3; y++ )
    {
        if ( y == 0 )
        {
            for ( int x = 0; x < 4; x++ )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "k", C2I(x, y, z ) ); 
        }
        else if ( y == 1 )
        {
            for ( int x = 0; x < 4; x++ )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "u", C2I( x, y - 1, z ) );
        }
        else
        {
            for ( int x = 0; x < 4; x++ )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = createVar( "u", C2I( x, y - 1, z ) );
        }
    }

    // set the difference
    for ( size_t index = 0; index < D.size(); index++ )
    {
        auto x = D[index] / 32;
        auto z = D[index] % 32;

        _S[ C2I(x, 0, z ) ] = _x[ 1 << index ];
        //_S[ C2I(x, 1, z ) ] = Polynomial( "0" );
        _S[ C2I(x, 2, z ) ] = _x[ 1 << index ];
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void XoodooHATF::initDim( int ** U )
{
    for ( int i = 0; i < _SIZE; i++ )
    {
        _S[i] = createVar( "0" );
    }

    for ( size_t i= 0; i < _SIZE; i++ )
    {
        for ( int j = 0; j < ( 1 << _ORDER ); j++ )
            if ( U[j][i] == 2 )
            {
                _S[i] = _S[i] + createVar( "u", i ) * _x[j];
            }

            else if ( U[j][i] == 1 )
                _S[i] = _S[i] + _x[j];

            else
                _S[i] = _S[i] + Polynomial( "0" );
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
    }
}

void XoodooHATF::initUVW(int UVW[], int UV[], int UW[], int VW[], int U[], int V[], int W[] )
{
    for ( int i = 0; i < 384; i++ )
        _S[i] = createVar( "u", i );

    for ( int i = 0; i < 384; i++ )
    {
        if ( UVW[i] == 1 )
            _S[i] = _S[i] + _x[7];

        if ( UV[i] == 1 )
            _S[i] = _S[i] + _x[6];

        if ( UW[i] == 1 )
            _S[i] = _S[i] + _x[5];

        if ( VW[i] == 1 )
            _S[i] = _S[i] + _x[3];

        if ( U[i] == 1 )
            _S[i] = _S[i] + _x[4];

        if ( V[i] == 1 )
            _S[i] = _S[i] + _x[2];

        if ( V[i] == 1 )
            _S[i] = _S[i] + _x[1];

    }

    for ( int i = 0; i < 384; i++ )
        _BM[ createVar( "u", i ).getPoly() ] = 0;

}

void XoodooHATF::initFromMiddle( vector<int> & D, int v, bool flag )
{
    for ( int x = 0; x < 4; x++ )
        for ( int y = 0;y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
                _S[ C2I(x, y, z) ] = createVar( "u", C2I(x,y,z) );

    for ( size_t index = 0; index < D.size(); index++ )
    {
        auto x = D[index] / 32;
        auto z = D[index] % 32;

        for ( int i = 0; i < 3; i++ )
            if ( v >> ( 2 - i ) & 0x1 )
            {
                if ( flag )
                    _S[ C2I(x, i, z) ] = _x[ 1 << index ] + _S[ C2I(x, i, z ) ];
                else
                    _S[ C2I(x, i, z) ] = _x[ 1 << index ];
            }
            else
            {
                _S[ C2I(x, i, z) ] = createVar( "0" );
            }
    }

    for ( int x = 0; x < 4; x++ )
        for ( int y = 0; y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
                _BM[ createVar( "u", C2I(x, y, z) ).getPoly() ] = 0;
}



void XoodooHATF::init( const vector<int> & D )
{
    for ( int x = 0; x < 4; x++ )
    {
        if ( x == 0 )
        {
            for ( int y = 0; y < 3; y++ )
            {
                if ( y == 1 )
                {
                    for ( int z = 0; z < 32; z++ )
                        _S[ C2I(x, y, z) ] = Polynomial( "0" );
                }
                else
                {
                    for ( int z = 0; z < 32; z++ )
                    {
                        _S[ C2I(x, y, z) ] = createVar( "u", z + 32 * x );

                        //cout << _S[ C2I( x, y, z ) ] << endl;
                    }
                }
            }
        }
        else
        {
            for ( int y = 0; y < 3; y++ )
                for ( int z = 0; z < 32; z++ )
                    _S[ C2I(x, y, z) ] = Polynomial( "0" );
        }
    }
    // set the difference
    for ( size_t index = 0; index < D.size(); index++ )
    {
        _S[ C2I(0, 0, D[index]) ] = _x[ 1 << index ];
        _S[ C2I(0, 1, D[index]) ] = Polynomial( "0" );
        _S[ C2I(0, 2, D[index]) ] = _x[ 1 << index ];
    }

    /*
    for ( int i = 0; i < 384; i++ )
        cout << _S[i] << endl;
    */

    for ( int i = 0; i < 32; i++ )
        _BM[ createVar( "u", i ).getPoly() ] = 0;
}

void XoodooHATF::RunRoundLast(int R, int ROUND ) 
{
    static unsigned int C[12] = { 0x58, 0x38, 0x3C0, 0xD0, 0x120, 0x14, 0x60, 0x2C, 0x380, 0xF0, 0x1A0, 0x12 };

    Polynomial *** A = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        A[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            A[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        A[x][y][z] = _S[ C2I( x, y, z ) ];

    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    //                               ROUND - 2,    ROUND - 1
    // R + index = ROUND + 10 
    // index = ROUND + 10 - R
    //RF( A, C[ R + 12 - ROUND] );
    theta( A );
    pwest( A );
    //iota( A, C[ R + 12 - ROUND] );
    //chi( A );
    //peast( A );

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        //A[x][y][z] = _S[ C2I( x, y, z ) ];
        _S[ C2I( x, y, z ) ] = A[x][y][z];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] A[i][j];

        delete [] A[i];
    }
    delete [] A;
}

void XoodooHATF::RunChiP(int R, int ROUND ) 
{
    Polynomial *** A = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        A[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            A[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        A[x][y][z] = _S[ C2I( x, y, z ) ];

    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    //                               ROUND - 2,    ROUND - 1
    // R + index = ROUND + 10 
    // index = ROUND + 10 - R
    chi( A );
    peast( A );

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        //A[x][y][z] = _S[ C2I( x, y, z ) ];
        _S[ C2I( x, y, z ) ] = A[x][y][z];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] A[i][j];

        delete [] A[i];
    }
    delete [] A;
}

void XoodooHATF::RunRound(int R, int ROUND ) 
{
    static unsigned int C[12] = { 0x58, 0x38, 0x3C0, 0xD0, 0x120, 0x14, 0x60, 0x2C, 0x380, 0xF0, 0x1A0, 0x12 };

    Polynomial *** A = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        A[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            A[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        A[x][y][z] = _S[ C2I( x, y, z ) ];

    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    //                               ROUND - 2,    ROUND - 1
    // R + index = ROUND + 10 
    // index = ROUND + 10 - R
    RF( A, C[ R + 12 - ROUND] );

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        //A[x][y][z] = _S[ C2I( x, y, z ) ];
        _S[ C2I( x, y, z ) ] = A[x][y][z];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] A[i][j];

        delete [] A[i];
    }
    delete [] A;
}

void XoodooHATF::RunChi() 
{
    Polynomial *** A = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        A[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            A[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        A[x][y][z] = _S[ C2I( x, y, z ) ];

    chi( A );

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        //A[x][y][z] = _S[ C2I( x, y, z ) ];
        _S[ C2I( x, y, z ) ] = A[x][y][z];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] A[i][j];

        delete [] A[i];
    }
    delete [] A;
}

void XoodooHATF::setCondition( vector<int> & D, vector<int> & V )
{
    for ( int i = 0; i < D.size(); i++ )
    {
        if ( V[i] == 0 )
        _S[D[i]] = Polynomial( "0" ) ;
        else
        _S[D[i]] = Polynomial( "1" ) ;
    }
}

void XoodooHATF::RunPE() 
{
    Polynomial *** A = new Polynomial ** [4];
    for ( int i = 0; i < 4; i++ )
    {
        A[i] = new Polynomial * [ 3 ];
        for ( int j = 0; j < 3; j++ )
            A[i][j] = new Polynomial [32];
    }

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        A[x][y][z] = _S[ C2I( x, y, z ) ];

    peast( A );

    for ( int x = 0; x < 4; x++ )
    for ( int y = 0; y < 3; y++ )
    for ( int z = 0; z < 32; z++ )
        //A[x][y][z] = _S[ C2I( x, y, z ) ];
        _S[ C2I( x, y, z ) ] = A[x][y][z];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 3; j++ )
            delete [] A[i][j];

        delete [] A[i];
    }
    delete [] A;
}

void XoodooHATF::resetBM()
{
    _BM.clear();
    for ( int x = 0; x < 4; x++ )
        for ( int y = 0; y < 3; y++ )
            for ( int z = 0; z < 32; z++ )
            {
        //_BM[ getNewVar( 0, C2I(0, 0, i ), 0 ).getPoly() ] = 0;
                _BM[ createVar( "k", C2I(x, y, z) ).getPoly() ] = 0;
                _BM[ createVar( "u", C2I(x, y, z) ).getPoly() ] = 0;
            }
}


// for the related key
map<string, string> XoodooHATF::getValueMapV2( int value )
{
    map<string, string> MAP;

    if ( value >> ( 7 - 0 ) & 0x1 )
        MAP.insert( make_pair( string( "k8" ), string( "k31+k45+u8+u31+u136+u159" ) ) );
    else
        MAP.insert( make_pair( string( "k8" ), string( "k31+k45+u8+u31+u136+u159+1" ) ) );
    if ( value >> ( 7 - 1 ) & 0x1 )
        MAP.insert( make_pair( string( "k7" ), string( "k16+u7+u16+u135+u144+u181" ) ) );
    else
        MAP.insert( make_pair( string( "k7" ), string( "k16+u7+u16+u135+u144+u181+1" ) ) );
    if ( value >> ( 7 - 2 ) & 0x1 )
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221" ) ) );
    else
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221+1" ) ) );
    if ( value >> ( 7 - 3 ) & 0x1 )
        MAP.insert( make_pair( string( "k18" ), string( "k27+k32+u18+u27+u146+u155" ) ) );
    else
        MAP.insert( make_pair( string( "k18" ), string( "k27+k32+u18+u27+u146+u155+1" ) ) );
    if ( value >> ( 7 - 4 ) & 0x1 )
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253" ) ) );
    else
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253+1" ) ) );
    if ( value >> ( 7 - 5 ) & 0x1 )
        MAP.insert( make_pair( string( "k52" ), string( "k61+k66+u52+u61+u180+u189" ) ) );
    else
        MAP.insert( make_pair( string( "k52" ), string( "k61+k66+u52+u61+u180+u189+1" ) ) );
    if ( value >> ( 7 - 6 ) & 0x1 )
        MAP.insert( make_pair( string( "k104" ), string( "k127+u13+u104+u127+u232+u255" ) ) );
    else
        MAP.insert( make_pair( string( "k104" ), string( "k127+u13+u104+u127+u232+u255+1" ) ) );
    if ( value >> ( 7 - 7 ) & 0x1 )
        MAP.insert( make_pair( string( "k41" ), string( "k50+u41+u50+u169+u178+u215" ) ) );
    else
        MAP.insert( make_pair( string( "k41" ), string( "k50+u41+u50+u169+u178+u215+1" ) ) );

    return MAP;
}

map<string, string> XoodooHATF::getValueMapV1( int value )
{
    map<string, string> MAP;

    if ( value >> ( 3 - 0 ) & 0x1 )
        MAP.insert( make_pair( string( "k103" ), string( "k112+u103+u112+u149+u231+u240" ) ) );
    else
        MAP.insert( make_pair( string( "k103" ), string( "k112+u103+u112+u149+u231+u240+1" ) ) );
    if ( value >> ( 3 - 1 ) & 0x1 )
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221" ) ) );
    else
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221+1" ) ) );
    if ( value >> ( 3 - 2 ) & 0x1 )
        MAP.insert( make_pair( string( "k82" ), string( "k91+u82+u91+u96+u210+u219" ) ) );
    else
        MAP.insert( make_pair( string( "k82" ), string( "k91+u82+u91+u96+u210+u219+1" ) ) );
    if ( value >> ( 3 - 3 ) & 0x1 )
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253" ) ) );
    else
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253+1" ) ) );

    return MAP;
}

// for the normal case
map<string, string> XoodooHATF::getValueMapV0( int value )
{
    map<string, string> MAP;

    if ( value >> ( 3 - 0 ) & 0x1 )
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253+1" ) ) );
    else
        MAP.insert( make_pair( string( "k11" ), string( "k102+k125+u102+u125+u230+u253" ) ) );

    if ( value >> ( 3 - 1 ) & 0x1 )
        MAP.insert( make_pair( string( "k18" ), string( "k27+k32+u18+u27+u146+u155+1" ) ) );
    else
        MAP.insert( make_pair( string( "k18" ), string( "k27+k32+u18+u27+u146+u155" )  ) );

    if ( value >> ( 3 - 2 ) & 0x1 )
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221+1" )  ) );
    else
        MAP.insert( make_pair( string( "k70" ), string( "k93+u70+u93+u107+u198+u221" )  ) );

    if ( value >> ( 3 - 3 ) & 0x1 )
        MAP.insert( make_pair( string( "k7" ), string( "k16+u7+u16+u135+u144+u181" )  ) );
    else
        MAP.insert( make_pair( string( "k7" ), string( "k16+u7+u16+u135+u144+u181+1" ) ) );

    return MAP;
}

map<string, string> XoodooHATF::getValueMapV3(  )
{
    map<string, string> MAP;

    MAP.insert( make_pair( string( "u7" ), string( "k16+k7+u16+u135+u144+u181" ) ) );
    MAP.insert( make_pair( string( "u70" ), string( "k93+k70+u93+u107+u198+u221+1" ) ) );
    MAP.insert( make_pair( string( "u5" ), string( "k14+k5+u14+u51+u133+u142+1" ) ) );
    MAP.insert( make_pair( string( "u67" ), string( "k90+k104+k67+u90+u195+u218+1" ) ) );
    MAP.insert( make_pair( string( "u18" ), string( "k27+k32+k18+u27+u146+u155+1" ) ) );
    MAP.insert( make_pair( string( "u102" ), string( "k102+k125+k11+u125+u230+u253" ) ) );
    MAP.insert( make_pair( string( "u37" ), string( "k46+k83+k37+u46+u165+u174" ) ) );
    MAP.insert( make_pair( string( "u79" ), string( "k88+k79+u88+u207+u216+u253" ) ) );

    return MAP;
}


map<string, string> XoodooHATF::getValueMapV4()
{
    map<string, string> MAP;

    MAP.insert( make_pair( string( "u207" ), string( "k79+k93+u79+u93+u107+u116+u221" ) ) );
    MAP.insert( make_pair( string( "u198" ), string( "k70+k93+u70+u93+u107+u221+1" ) ) );
    MAP.insert( make_pair( string( "u181" ), string( "k7+k16+u7+u16+u135+u144" ) ) );
    MAP.insert( make_pair( string( "u132" ), string( "k4+k27+k41+u4+u27+u155+1" ) ) );
    MAP.insert( make_pair( string( "u191" ), string( "k54+k63+k68+u54+u63+u182+1" ) ) );
    MAP.insert( make_pair( string( "u234" ), string( "k97+k106+u15+u97+u106+u225+1" ) ) );
    MAP.insert( make_pair( string( "u217" ), string( "k43+k52+u43+u52+u171+u180" ) ) );
    MAP.insert( make_pair( string( "u190" ), string( "k16+k25+u16+u25+u144+u153" ) ) );
    MAP.insert( make_pair( string( "u239" ), string( "k20+k102+k111+u102+u111+u230" ) ) );
    MAP.insert( make_pair( string( "u146" ), string( "k18+k27+k32+u18+u27+u155+1" ) ) );
    MAP.insert( make_pair( string( "u253" ), string( "k11+k102+k125+u102+u125+u230" ) ) );
    MAP.insert( make_pair( string( "u138" ), string( "k1+k10+k47+u1+u10+u129" ) ) );

    return MAP;
}



