#include"AsconHATF.h"
#include"split.h" 
#include"ascon.h"
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include"polynomial.h"
#include"quadratic.h"
#include<cassert>
#include<string>
#include"HATF.h"

#define DEBUG 0

using namespace std;

mutex workmutex;
mutex smutex;

extern int THREADNUM;

void SingleSboxM( vector<Polynomial> & X  )
{
    auto y0 = X[0];
    auto y1 = X[1];
    auto y2 = X[2];
    auto y3 = X[3];
    auto y4 = X[4];

    X[0] = y4*y1 + y3 + y2*y1 + y2 + y1*y0 + y1 + y0;
    X[1] = y4 + y3*y2 + y3*y1 + y3 + y2*y1 + y2 + y1 + y0;
    X[2] = y4*y3 + y4 + y2 + y1 + Polynomial( "1" );
    X[3] = y4*y0 + y4 + y3*y0 + y3 + y2 + y1 + y0;
    X[4] = y4*y1 + y4 + y3 + y1*y0 + y1;
}

void SingleSboxPN ( vector<Polynomial> & X )
{
    auto t0 = X[0];
    auto t4 = X[4];
    auto t3 = X[3];
    auto t1 = X[1];
    auto t2 = X[2];
    X[0] = t0 + (( Polynomial( "1" ) + t1) * t2);
    X[2] = t2 + (( Polynomial( "1" ) + t3) * t4);
    X[4] = t4 + (( Polynomial( "1" ) + t0) * t1);
    X[1] = t1 + (( Polynomial( "1" ) + t2) * t3);
    X[3] = t3 + (( Polynomial( "1" ) + t4) * t0);
    X[1] = X[1] + X[0];
    X[3] = X[3] + X[2];
    X[0] = X[0] + X[4];
    X[2] = Polynomial( "1" ) + X[2];
}

void SingleSboxPNLast ( vector<Polynomial> & X )
{
    auto t0 = X[0];
    auto t4 = X[4];
    auto t3 = X[3];
    auto t1 = X[1];
    auto t2 = X[2];
    X[0] = t0 + (( Polynomial( "1" ) + t1) * t2);
    //
    //X[2] = t2 + (( Polynomial( "1" ) + t3) * t4);
    //
    //X[4] = t4 + (( Polynomial( "1" ) + t0) * t1);
    X[4] = t4 + (( Polynomial( "1" ) + t0) * t1);

    //X[1] = t1 + (( Polynomial( "1" ) + t2) * t3);
    //X[3] = t3 + (( Polynomial( "1" ) + t4) * t0);
    //X[1] = X[1] + X[0];
    //X[3] = X[3] + X[2];
   
    X[0] = X[0] + X[4];
    
    //X[2] = Polynomial( "1" ) + X[2];
}

void SingleSboxPL ( vector<Polynomial> & X )
{
    X[0] = X[0] + X[4];
    X[4] = X[4] + X[3];
    X[2] = X[2] + X[1];
}

vector<Polynomial> SboxPL( const vector<Polynomial> & X ) 
{
    vector<Polynomial> V(320);

    vector<Polynomial> * Single = new vector<Polynomial> [64];


    int STEP = 64 / THREADNUM;

    for ( int step = 0; step < STEP; step ++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;

        for ( int col = start; col < end; col++ )
        {
            for ( int row = 0; row < 5; row++ )
                Single[col].push_back( X[col + 64 * row] );

            threads.push_back( thread( SingleSboxPL, ref( Single[col] ) ) );
        }

        for ( auto & th : threads )
            th.join();
    }

    for ( int col = 0; col < 64; col++ )
    {
        for ( int j = 0; j < 5; j++ )
            V[ col + 64 * j ] = Single[col][j];
    }

    delete [] Single;

    return V;
}

vector<Polynomial> SboxPN( const vector<Polynomial> & X ) 
{
    vector<Polynomial> V(320);

    vector<Polynomial> * Single = new vector<Polynomial> [64];


    int STEP = 64 / THREADNUM;

    for ( int step = 0; step < STEP; step ++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;

        for ( int col = start; col < end; col++ )
        {
            for ( int row = 0; row < 5; row++ )
                Single[col].push_back( X[col + 64 * row] );

            threads.push_back( thread( SingleSboxPN, ref( Single[col] ) ) );
        }

        for ( auto & th : threads )
            th.join();
    }

    for ( int col = 0; col < 64; col++ )
    {
        for ( int j = 0; j < 5; j++ )
            V[ col + 64 * j ] = Single[col][j];
    }

    delete [] Single;

    return V;
}

vector<Polynomial> SboxPNLast( const vector<Polynomial> & X ) 
{
    vector<Polynomial> V(320);

    vector<Polynomial> * Single = new vector<Polynomial> [64];


    int STEP = 64 / THREADNUM;

    for ( int step = 0; step < STEP; step ++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;

        for ( int col = start; col < end; col++ )
        {
            for ( int row = 0; row < 5; row++ )
                Single[col].push_back( X[col + 64 * row] );

            threads.push_back( thread( SingleSboxPNLast, ref( Single[col] ) ) );
        }

        for ( auto & th : threads )
            th.join();
    }

    for ( int col = 0; col < 64; col++ )
    {
        for ( int j = 0; j < 5; j++ )
            V[ col + 64 * j ] = Single[col][j];
    }

    delete [] Single;

    return V;
}

vector<Polynomial> SingleSbox( const vector<Polynomial> & X  )
{
    auto y0 = X[0];
    auto y1 = X[1];
    auto y2 = X[2];
    auto y3 = X[3];
    auto y4 = X[4];

    auto x0 = y4*y1 + y3 + y2*y1 + y2 + y1*y0 + y1 + y0;
    //auto x0 = ( y4 + y2 + y0 + Polynomial( "1" ) ) * y1 + y3 + y2 + y0;

    auto x1 = y4 + y3*y2 + y3*y1 + y3 + y2*y1 + y2 + y1 + y0;
    auto x2 = y4*y3 + y4 + y2 + y1 + Polynomial( "1" );
    auto x3 = y4*y0 + y4 + y3*y0 + y3 + y2 + y1 + y0;
    auto x4 = y4*y1 + y4 + y3 + y1*y0 + y1;

    vector<Polynomial> res;
    res.push_back( x0 );
    res.push_back( x1 );
    res.push_back( x2 );
    res.push_back( x3 );
    res.push_back( x4 );

    return res;
}

vector<Polynomial> Sbox( const vector<Polynomial> & X ) 
{
    vector<Polynomial> V(320);
    for ( int col = 0; col < 64; col++ )
    {
        vector<Polynomial> Single;
        for ( int row = 0; row < 5; row++ )
            Single.push_back( X[col + 64 * row] );
        auto temp = SingleSbox( Single );
        for ( int row = 0; row < 5; row++ )
            V[ col + 64 * row ] = temp[row];
    }
    return V;
}

vector<Polynomial> SboxM( const vector<Polynomial> & X ) 
{
    vector<Polynomial> V(320);

    vector<Polynomial> * Single = new vector<Polynomial> [64];


    int STEP = 64 / THREADNUM;

    for ( int step = 0; step < STEP; step ++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;

        for ( int col = start; col < end; col++ )
        {
            for ( int row = 0; row < 5; row++ )
                Single[col].push_back( X[col + 64 * row] );

            threads.push_back( thread( SingleSboxM, ref( Single[col] ) ) );
        }

        for ( auto & th : threads )
            th.join();
    }

    for ( int col = 0; col < 64; col++ )
    {
        for ( int j = 0; j < 5; j++ )
            V[ col + 64 * j ] = Single[col][j];
    }

    delete [] Single;

    return V;
}

vector<Polynomial> SingleMatrix( vector<Polynomial> & X, int r0, int r1 )
{
    vector<Polynomial> Y(64);
    for ( int i = 0; i < 64; i++ )
        Y[i] = X[i] + X[ ( i + (64 - r0) ) % 64 ] + X[ ( i + (64 - r1) ) % 64 ];
    return Y;
}

vector<Polynomial> Matrix( const vector<Polynomial> & X )
{
    vector<Polynomial> Y( 320 );

    int rot[5][2] = { { 19, 28 }, {61, 39}, {1, 6}, {10, 17}, {7, 41} };

    for ( int row = 0; row < 5; row++ )
    {
        vector<Polynomial> V(64);
        for ( int col = 0; col < 64; col++ )
        {
            V[col] = X[ 64 * row + col ];
        }

        auto temp = SingleMatrix( V, rot[row][0], rot[row][1] );

        for ( int col = 0; col < 64; col++ )
            Y[col + 64 * row] = temp[col]; 
    }

    return Y;
}

vector<Polynomial> addConst( vector<Polynomial> & X, int r )
{
    vector<Polynomial> Y;
    for ( int i = 0; i < 320; i++ )
        Y.push_back( X[i] );

    int constant[] = { 0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69,
            0x5a, 0x4b };

    int base = 184;

    for ( int i = 0; i < 8; i++ )
        if ( constant[r] >> ( 7 - i ) & 0x1 )
            Y[ base + i ] = Y[base + i] + Polynomial( "1" );

    return Y;
}





AsconHATF::AsconHATF( int ORDER, int size, int rd, int level ):
    HATF( ORDER, size, rd, level )
{
}

void AsconHATF::initUV( int ** U, int rr )
{
    for ( int i = 0; i < 320; i++ )
    {
        _S[i] = createVar( string( "u" ), i );
    }

    for ( size_t i= 0; i < 320; i++ )
    {
        for ( int j = 1; j < ( 1 << _ORDER ); j++ )
        {
            if ( U[j][i] == 1 )
                _S[i] = _S[i] + _x[j];
        }
    }

    for ( int i = 0; i < 320; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::initValue( vector<int> & D, int value  )
{
    unsigned long long IV = 0x80400c0600000000UL;

    for ( int i = 0; i < 64; i++ )
    {
        if ( ( IV >> ( 63 - i ) & 0x1 ) == 1 ) 
            _S[i] = Polynomial( "1" ); 
        else
            _S[i] = Polynomial( "0" ); 
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[64 + i] = createVar( string( "k" ), i );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[128 + i] = createVar( string( "k" ), i + 64 );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[192 + i] = createVar( string( "u" ), i  );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[256 + i] = createVar( string( "u" ), i + 64 );
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        _S[ 192 + D[index] ] = _x[ 1 << index ];
        _S[ 256 + D[index] ] = _x[ 1 << index ];

        int * p = new int[ 2 * _ORDER ];
        for ( int i = 0; i < ( 2 * _ORDER ); i++ )
            p[i] = value >> ( 2 * _ORDER - 1 - i ) & 0x1;

        _S[ 64 + D[index] ] = createVar( to_string( p[2 * index] ) );
        _S[ 128 + D[index] ] = createVar( to_string( p[ 2 * index + 1 ] ) );

        delete [] p;
    }

    for ( int i = 0; i < 128; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::initOne( vector<int> & D  )
{
    unsigned long long IV = 0x80400c0600000000UL;

    for ( int i = 0; i < 64; i++ )
    {
        if ( ( IV >> ( 63 - i ) & 0x1 ) == 1 ) 
            _S[i] = Polynomial( "1" ); 
        else
            _S[i] = Polynomial( "0" ); 
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[64 + i] = createVar( string( "k" ), i );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[128 + i] = createVar( string( "k" ), i + 64 );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[192 + i] = createVar( string( "u" ), i  );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[256 + i] = createVar( string( "u" ), i + 64 );
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        //_S[ 192 + D[index] ] = _x[ 1 << index ];
        _S[ 256 + D[index] ] = _x[ 1 << index ];
    }

    for ( int i = 0; i < 128; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::initMid( vector<int> & D  )
{
    for ( int i = 0; i < _SIZE; i++ )
    {
        _S[i] = createVar( string( "u" ), i  );
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        _S[ 0 + D[index] ] = _S[0 + D[index] ] + _x[ 1 << index ];
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::initVec( vector< vector< int> > & D, vector<int> & row )
{
    for ( int i = 0; i < _SIZE; i++ )
    {
        _S[i] = createVar( string( "u" ), i );
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        for ( auto it : D[index] )
        {
            for ( auto jt : row )
                _S[ 64 * jt + it ] = _S[ 64 * jt + it ] + _x[ 1 << index ];
            //_S[ 256 + it ] = _x[ 1 << index ];
        }
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::initValueMap( vector<int> & D, const map< string, string > & MAP )
{
    unsigned long long IV = 0x80400c0600000000UL;

    for ( int i = 0; i < 64; i++ )
    {
        if ( ( IV >> ( 63 - i ) & 0x1 ) == 1 ) 
            _S[i] = Polynomial( "1" ); 
        else
            _S[i] = Polynomial( "0" ); 
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[64 + i] = createVar( string( "k" ), i );
        //_S[64 + i] = createVar( "0" );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[128 + i] = createVar( string( "k" ), i + 64 );
        //_S[128 + i] = createVar( "0" );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[192 + i] = createVar( string( "u" ), i  );
        //_S[192 + i] = createVar( string( "0" ) );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[256 + i] = createVar( string( "u" ), i + 64 );
        //_S[256 + i] = createVar( "0" );
    }

    for ( int i = 0; i < 320; i++ )
    {
        if ( MAP.find( _S[i].getPoly() ) != MAP.end() )
        {
            _S[i] = Polynomial( MAP.at( _S[i].getPoly() ) );
        }
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        _S[ 192 + D[index] ] = _x[ 1 << index ];
        _S[ 256 + D[index] ] = _x[ 1 << index ];
    }

    for ( int i = 0; i < 128; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::init( const vector<int> & D )
{
    unsigned long long IV = 0x80400c0600000000UL;

    for ( int i = 0; i < 64; i++ )
    {
        if ( ( IV >> ( 63 - i ) & 0x1 ) == 1 ) 
            _S[i] = Polynomial( "1" ); 
        else
            _S[i] = Polynomial( "0" ); 
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[64 + i] = createVar( string( "k" ), i );
        //_S[64 + i] = createVar( "0" );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[128 + i] = createVar( string( "k" ), i + 64 );
        //_S[128 + i] = createVar( "0" );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[192 + i] = createVar( string( "u" ), i  );
        //_S[192 + i] = createVar( string( "0" ) );
    }

    for ( int i = 0; i < 64; i++ )
    {
        _S[256 + i] = createVar( string( "u" ), i + 64 );
        //_S[256 + i] = createVar( "0" );
    }

    for ( size_t index = 0; index < D.size(); index++ )
    {
        _S[ 192 + D[index] ] = _x[ 1 << index ];
        _S[ 256 + D[index] ] = _x[ 1 << index ];

    }

    for ( int i = 0; i < 128; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::resetBM()
{
    _BM.clear();
    for ( int i = 0; i < 384; i++ )
    {
        _BM[ createVar( string( "u" ), i ).getPoly() ] = 0;
        _BM[ createVar( string( "k" ), i ).getPoly() ] = 0;
    }
}

void AsconHATF::RunPA( int conIndex )
{
    auto temp = addConst( _S, conIndex );
    _S = SboxPL ( temp );
}

void AsconHATF::RunPB()
{
    auto temp = SboxPN( _S );
    _S = Matrix( temp );
}

void AsconHATF::RunSboxPNLast()
{
    _S = SboxPNLast( _S );
}

void AsconHATF::RunSboxPN()
{
    _S = SboxPN( _S );
}

void AsconHATF::RunRF( int conIndex )
{
    RunPB();
    RunPA( conIndex );
}

void AsconHATF::RunSboxAC( int conIndex ) 
{
    //auto start = chrono::steady_clock::now();
    auto temp = addConst( _S, conIndex );
    _S = SboxM( temp );
    //_S = Sbox( temp );
    //auto end = chrono::steady_clock::now();

    // cout << "Sbox, Elapsed time in nano seconds: "
    //    << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
    //   << " sec" << endl;
}

void AsconHATF::RunFirstSboxAC( int conIndex ) 
{
    auto temp = addConst( _S, conIndex );
    _S = SboxM( temp );
}

void AsconHATF::RunMatrixS( int conIndex ) 
{
    auto temp = Matrix( _S );
    temp = addConst( temp, conIndex );
    _S = SboxM( temp );
}

void AsconHATF::RunMatrix() 
{
    //auto start = chrono::steady_clock::now();
    auto S = Matrix( _S );
    _S = S;
    //auto end = chrono::steady_clock::now();
    // cout << "Matrix, Elapsed time in nano seconds: "
    //    << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
    //    << " sec" << endl;

}

map<string, string> AsconHATF::getValueMap( const vector<int> & D, int value )
{
    map<string, string> MAP;
    if ( value == -1 )
        return MAP;

    size_t DIM = D.size();
    int * vl = new int [ 2 * DIM ];

    for ( size_t i = 0; i < 2 * DIM; i++ )
    {
        vl[i] = value >> ( 2 * DIM - 1 - i ) & 0x1; 
    }
    for (size_t i = 0; i < DIM; i++ )
    {
        MAP[ createVar( "k", D[i] ).getPoly() ] = to_string( vl[ 2 * i] );
        MAP[ createVar( "k", 64 + D[i] ).getPoly() ] = to_string( vl[ 2* i + 1] );
    }
    delete [] vl;

    return MAP;
}

map<string, string> AsconHATF::getValueMap1( int value )
{
    map<string, string> MAP;

if ( value >> ( 6 - 0 ) & 0x1 )
    MAP.insert( make_pair( string( "k64" ), string( "0" ) ) );
else
    MAP.insert( make_pair( string( "k64" ), string( "1" ) ) );
if ( value >> ( 6 - 1 ) & 0x1 )
    MAP.insert( make_pair( string( "k0" ), string( "0" ) ) );
else
    MAP.insert( make_pair( string( "k0" ), string( "1" ) ) );
if ( value >> ( 6 - 2 ) & 0x1 )
    MAP.insert( make_pair( string( "u23" ), string( "u54+u57+u87*k23+u87+u111+u118+u121*k57+u121+k23+k47+k54+k57+k111+k118" ) ) );
else
    MAP.insert( make_pair( string( "u23" ), string( "u54+u57+u87*k23+u87+u111+u118+u121*k57+u121+k23+k47+k54+k57+k111+k118+1" ) ) );
if ( value >> ( 6 - 3 ) & 0x1 )
    MAP.insert( make_pair( string( "u95" ), string( "u28*k92+u28+u31*k31+u31*k95+u31+u53*k53+u53*k117+u53+u92+u117+k28*k92+k28+k31*k95+k31+k53*k117+k53+k92+k95+k117" ) ) );
else
    MAP.insert( make_pair( string( "u95" ), string( "u28*k92+u28+u31*k31+u31*k95+u31+u53*k53+u53*k117+u53+u92+u117+k28*k92+k28+k31*k95+k31+k53*k117+k53+k92+k95+k117+1" ) ) );
if ( value >> ( 6 - 4 ) & 0x1 )
    MAP.insert( make_pair( string( "u67" ), string( "u3*k67+u3+u25*k25+u25*k89+u25+u89+k3*k67+k3+k25*k89+k25+k67+k89" ) ) );
else
    MAP.insert( make_pair( string( "u67" ), string( "u3*k67+u3+u25*k25+u25*k89+u25+u89+k3*k67+k3+k25*k89+k25+k67+k89+1" ) ) );
if ( value >> ( 6 - 5 ) & 0x1 )
    MAP.insert( make_pair( string( "u2" ), string( "u42+u66+u76*k12+u76+u83*k19+u106*k42+u106+k2+k9+k12+k42+k66+k73+k83" ) ) );
else
    MAP.insert( make_pair( string( "u2" ), string( "u42+u66+u76*k12+u76+u83*k19+u106*k42+u106+k2+k9+k12+k42+k66+k73+k83+1" ) ) );
if ( value >> ( 6 - 6 ) & 0x1 )
    MAP.insert( make_pair( string( "u11" ), string( "u21+u51+u75+u82+u85*k21+u85+u92*k28+u115*k51+u115+k11+k18+k51+k75+k82+k92" ) ) );
else
    MAP.insert( make_pair( string( "u11" ), string( "u21+u51+u75+u82+u85*k21+u85+u92*k28+u115*k51+u115+k11+k18+k51+k75+k82+k92+1" ) ) );
    for ( auto it : MAP )
    {
        MAP[it.first] = sortString( it.second );
    }

    return MAP;
}









