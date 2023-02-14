#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include"split.h"
#include"polynomial.h"
#include"ascon.h"
#include"AsconHATF.h"
#include"quadratic.h"
#include<iomanip>
#include<random>
#include<limits>
#include"log.h"
#include<cassert>
#include<cstdio>

using namespace std;

extern int THREADNUM;
extern int MODE;

//constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};
//
void setCondition09( HATF & hatf, int d  )
{
    Polynomial x[128];
    Polynomial k[128];
    for ( int i = 0; i < 128; i++ )
    {
        x[i] = hatf.getS( 192 + i ); 
        k[i] = hatf.getS( 64 + i ); 
    }

    //k[0]= Polynomial( "" ); 
    //k[9]= Polynomial( "" ); 
    //k[64]= Polynomial( "" ); 
    int ID[14] = { 0 };
    for ( int i = 0; i < 14; i++ )
        ID[i] = d >> ( 13 - i ) & 0x1;
    
k[64] = Polynomial( to_string( ID[0] ) );
k[0] = Polynomial( to_string( ID[1] ) );
k[73] = Polynomial( to_string( ID[2] ) );
k[9] = Polynomial( to_string( ID[3] ) );
x[108] = x[19]*k[19]+x[19]*k[83]+x[19]+x[22]*k[22]+x[22]*k[86]+x[22]+x[44]*k[44]+x[44]*k[108]+x[44]+x[83]+x[86]+k[19]*k[83]+k[19]+k[22]*k[86]+k[22]+k[44]*k[108]+k[44]+k[83]+k[86]+k[108] + Polynomial( to_string( ID[4] ) );

x[118] = x[23]+x[47]+x[54]+x[57]+x[87]*k[23]+x[87]+x[111]+x[121]*k[57]+x[121]+k[23]+k[47]+k[54]+k[57]+k[111]+k[118] + Polynomial( to_string( ID[5] ) );

x[91] = x[27]+x[30]+x[60]+x[94]*k[30]+x[94]+x[101]*k[37]+x[124]*k[60]+x[124]+k[20]+k[27]+k[60]+k[84]+k[91]+k[101] + Polynomial( to_string( ID[6] ) );

x[98] = x[12]*k[12]+x[12]*k[76]+x[12]+x[34]*k[34]+x[34]*k[98]+x[34]+x[76]+k[12]*k[76]+k[12]+k[34]*k[98]+k[34]+k[76]+k[98]+Polynomial( "1" ) + Polynomial( to_string( ID[7] ) );

x[42] = x[2]+x[12]+x[66]+x[76]*k[12]+x[76]+x[83]*k[19]+x[106]*k[42]+x[106]+k[2]+k[12]+k[42]+k[66]+k[83] + Polynomial( to_string( ID[8] ) );

x[117] = x[28]*k[28]+x[28]*k[92]+x[28]+x[31]*k[31]+x[31]*k[95]+x[31]+x[53]*k[53]+x[53]*k[117]+x[53]+x[92]+x[95]+k[28]*k[92]+k[28]+k[31]*k[95]+k[31]+k[53]*k[117]+k[53]+k[92]+k[95]+k[117] + Polynomial( to_string( ID[9] ) );

x[89] = x[3]*k[3]+x[3]*k[67]+x[3]+x[25]*k[25]+x[25]*k[89]+x[25]+x[67]+k[3]*k[67]+k[3]+k[25]*k[89]+k[25]+k[67]+k[89]+Polynomial( "1" ) + Polynomial( to_string( ID[10] ) );

x[82] = x[11]+x[18]+x[21]+x[51]+x[75]+x[85]*k[21]+x[85]+x[92]*k[28]+x[115]*k[51]+x[115]+k[11]+k[18]+k[51]+k[75]+k[82]+k[92]+Polynomial( "1" ) + Polynomial( to_string( ID[11] ) );

x[126] = x[37]*k[37]+x[37]*k[101]+x[37]+x[40]*k[40]+x[40]*k[104]+x[40]+x[62]*k[62]+x[62]*k[126]+x[62]+x[101]+x[104]+k[37]*k[101]+k[37]+k[40]*k[104]+k[40]+k[62]*k[126]+k[62]+k[101]+k[104]+k[126]+Polynomial( "1" ) + Polynomial( to_string( ID[12] ) );

x[127] = x[2]+x[32]+x[56]+x[63]+x[66]*k[2]+x[66]+x[96]*k[32]+x[96]+x[120]+k[2]+k[32]+k[56]+k[63]+k[120]+k[127]+Polynomial( "1" ) + Polynomial( to_string( ID[13] ) );


    for ( int i = 0; i < 128; i++ )
    {
        hatf.setS( 192 + i, x[i] ); 
        hatf.setS( 64 + i, k[i] ); 
    }
}

void setCondition03061( HATF & hatf, int d  )
{
    Polynomial x[128];
    Polynomial k[128];
    for ( int i = 0; i < 128; i++ )
    {
        x[i] = hatf.getS( 192 + i ); 
        k[i] = hatf.getS( 64 + i ); 
    }

    //k[0]= Polynomial( "" ); 
    //k[9]= Polynomial( "" ); 
    //k[64]= Polynomial( "" ); 
    int ID[20] = { 0 };
    for ( int i = 0; i < 20; i++ )
        ID[i] = d >> ( 19 - i ) & 0x1;

    k[64] = Polynomial( to_string( ID[0] ) );
    k[94] = Polynomial( to_string( ID[1] ) );
    k[0] = Polynomial( to_string( ID[2] ) );
    k[30] = Polynomial( to_string( ID[3] ) );
    k[125] = Polynomial( to_string( ID[4] ) );
    k[61] = Polynomial( to_string( ID[5] ) );

    x[86] = x[22]*k[22]+x[22]*k[86]+x[22]+k[22]*k[86]+k[22]+k[86]+Polynomial("1");

    x[80] = x[16]*k[16]+x[16]*k[80]+x[16]+x[19]*k[19]+x[19]*k[83]+x[19]+x[41]*k[41]+x[41]*k[105]+x[41]+x[83]+x[105]+k[16]*k[80]+k[16]+k[19]*k[83]+k[19]+k[41]*k[105]+k[41]+k[80]+k[83]+k[105];

    x[122] = x[19]*k[19]+x[19]*k[83]+x[19]+x[58]*k[58]+x[58]*k[122]+x[83]+k[19]*k[83]+k[19]+k[58]*k[122]+k[83]+k[122]+Polynomial("1");

    x[111] = x[23]+x[47]+x[54]+x[57]+x[87]*k[23]+x[87]+x[118]+x[121]*k[57]+x[121]+k[23]+k[47]+k[54]+k[57]+k[111]+k[118]+Polynomial( to_string( ID[6] ) );

    x[17] = x[41]+x[48]+x[51]+x[81]*k[17]+x[81]+x[105]+x[112]+x[115]*k[51]+x[115]+x[122]*k[58]+k[17]+k[41]+k[48]+k[51]+k[105]+k[112]+k[122]+Polynomial( to_string( ID[7] ) );


    x[66] = x[2]+x[12]+x[42]+x[76]*k[12]+x[76]+x[83]*k[19]+x[106]*k[42]+x[106]+k[2]+k[9]+k[12]+k[42]+k[66]+k[73]+k[83]+Polynomial( to_string( ID[8] ) );

    x[119] = x[33]*k[33]+x[33]*k[97]+x[33]+x[55]*k[55]+x[55]*k[119]+x[55]+x[97]+k[33]*k[97]+k[33]+k[55]*k[119]+k[55]+k[97]+k[119]+Polynomial("1")+Polynomial( to_string( ID[9] ) );

    x[96] = x[8]+x[32]+x[39]+x[42]+x[72]*k[8]+x[72]+x[103]+x[106]*k[42]+x[106]+x[113]*k[49]+k[8]+k[32]+k[39]+k[42]+k[96]+k[103]+k[113]+Polynomial("1")+Polynomial( to_string( ID[10] ) );

    x[20] = x[44]+x[51]+x[54]+x[84]*k[20]+x[84]+x[108]+x[115]+x[118]*k[54]+x[118]+k[44]+k[51]+k[54]+k[108]+k[115]+Polynomial("1")+Polynomial( to_string( ID[11] ) );

    x[79] = x[8]+x[15]+x[18]+x[48]+x[72]+x[82]*k[18]+x[82]+x[89]*k[25]+x[112]*k[48]+x[112]+k[8]+k[15]+k[18]+k[48]+k[72]+k[79]+k[89]+Polynomial("1")+Polynomial( to_string( ID[12] ) );

    x[67] = x[3]*k[3]+x[3]*k[67]+x[3]+x[25]*k[25]+x[25]*k[89]+x[25]+x[89]+k[3]*k[67]+k[3]+k[25]*k[89]+k[25]+k[67]+k[89]+Polynomial("1")+Polynomial( to_string( ID[13] ) );

    x[116] = x[10]*k[10]+x[10]*k[74]+x[10]+x[49]*k[49]+x[49]*k[113]+x[49]+x[52]*k[52]+x[52]*k[116]+x[52]+x[74]+x[113]+k[10]*k[74]+k[10]+k[49]*k[113]+k[49]+k[52]*k[116]+k[52]+k[74]+k[113]+k[116]+Polynomial("1")+Polynomial( to_string( ID[14] ) );

    x[75] = x[11]+x[18]+x[21]+x[51]+x[82]+x[85]*k[21]+x[85]+x[92]*k[28]+x[115]*k[51]+x[115]+k[11]+k[18]+k[51]+k[75]+k[82]+k[92]+Polynomial("1")+Polynomial( to_string( ID[15] ) );

    x[114] = x[25]*k[25]+x[25]*k[89]+x[25]+x[28]*k[28]+x[28]*k[92]+x[28]+x[50]*k[50]+x[50]*k[114]+x[50]+x[89]+x[92]+k[25]*k[89]+k[25]+k[28]*k[92]+k[28]+k[50]*k[114]+k[50]+k[89]+k[92]+k[114]+Polynomial( to_string( ID[16] ) );

    x[95] = x[28]*k[28]+x[28]*k[92]+x[28]+x[31]*k[31]+x[31]*k[95]+x[31]+x[53]*k[53]+x[53]*k[117]+x[53]+x[92]+x[117]+k[28]*k[92]+k[28]+k[31]*k[95]+k[31]+k[53]*k[117]+k[53]+k[92]+k[95]+k[117]+Polynomial( to_string( ID[17] ) );

    x[127] = x[6]+x[9]+x[39]+x[63]+x[70]+x[73]*k[9]+x[73]+x[80]*k[16]+x[103]*k[39]+x[103]+k[6]+k[39]+k[63]+k[70]+k[80]+k[127]+Polynomial("1")+Polynomial( to_string( ID[18] ) );

    x[77] = x[13]+x[23]+x[53]+x[87]*k[23]+x[87]+x[117]*k[53]+x[117]+k[13]+k[20]+k[23]+k[53]+k[77]+k[84]+Polynomial("1")+Polynomial( to_string( ID[19] ) );

    for ( int i = 0; i < 128; i++ )
    {
        hatf.setS( 192 + i, x[i] ); 
        hatf.setS( 64 + i, k[i] ); 
    }
}

vector<double> getBiasAscon( int ROUND, int ORDER, const vector<int> & D, int MODE )
{
    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 2 );
    assert ( D.size() - ORDER == 0 );

    hatf.init( D );

    hatf.RunPA( 0 ); 
    hatf.Sub( 0, MODE, 0, 320/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.Sub( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPN();
    hatf.Sub( ROUND, MODE, 0, 320/THREADNUM, true );

    vector<double> res (64, 0);

    for ( int value = 0; value < ( 1 << ( 2 * D.size() ) ); value++ )
    {
        printSteps( value, 1 << ( 2 * D.size() ) );

        hatf.resetBM();
        auto valueMap = hatf.getValueMap( D, value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            res[i] += bias;
        }
    }
    cout << endl;

    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / ( ( 1 << 2 * D.size() ) );
    }
    return res;
}

vector<double> ascon_applicatoin1_round4( int MODE )
{
    int ROUND = 4;
    int ORDER = 1;
    vector<int> D { 0 };

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 2 );

    assert ( D.size() - ORDER == 0 );

    hatf.init( D );

    hatf.RunPA( 0 ); 
    hatf.Sub( 0, MODE, 0, 320/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.Sub( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPN();
    hatf.Sub( ROUND, MODE, 0, 320/THREADNUM, true );

    vector<double> res (64, 0);

    for ( int value = 0; value < ( 1 << ( 2 * D.size() ) ); value++ )
    {
        hatf.resetBM();
        auto valueMap = hatf.getValueMap( D, value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            res[i] += bias;
        }
    }

    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / ( ( 1 << 2 * D.size() ) );
    }
    return res;
}

void setCondition( HATF & hatf, int d  )
{
    Polynomial x[128];
    Polynomial k[128];
    for ( int i = 0; i < 128; i++ )
    {
        x[i] = hatf.getS( 192 + i );
        k[i] = hatf.getS( 64 + i );
    }

    //k[0]= Polynomial( "" );
    //k[9]= Polynomial( "" );
    //k[64]= Polynomial( "" );
    int ID[7] = { 0 };
    for ( int i = 0; i < 7; i++ )
        ID[i] = d >> ( 6 - i ) & 0x1;

    k[64] = Polynomial( to_string( ID[0] ) ) + Polynomial( "0" );

    k[0] = Polynomial( to_string( ID[1] ) ) + Polynomial( "0" );

    x[86]=Polynomial( to_string( ID[2] ) ) + x[19]*k[19]+x[19]*k[83]+x[19]+x[22]*k[22]+x[22]*k[86]+x[22]+x[44]*k[44]+x[44]*k[108]+x[44]+x[83]+x[108]+k[19]*k[83]+k[19]+k[22]*k[86]+k[22]+k[44]*k[108]+k[44]+k[83]+k[86]+k[108];

    x[23]=Polynomial( to_string( ID[3] ) ) + x[47]+x[54]+x[57]+x[87]*k[23]+x[87]+x[111]+x[118]+x[121]*k[57]+x[121]+k[23]+k[47]+k[54]+k[57]+k[111]+k[118];

    x[95]=Polynomial( to_string( ID[4] ) ) + x[28]*k[28]+x[28]*k[92]+x[28]+x[31]*k[31]+x[31]*k[95]+x[31]+x[53]*k[53]+x[53]*k[117]+x[53]+x[92]+x[117]+k[28]*k[92]+k[28]+k[31]*k[95]+k[31]+k[53]*k[117]+k[53]+k[92]+k[95]+k[117];

    x[67]=Polynomial( to_string( ID[5] ) ) + x[3]*k[3]+x[3]*k[67]+x[3]+x[25]*k[25]+x[25]*k[89]+x[25]+x[89]+k[3]*k[67]+k[3]+k[25]*k[89]+k[25]+k[67]+k[89]+Polynomial("1");

    x[2]=Polynomial( to_string( ID[6] ) ) + x[12]+x[42]+x[66]+x[76]*k[12]+x[76]+x[83]*k[19]+x[106]*k[42]+x[106]+k[2]+k[9]+k[12]+k[42]+k[66]+k[73]+k[83];

    for ( int i = 0; i < 128; i++ )
    {
        hatf.setS( 192 + i, x[i] );
        hatf.setS( 64 + i, k[i] );
    }
}

vector<double> ascon_applicatoin1_round5( int MODE )
{
    int ROUND = 5;
    int ORDER = 1;
    vector<int> D { 0 };


    vector<double> res (64, 0);

    for ( int value = 0; value < 128; value++ )
    {
        printSteps( value, 128 );

        AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
        assert ( D.size() - ORDER == 0 );
        hatf.init( D );
        setCondition( hatf, value );

        hatf.RunPA( 0 ); 
        hatf.RunRF( 1 ); 
        hatf.RunRF( 2 ); 
        hatf.SubBias( 2, MODE, 0, 320/THREADNUM, false );

        for ( int r = 3; r < ROUND; r++ )
        {
            hatf.RunRF( r ); 
            hatf.SubBias( r, MODE, 0, 320/THREADNUM, false );
        }

        hatf.RunSboxPNLast();
        hatf.SubBias( ROUND, MODE, 0, 64/THREADNUM, true );

        //auto valueMap = hatf.getValueMap( D, -1 );
        //auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            //f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            //if ( i == 47 )
            //    cout << bias << endl;
            res[i] += bias;
        }
    }

    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / 128;
    }
    return res;
}

vector<double> ascon_applicatoin2( int MODE )
{
    int ROUND = 4;
    int ORDER = 2;
    vector<int> D { 0, 60 };


    vector<double> res (64, 0);

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
    assert ( D.size() - ORDER == 0 );
    hatf.init( D );
    //setCondition( hatf, value );

    hatf.RunPA( 0 ); 
    hatf.Sub( 0, MODE, 0, 320/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.Sub( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPNLast();
    hatf.Sub( ROUND, MODE, 0, 64/THREADNUM, true );

    for ( int value = 0; value < ( 1 << 2 * ORDER ); value++ ) 
    {
        hatf.resetBM();
        
        auto valueMap = hatf.getValueMap( D, value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            //if ( i == 47 )
            //    cout << bias << endl;
            res[i] += bias;
        }
    }

    cout << endl;
    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / ( 1 << 2 * ORDER);
    }
    return res;
}

vector<double> ascon_applicatoin3( int MODE )
{
    int ROUND = 5;
    int ORDER = 2;
    vector<int> D { 0, 3 };


    vector<double> res (64, 0);

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
    assert ( D.size() - ORDER == 0 );
    hatf.init( D );
    //setCondition( hatf, value );

    hatf.RunPA( 0 ); 
    hatf.Sub( 0, MODE, 0, 320/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.Sub( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPNLast();
    hatf.Sub( ROUND, MODE, 0, 64/THREADNUM, true );

    for ( int value = 0; value < ( 1 << 2 * ORDER ); value++ ) 
    {
        printSteps( value, 1 << 2 * ORDER );

        hatf.resetBM();

        auto valueMap = hatf.getValueMap( D, value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            //if ( i == 47 )
            //    cout << bias << endl;
            res[i] += bias;
        }
    }

    cout << endl;
    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / ( 1 << 2 * ORDER );
    }
    return res;
}

vector<double> ascon_applicatoin4( int MODE )
{
    int ROUND = 5;
    int ORDER = 8;
    vector<int> D { 0, 8, 9, 13, 14, 26, 43, 60 };


    vector<double> res (64, 0);

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
    assert ( D.size() - ORDER == 0 );
    hatf.init( D );
    //setCondition( hatf, value );

    hatf.RunPA( 0 ); 
    hatf.Sub( 0, MODE, 0, 320/THREADNUM, false );

    for ( int r = 1; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.Sub( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPNLast();
    hatf.Sub( ROUND, MODE, 0, 64/THREADNUM, true );

    for ( int value = 0; value < ( 1 << 2 * ORDER ); value++ ) 
    {
        printSteps( value, 1 << 2 * ORDER );

        hatf.resetBM();

        auto valueMap = hatf.getValueMap( D, value );
        auto finalMap = hatf.CalBias( MODE, valueMap );

        for ( int i = 0; i < 64; i++ )
        {
            auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
            f0 = Subs( f0, finalMap );
            double bias = (f0).calBiasLinear( hatf._BM );
            //if ( i == 47 )
            //    cout << bias << endl;
            res[i] += bias;
        }
    }

    cout << endl;
    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i] / ( 1 << 2 * ORDER );
    }
    return res;
}

vector<double> ascon_applicatoin5( int MODE )
{
    int ROUND = 5;
    int ORDER = 2;
    vector<int> D { 0, 9 };


    vector<double> res (64, 0);

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
    assert ( D.size() - ORDER == 0 );
    hatf.init( D );
    setCondition09( hatf, 0 );

    hatf.RunPA( 0 ); 
    hatf.RunRF( 1 ); 
    hatf.RunRF( 2 ); 

    hatf.SubBias( 2, MODE, 0, 320/THREADNUM, false );

    for ( int r = 3; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.SubBias( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPNLast();
    hatf.SubBias( ROUND, MODE, 0, 64/THREADNUM, true );

    for ( int i = 0; i < 64; i++ )
    {
        auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
        double bias = (f0).calBiasLinear( hatf._BM );
        res[i] += bias;
    }

    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i];
    }
    return res;
}

vector<double> ascon_applicatoin6( int MODE )
{
    int ROUND = 6;
    int ORDER = 3;
    vector<int> D { 0, 30, 61 };


    vector<double> res (64, 0);

    AsconHATF hatf ( ORDER, 320, ROUND, ROUND + 1 );
    assert ( D.size() - ORDER == 0 );
    hatf.init( D );
    setCondition03061( hatf, 0 );

    hatf.RunPA( 0 ); 
    hatf.RunRF( 1 ); 
    hatf.RunRF( 2 ); 

    hatf.SubBias( 2, MODE, 0, 320/THREADNUM, false );

    for ( int r = 3; r < ROUND; r++ )
    {
        hatf.RunRF( r ); 
        hatf.SubBias( r, MODE, 0, 320/THREADNUM, false );
    }

    hatf.RunSboxPNLast();
    hatf.SubBias( ROUND, MODE, 0, 64/THREADNUM, true );

    for ( int i = 0; i < 64; i++ )
    {
        auto f0 = ( hatf.getS(i) ).getCoef( hatf._x[ (1 << ORDER) - 1 ] );
        double bias = (f0).calBiasLinear( hatf._BM );
        res[i] += bias;
    }

    for ( int i = 0; i < 64; i++ )
    {
        res[i] = res[i];
    }
    return res;
}

