#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include"split.h"
#include"polynomial.h"
#include"XoodooHATF.h"
#include"log.h"
#include"quadratic.h"
#include<iomanip>
#include<cassert>
#include<fstream>
#include<cmath>

using namespace std;

extern int THREADNUM;

void ReadUV( const vector<int> & D, int ** U )
{
    string name ( "C" ); 
    for ( auto it : D )
    {
        name += string( "_" ) + to_string( it );
    }
    name += string( ".txt" );

    ifstream is;
    is.open( name, ios::in );

    if ( is.is_open( ) )
    {
        for ( int i = 0; i < ( 1 << D.size() ); i++ )
        {
            string buff;
            getline( is, buff ); 
            auto value = split ( buff, "," );
            for ( int j = 0; j < 384; j++ )
            {
                //cout << value[j] << endl;
                try
                {
                    U[i][j] = stoi( value[j] );
                }
                catch( ... )
                {
                    cerr << value[j] << endl;
                }
            }
        }
    }
    else
    {
        cerr << "File is not opened " << name << endl;
        exit( EXIT_FAILURE );
    }
}



int xoodoo_4_related()
{
    int ROUND = 4;
    int ORDER = 1;
    int MODE = DISJOINT;

    double maxv = 0;

    vector<int> D { 0 };
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initRelated( D );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    double ave = 0;
    for ( int value = 0; value < 4; value ++ )
    {
        cout << hex << value<< endl;

        hatf.resetBM();

        auto valueMap = hatf.getValueMapV1( value );

        /*
        for ( auto it : valueMap )
            cout << it.first << " " << it.second << endl;
        */

        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            if ( i == 0 )
            {
                cout << bias << endl;
                ave += bias;
            }
        }
    }

    cout << ave / 4 << endl;


    return 0;
}

int xoodoo_4_second()
{
    int ROUND = 4;
    int ORDER = 2;
    int MODE = DISJOINT;

    double maxv = 0;

    vector<int> D { 0, 47 };
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    double ave[192] = { 0 };

    for ( int value = 0; value < 256; value ++ )
    {
        cout << hex << value<< endl;
        hatf.resetBM();

        auto valueMap = hatf.getValueMapV0( value );

        /*
        for ( auto it : valueMap )
            cout << it.first << " " << it.second << endl;
        */

        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            ave[ i ] += bias;

        }
    }

    for ( int i = 0; i < 192; i++ )
        cout << i << " " << ave[i] / 256 << endl;


    return 0;
}

int xoodoo_4_start()
{
    int ROUND = 4;
    int ORDER = 1;
    int MODE = DISJOINT;

    double maxv = 0;

    vector<int> D { 0 };
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    double ave = 0;
    for ( int value = 0; value < 16; value ++ )
    {
        cout << hex << value<< endl;
        hatf.resetBM();

        auto valueMap = hatf.getValueMapV0( value );

        /*
        for ( auto it : valueMap )
            cout << it.first << " " << it.second << endl;
        */

        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            if ( i == 143 )
            {
                cout << bias << endl;
                ave += bias;
            }
        }
    }

    cout << ave / 16.0 << endl;


    return 0;
}

int xoodooPerm()
{
    int ROUND = 5;
    int ORDER = 3;
    int MODE = DISJOINT;

    double maxv = 0;

    vector<int> D { 0, 1, 10 };
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.init( D );

    hatf.RunChi(); 
    hatf.RunPE(); 
    hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND -1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, true );
    }

    for ( int i = 0 ; i < 384; i++ )
    {
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );

        auto bias = f0.calBiasLinear( hatf._BM );
        cout << i << " " << bias << endl;
        if ( abs( bias ) > abs( maxv ) )
        {
            maxv = bias;
            cout << i << " " << maxv << endl;
        }
    }
    return 0;
}

int xoodoo_4_general()
{
    int ROUND = 4;
    int ORDER = 2;
    int MODE = DISJOINT;


    double maxv = 0;

    for ( int index = 1; index < 128; index++ )
    //for ( int index1 = 123; index1 < 124; index1++ )
    //for ( int index2 = 1; index2 < 128; index2++ )
    {
        vector<int> D { 0, index };
        XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

        hatf.initFromMiddle( D, 5, true );

        //hatf.RunChi(); 
        //hatf.RunPE(); 
        //hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

        for ( int r = 0; r < ROUND; r++ )
        {
            hatf.RunRound( r, ROUND ); 
            if ( r < ROUND - 1 )
            hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
            else
            hatf.SubBias( r, MODE, 0, 192/THREADNUM, true );
        }

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );

            bias = f0.calBiasLinear( hatf._BM );

            //cout << i << " " << bias << endl;

            if ( abs( bias ) > abs( maxv ) )
            {
                maxv = bias;
                cout << index << " " << i << " " << maxv << " " << log( maxv ) / log(2) << endl;
            }
        }
    }
    return 0;
}

int xoodoo_5_without_last()
{
    int ROUND = 5;
    int ORDER = 2;
    int MODE = DISJOINT;

    double maxv = 0;

    vector<int> D { 0 };
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    double ave = 0;
    for ( int value = 0; value < 16; value ++ )
    {
        cout << hex << value<< endl;
        hatf.resetBM();

        auto valueMap = hatf.getValueMapV0( value );

        /*
        for ( auto it : valueMap )
            cout << it.first << " " << it.second << endl;
        */

        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            if ( i == 143 )
            {
                cout << bias << endl;
                ave += bias;
            }
        }
    }

    cout << ave / 16.0 << endl;


    return 0;
}

vector<double> getBiasXoodyak( int ROUND, int ORDER, const vector<int> & D, int MODE )
{
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int i = 0 ; i < 192; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] += bias;
    }

    return res;
}

vector<double> getBiasXoodoo( int ROUND, int ORDER, const vector<int> & D, int MODE )
{
    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.init( D );

    hatf.RunChi();
    hatf.RunPE();
    hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, true );
    }

    vector<double> res (384, 0);

    for ( int i = 0 ; i < 384; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] += bias;
    }

    return res;
}

vector<double> xoodyak_application1_e128e256( int MODE )
{
    int ROUND = 4;
    int ORDER = 1;
    vector<int> D { 0 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int value = 0; value < 16; value++ )
    {
        printSteps( value, 16 );

        hatf.resetBM();
        auto valueMap = hatf.getValueMapV0( value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            res[i] += bias;
        }
    }
    cout << endl;

    for ( int i = 0; i < 192; i++ )
        res[i] = res[i] / 16;

    return res;
}

vector<double> xoodyak_application1_e0e256( int MODE )
{
    int ROUND = 4;
    int ORDER = 1;
    vector<int> D { 0 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initRelated( D );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int value = 0; value < 16; value++ )
    {
        printSteps( value, 16 );

        hatf.resetBM();
        auto valueMap = hatf.getValueMapV1( value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            if ( i == 0 )
                cout << bias << endl;

            res[i] += bias;
        }
    }
    cout << endl;

    for ( int i = 0; i < 192; i++ )
        res[i] = res[i] / 16;

    return res;
}

vector<double> xoodyak_application2( int MODE )
{
    int ROUND = 4;
    int ORDER = 2;
    vector<int> D { 0, 47 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int i = 0 ; i < 192; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;
    }

    return res;
}

vector<double> xoodyak_application3( int MODE )
{
    int ROUND = 4;
    int ORDER = 4;
    vector<int> D { 0, 71, 79, 123 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initRelated( D );

    hatf.RunChi();
    hatf.RunPE();
    hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int i = 0 ; i < 192; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;
    }

    return res;
}

vector<double> xoodyak_application4( int MODE )
{
    int ROUND = 5;
    int ORDER = 2;
    vector<int> D { 0, 34 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    for ( int value = 0; value < 256; value++ )
    {
        printSteps( value, 256 );

        hatf.resetBM();
        auto valueMap = hatf.getValueMapV2( value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0 ; i < 192; i++ )
        {
            double bias = 0;
            auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
            f0 = Subs( f0, finalMap );
            bias = f0.calBiasLinear( hatf._BM );

            res[i] += bias;

            //if ( i == 52 )
            //    cout << bias << endl;
        }
    }

    cout << endl;

    for ( int i = 0; i < 192; i++ )
        res[i] = res[i] / 256;

    return res;
}

vector<double> xoodyak_application5_4round( int MODE )
{
    int ROUND = 4;
    int ORDER = 2;
    vector<int> D { 0, 20 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.init( D );

    hatf.RunChi();
    hatf.RunPE();
    hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, true );
    }

    vector<double> res (384, 0);

    for ( int i = 0 ; i < 384; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;
    }

    return res;
}

vector<double> xoodyak_application5_5round( int MODE )
{
    int ROUND = 5;
    int ORDER = 3;
    vector<int> D { 0, 13, 14 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.init( D );


    hatf.RunChi();
    hatf.RunPE();
    hatf.SubBias( 0, MODE, 0, 384/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.SubBias( r, MODE, 0, 384/THREADNUM, true );
    }

    vector<double> res (384, 0);

    for ( int i = 0 ; i < 384; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;
    }

    return res;
}

vector<double> xoodyak_application6( int MODE )
{
    int ROUND = 4;
    int ORDER = 2;
    vector<int> D { 0, 72 };

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );

    hatf.initKey( D, -1 );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    auto valueMap = hatf.getValueMapV3( );
    auto finalMap = hatf.CalBias( MODE, valueMap );

    for ( int i = 0 ; i < 192; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) - 1) );
        f0 = Subs( f0, finalMap );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;

        //if ( i == 52 )
        //    cout << bias << endl;
    }


    return res;
}

vector<double> xoodyak_application7( int MODE )
{
    int ROUND = 5;
    int ORDER = 3;
    vector<int> D { 0, 9, 36 };

    //int ** UV = new int*[8];
    //for ( int i = 0; i < 8; i++ )
    //    UV[i] = new int[384];

    XoodooHATF hatf ( ORDER, 384, ROUND, ROUND + 1 );
    hatf.initKey( D, -1 );

    //ReadUV( D, UV );

    //hatf.initDim( UV );

    for ( int r = 0; r < ROUND; r++ )
    {
        hatf.RunRound( r, ROUND ); 
        if ( r < ROUND - 1 )
        hatf.Sub( r, MODE, 0, 384/THREADNUM, false );
        else
        hatf.Sub( r, MODE, 0, 192/THREADNUM, true );
    }

    vector<double> res (192, 0);

    auto valueMap = hatf.getValueMapV4();
    auto finalMap = hatf.CalBias( MODE, valueMap );

    for ( int i = 0 ; i < 192; i++ )
    {
        double bias = 0;
        auto f0 = hatf.getS( i ).getCoef( hatf.getX( ( 1 << ORDER ) -1 ) );
        f0 = Subs( f0, finalMap );
        bias = f0.calBiasLinear( hatf._BM );

        res[i] = bias;
    }

    return res;
}
