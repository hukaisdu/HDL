#include"AsconHATF.h"
#include"ascon.h"
#include<chrono>
#include<thread>
#include<mutex>
#include<map>
#include"polynomial.h"
#include"quadratic.h"
#include<cassert>

using namespace std;

HATF::HATF( int ORDER, int size, int ROUND, int level  ):
    _S ( size ),
    _ORDER( ORDER ),
    _SIZE( size ),
    _ROUND ( ROUND ),
    _LEVEL( level )
{

    for ( int i = 0; i < ( 1 << _ORDER); i++ )
    {
        _x.push_back(  Polynomial( "1" ) );
        for ( int j = 0; j < _ORDER; j++ )
            if ( i >> j & 0x1 )
                _x[i] = _x[i] * Polynomial( string( "x" ) + to_string( j ) );
    }


    _TM = new map<string, string> * [ _LEVEL ];
    for ( int i = 0; i < _LEVEL; i++ )
    {
        _TM[i] = new map<string, string> [ _SIZE ];
    }
}

HATF::~HATF()
{
    for ( int i = 0; i < _LEVEL; i++ )
    {
        delete [] _TM[i];
    }
    delete [] _TM;
}

Polynomial HATF::getX( int index )
{
    return _x[ index ];
}

Polynomial HATF::getS( int i )
{
    return _S[i];
}

int HATF::getOrder()
{
    return _ORDER;
}

void HATF::setS( int i, const Polynomial & s )
{
    _S[i] = s;
}

Polynomial HATF::getNewVar( int r, int pos, int order )
{
    int index = order + ( r * _SIZE + pos ) * ( 1 << _ORDER );
    return Polynomial( string( "C" ) + to_string( index ) ); 
}

void HATF::Sub( int R, int mode, int START, int END, bool flag )
{
    map<string, Polynomial> T;

    // 320 / 8
    //int STEP = _SIZE / THREADNUM;

    map<string, double> * localM = new map<string, double> [ _SIZE ];
    map<string, string> * localTM = new map<string, string> [ _SIZE ];
    Polynomial * localS = new Polynomial [ _SIZE ];

    for ( int step = START; step < END; step++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;
        for ( int i = start; i < end; i++ )
        {
            threads.push_back( thread( WorkerQuadratic, R, i, ref(*this), ref( localS[i] ), ref( localTM[i] ), mode, flag ) );
            //threads.push_back( thread( WorkerQuadratic, R, i, ref(*this), ref(T), ref( localM[i] ), ref(_BM), ref( localS[i] ), mode, flag ) );
        }
        //cout << threads.size() << endl;
        for ( auto & th : threads )
            th.join();
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM.merge( localM[i] );
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _TM[R][i].merge( localTM[i] );
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        setS( i,localS[i] );
    }

    delete [] localM;
    delete [] localS;
    delete [] localTM;
}


void HATF::SubBias( int R, int mode, int START, int END, bool flag )
{
    map<string, Polynomial> T;

    // 320 / 8
    //int STEP = _SIZE / THREADNUM;

    map<string, double> * localBM = new map<string, double> [ _SIZE ];

    map<string, string> * localTM = new map<string, string> [ _SIZE ];

    Polynomial * localS = new Polynomial [ _SIZE ];

    for ( int step = START; step < END; step++ )
    {
        int start = step * THREADNUM;
        int end = step * THREADNUM + THREADNUM;

        vector<thread> threads;
        for ( int i = start; i < end; i++ )
        {
            threads.push_back( thread( WorkerQuadraticBias, R, i, ref(*this), ref( localS[i] ), ref( localTM[i] ), ref( _BM ), ref( localBM[i] ), mode, flag ) );
        }
        //cout << threads.size() << endl;
        for ( auto & th : threads )
            th.join();
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _BM.merge( localBM[i] );
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        _TM[R][i].merge( localTM[i] );
    }

    for ( int i = 0; i < _SIZE; i++ )
    {
        setS( i,localS[i] );
    }

    delete [] localBM;
    delete [] localS;
    delete [] localTM;
}

int weight ( int x )
{
    int wt = 0;
    for ( int i = 0; i < 32; i++ )
        wt += x >> i & 0x1;
    return wt;
}

void WorkerQuadraticBias( int R, int i, HATF & hatf, Polynomial & localS, map<string, string> & localTM, 
        const map<string, double> & BM, 
        map<string, double> & localBM, 
        int mode, bool flag )
{
    int ORDER = hatf.getOrder(); 

    vector< Polynomial > COEF ( 1 << ORDER );

    for ( int d = 0; d < (1 << ORDER); d++ )
    {
        if ( ( flag == true ) && ( d < ( ( 1 << ORDER ) - 1 ) ) ) 
            continue;

        //ke
        //cout << R << " " << i << " " << d << endl;

        auto coef = hatf.getS( i ).getCoef ( hatf.getX( d ) );

        if ( coef.getPoly() == string("") ) 
        {
            COEF[d] = Polynomial( "" );
        }
        else if ( coef.getPoly() == "1" )
        {
           COEF[d] = Polynomial( "1" );
        }
        else if ( coef.getDegree() <= 1 )
        //else if ( coef.getVariables().size() <= 1 )
        {
           COEF[d] = coef;
        }
        else if ( localTM.find( coef.getPoly() ) != localTM.end() )
        {
            COEF[d] = Polynomial ( localTM[ coef.getPoly() ] );
        }
        else if ( localTM.find( (coef + Polynomial("1")).getPoly() ) != localTM.end() )
        {
            COEF[d] = Polynomial ( localTM[ (coef + Polynomial( "1" ) ).getPoly() ] ) + Polynomial( "1" );
        }
        else
        {

           COEF[d] = hatf.getNewVar( R, i, d );
           localTM[ COEF[d].getPoly() ] = coef.getPoly();
            
            //COEF[d] = hatf.getNewVar( R, i, d );
            //localTM[ COEF[d].getPoly() ] = coef.getPoly();
            
            auto Q = coef;
            auto nv = COEF[d];

            double bias = 0;

            if ( d > 0 )
            {
                    if ( Q.getDegree() == 1 )
                        bias = Q.calBiasLinear( BM );
                    else
                    {
                        if ( mode == CLASSIC )
                        {
                            map<string, double> trans;
                            for ( auto const & it : Q.getVariables() )
                                trans[it.getPoly()] = BM.at( it.getPoly() );

                            bias = Q.calBiasNonLinearQ( trans );
                        }
                        else if ( mode == DISJOINT )
                        {
                            if ( Q.getDegree() > 2 )
                                bias = 0;
                            else
                            {
                                auto V = Q.getVariables();

                                int dim = V.size();

                                int ** IM = new int * [ dim ];
                                for ( int i = 0; i < dim; i++ )
                                    IM[i] = new int [ dim ];

                                auto PP = Algorithm1( Q, V, IM );

                                auto VV = mulMatrixV( V, IM, dim );

                                for ( int i =0; i < dim; i++ )
                                    delete [] IM[i];
                                delete [] IM;

                                map<string, double> Trans;
                                for ( int row = 0; row < dim; row ++ )
                                {
                                    Trans[ V[row].getPoly() ] = VV[row].calBiasLinear( BM );
                                }

                                bias = calBiasDisjoint( PP, Trans );
                            }
                        }
                    }
            }
            else
                bias = 0;

            //lock_guard<mutex> lock( workmutex );
            //workmutex.lock();
            localBM[ nv.getPoly() ] = bias;
        }
    }

    //int sizeadd = localBM.size();

    localS = Polynomial( "0" );

    for ( int d = 0; d < ( 1 << ORDER ); d++ )
        localS = localS + COEF[d] * hatf.getX( d );
}

void WorkerQuadratic( int R, int i, HATF & hatf, Polynomial & localS, map<string, string> & localTM, int mode, bool flag )
{
    int ORDER = hatf.getOrder(); 

    vector< Polynomial > COEF ( 1 << ORDER );

    for ( int d = 0; d < (1 << ORDER); d++ )
    {
        if ( ( flag == true ) && ( d < ( ( 1 << ORDER ) - 1 ) ) ) 
            continue;

        //ke
        //cout << R << " " << i << " " << d << endl;

        auto coef = hatf.getS( i ).getCoef ( hatf.getX( d ) );

        if ( coef.getPoly() == string("") ) 
        {
            COEF[d] = Polynomial( "" );
        }
        else if ( coef.getPoly() == "1" )
        {
           COEF[d] = Polynomial( "1" );
        }
        else if ( coef.getDegree() <= 1 )
        //else if ( coef.getVariables().size() <= 1 )
        {
           COEF[d] = coef;
        }
        else if ( localTM.find( coef.getPoly() ) != localTM.end() )
        {
            COEF[d] = Polynomial ( localTM[ coef.getPoly() ] );
        }
        else if ( localTM.find( (coef + Polynomial("1")).getPoly() ) != localTM.end() )
        {
            COEF[d] = Polynomial ( localTM[ (coef + Polynomial( "1" ) ).getPoly() ] ) + Polynomial( "1" );
        }
        else
        {
            /*
            auto Q = coef.graded_part(2);
            auto M = coef.graded_part(1);
            for ( auto var : M.getVariables() )
                 Q = Q + var;

            auto L = Q + coef;
            auto nv = hatf.getNewVar( R, i, d );

            COEF[d] = nv + L;
            */

           COEF[d] = hatf.getNewVar( R, i, d );
           localTM[ COEF[d].getPoly() ] = coef.getPoly();
            
            //COEF[d] = hatf.getNewVar( R, i, d );
            //localTM[ COEF[d].getPoly() ] = coef.getPoly();
            
           /*
            auto Q = coef;
            auto nv = COEF[d];

            double bias = 0;

            if ( d > 0 )
            {
                    if ( Q.getDegree() == 1 )
                        bias = Q.calBiasLinear( BM );
                    else
                    {
                        if ( mode == CLASSIC )
                        {
                            map<string, double> trans;
                            for ( auto const & it : Q.getVariables() )
                                trans[it.getPoly()] = BM.at( it.getPoly() );

                            bias = Q.calBiasNonLinearQ( trans );
                        }
                        else if ( mode == DISJOINT )
                        {
                            auto V = Q.getVariables();

                            int dim = V.size();

                            int ** IM = new int * [ dim ];
                            for ( int i = 0; i < dim; i++ )
                                IM[i] = new int [ dim ];

                            auto PP = Algorithm1( Q, V, IM );

                            auto VV = mulMatrixV( V, IM, dim );

                            for ( int i =0; i < dim; i++ )
                                delete [] IM[i];
                            delete [] IM;

                            map<string, double> Trans;
                            for ( int row = 0; row < dim; row ++ )
                            {
                                Trans[ V[row].getPoly() ] = VV[row].calBiasLinear( BM );
                            }

                            bias = calBiasDisjoint( PP, Trans );
                        }
                    }
            }
            else
                bias = 0;

            //lock_guard<mutex> lock( workmutex );
            //workmutex.lock();
            localBM[ nv.getPoly() ] = bias;
            */
        }
    }

    //int sizeadd = localBM.size();

    localS = Polynomial( "0" );

    for ( int d = 0; d < ( 1 << ORDER ); d++ )
        localS = localS + COEF[d] * hatf.getX( d );
}


/*
void HATF::CalBias( int mode )
{
    for ( int R = 0; R < _LEVEL; R++ )
    {

        for ( int pos = 0; pos < _SIZE; pos ++ )
        {
            for ( auto const & p : _TM[R][pos] )
            {
                auto Q = Polynomial( p.second );
                double bias = 0;
                if ( Q.getDegree() == 1 )
                    bias = Q.calBiasLinear( _BM );
                else
                {
                    if ( mode == CLASSIC )
                    {
                        map<string, double> trans;
                        for ( auto const & it : Q.getVariables() )
                            trans[it.getPoly()] = _BM.at( it.getPoly() );

                        bias = Q.calBiasNonLinearQ( trans );
                    }
                    else if ( mode == DISJOINT )
                    {
                        auto V = Q.getVariables();

                        int dim = V.size();

                        int ** IM = new int * [ dim ];
                        for ( int i = 0; i < dim; i++ )
                            IM[i] = new int [ dim ];

                        auto PP = Algorithm1( Q, V, IM );

                        auto VV = mulMatrixV( V, IM, dim );

                        for ( int i =0; i < dim; i++ )
                            delete [] IM[i];
                        delete [] IM;

                        map<string, double> Trans;
                        for ( int row = 0; row < dim; row ++ )
                        {
                            Trans[ V[row].getPoly() ] = VV[row].calBiasLinear( _BM );
                        }

                        bias = calBiasDisjoint( PP, Trans );
                    }
                }

                _BM[ p.first ] = bias;
            }
        }
    }
}
*/

void WorkerBias( int R, int pos, const map< string, string> & TM, 
        const map<string,double> & BM, 
        map<string, double> & localBM, 
        const map<string, string> & valueMap,
        map<string, string> & localVMap, int mode )
{
    for ( auto const & p : TM )
    {
        // simplify
        // p.second -> with map
        auto pp = Polynomial( p.second );
        auto ps = Subs( pp, valueMap ); 

        double bias = 0;

        if ( ps.getPoly() == "" )
            bias = 0.5;
        else if ( ps.getPoly() == "1" )
            bias = -0.5;
        else if ( ps.getDegree() == 1 )
            bias = 1;
        else
        {
            if ( mode == CLASSIC )
            {
                if ( ps.getVariables().size() > 20 )
                    bias = 0;
                else
                {
                    map<string, double> trans;
                    for ( auto const & it : ps.getVariables() )
                        trans[it.getPoly()] = BM.at( it.getPoly() );

                    bias = ps.calBiasNonLinearQ( trans );
                }
            }
            else if ( mode == DISJOINT )
            {
                if ( ps.getDegree() > 2 )
                {
                    bias = 0;
                }
                else
                {
                    //cout << "ps " << ps << endl;
                    auto V = ps.getVariables();

                    int dim = V.size();

                    int ** IM = new int * [ dim ];
                    for ( int i = 0; i < dim; i++ )
                        IM[i] = new int [ dim ];


                    auto PP = Algorithm1( ps, V, IM );

                    auto VV = mulMatrixV( V, IM, dim );

                    for ( int i =0; i < dim; i++ )
                        delete [] IM[i];
                    delete [] IM;

                    map<string, double> Trans;
                    for ( int row = 0; row < dim; row ++ )
                    {
                        Trans[ V[row].getPoly() ] = VV[row].calBiasLinear( BM );
                    }

                    bias = calBiasDisjoint( PP, Trans );
                }

            }
        }

        if ( bias == 0.5 )
            localVMap[ p.first ] = to_string( 0 ); 
        else if ( bias == -0.5 )
            localVMap[ p.first ] = to_string( 1 ); 
        else if ( bias == 1 )
            localVMap[ p.first ] = ps.getPoly();
        else
            localBM[ p.first ] = bias;

    }
}

map<string,string> HATF::CalBias( int mode, const map<string, string> & valueMap )
{
    map<string, string> secondValueMap = valueMap;

    for ( int R = 0; R < _LEVEL; R++ )
    {
        map<string, double> * localBM = new map<string, double> [ _SIZE ];
        map<string, string> * localVMap = new map<string, string> [ _SIZE ];

        int STEP = _SIZE / THREADNUM;

        for ( int step = 0; step < STEP; step ++ )
        {
            int start = step * THREADNUM;
            int end = step * THREADNUM + THREADNUM;
            vector<thread> threads;
            for ( int i = start; i < end; i++ )
            {
                //cout << R << "  " << i << endl;
                threads.push_back( thread( WorkerBias,  R, i, ref( _TM[R][i] ), ref( _BM ), ref( localBM[i] ), ref( secondValueMap ), ref( localVMap[i] ), mode ) );
            }
            for ( auto & th : threads )
                th.join();
        }

        for ( int i = 0; i < _SIZE; i++ )
            _BM.merge( localBM[i] );

        for ( int i = 0; i < _SIZE; i++ )
            secondValueMap.merge( localVMap[i] );

        //for ( auto it : secondValueMap )
        //    cout << it.first << " " << it.second << endl;

        delete [] localBM;
        delete [] localVMap;
    }

    return secondValueMap;
}

Polynomial Subs( const Polynomial& p, const map<string, string> & MAP )
{
    auto V = p.getVariables();
    bool flag = false;

    for ( auto const & v : V )
        if ( MAP.find( v.getPoly() ) != MAP.end() )
        {
            flag = true;
            break;
        }

    if ( flag == false )
        return p;
    else
    {
        auto M = p.getMonomials();
        Polynomial res ( "" );
        for ( auto const & m : M )
        {
            //cout << m << endl;
            Polynomial term ( "1" );
            if ( m.getPoly() == "1" )
                ;
            else
            {
                for ( auto const & v : m.getVariables() )
                {
                    if ( MAP.find( v.getPoly() ) != MAP.end() )
                        term = term * Polynomial( MAP.at( v.getPoly() ) );
                    else
                        term = term * v;
                }
            }
            res = res + term;
            //cout << res << endl;
        }

        return res;
    }
}




