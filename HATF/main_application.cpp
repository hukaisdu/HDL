#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include"split.h"
#include"HATF.h"
#include"ascon.h"
#include"xoodoo.h"
#include<iomanip>
#include<cassert>
#include<cstdio>

using namespace std;

int THREADNUM;
int MODE = DISJOINT;

enum CIPHER { ascon, xoodyak, xoodoo };

constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};

void printSetting( CIPHER cipher, int ROUND, int ORDER, const vector<int> & D, int THREAD )
{
    cout << "CIPHER: ";
    if ( cipher == ascon )
        cout << "Ascon";
    else if ( cipher == xoodyak )
        cout << "Xoodyak";
    else 
        cout << "Xoodoo";
    cout << "\t";

    cout << "ROUND: " << ROUND << "\t";
    cout << "ORDER: " << ORDER << "\t";
    cout << "THREAD: " << THREAD << "\t";
    cout << endl;
    cout << "Input Difference: "; 
    for ( int i = 0; i < ( D.size() - 1 ); i++ )
        cout << D[i] << ", ";
    cout << D[D.size() - 1];
    cout << endl;
}

int main( int argc, char * argv[] )
{
    CIPHER cipher;
    int APP;

    /* default setting */
    cipher = ascon;
    APP = 2;
    THREADNUM = 8;
    /*-------- */

    string help ("Usage: ./main --cipher [ascon|xoodyak] --application [1|2|3|4|5|6] --thread t" );

    if ( argv[1] == string ( "--help" ) ) 
    {
        cout << help << endl;
        exit( EXIT_SUCCESS );
    }
    else
    {
        for ( int i = 0; i < argc; i++ )
        {
            if ( argv[i] == string( "--cipher" ) )
            {
                if ( argv[i+1] == string ( "ascon" ) )
                    cipher = ascon;
                else if ( argv[i+1] == string ( "xoodyak" ) )
                    cipher = xoodyak;
                else if ( argv[i+1] == string ( "xoodoo" ) )
                    cipher = xoodoo;
                else
                {
                    cerr << "No support cipher: " << argv[1] << endl;
                    exit( EXIT_FAILURE );
                }
                i++;
            }
            if ( argv[i] == string( "--application" ) )
            {
                APP = stoi( argv[i+1] );
                i++;
            }
            if ( argv[i] == string( "--thread" ) )
            {
                THREADNUM = stoi( argv[i+1] );

                if ( 64 % THREADNUM != 0 )
                {
                    cerr << "Thread number can only be a integer factor of 64!" << endl; 
                    exit( EXIT_FAILURE );
                }

                i++;
            }
        }
    }

    if ( cipher == ascon )
    {
        if ( APP == 1 )
        {
            cout << "Application 1: the bias of 4- and 5-round Ascon with Delta(0) " << endl;
            cout << endl;
            cout << "4-round result with partitioning the space into 4 subspace " << endl;
            auto res4r = ascon_applicatoin1_round4( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
            cout << "5-round result with partitioning the space into 128 subspace (it might take a long time) " << endl;
            auto res5r = ascon_applicatoin1_round5( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
        }
        else if ( APP == 2 )
        {
            cout << "Application 2: the bias of 4-round Ascon with Delta(0, 60) " << endl;
            auto res4r = ascon_applicatoin2( MODE );
            cout << endl;
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 3 )
        {
            cout << "Application 3: the bias of 5-round Ascon with Delta(0, 3) " << endl;
            auto res5r = ascon_applicatoin3( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 4 )
        {
            cout << "Application 4: the bias of 5-round Ascon with Delta(0, 8, 9, 13, 14, 26, 43, 60) " << endl;
            auto res5r = ascon_applicatoin4( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 5 )
        {
            cout << "Application 5: the bias of 5-round Ascon with Delta(0, 9) with conditions" << endl;
            auto res5r = ascon_applicatoin5( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 6 )
        {
            cout << "Application 6: the bias of 5-round Ascon with Delta(0, 30, 61) with conditions" << endl;
            auto res5r = ascon_applicatoin6( MODE );
            for ( int i = 0; i < 16; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
    } 
    else if ( cipher == xoodyak )
    {
        if ( APP == 1 )
        {
            cout << "Application 1: the bias of 4-round Xoodyak with (e128, e256) and (e0, e256)" << endl;
            cout << endl;
            cout << "4-round result with difference (e128, e256) with partitioning the space into 16 subspace " << endl;
            auto res4r = xoodyak_application1_e128e256( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r[ 4 * i + j ] << "\t";
                cout << endl;
            }

            cout << endl;
            cout << "4-round result with difference (e0, e256) with partitioning the space into 16 subspace " << endl;

            auto res4r1 = xoodyak_application1_e0e256( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r1[ 4 * i + j ] << "\t";
                cout << endl;
            }
        }
        else if ( APP == 2 )
        {
            cout << "Application 2: the bias of 4-round Xoodyak with (e128,e256, e175, e303) " << endl;
            auto res4r = xoodyak_application2( MODE );
            cout << endl;
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 3 )
        {
            cout << "Application 2: the bias of 4-round Xoodyak with 4-th order difference related-key (0, 71, 79, 127)" << endl;
            auto res5r = xoodyak_application3( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 4 )
        {
            cout << "Application 4: the bias of 5-round Xoodyak with 2nd order difference (0, 34) " << endl;
            auto res5r = xoodyak_application4( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 5 )
        {
            cout << "Application 5: the bias of 4- and 5-round Xoodoo" << endl;
            cout << "Bias of 4-round Xoodoo with difference Delta(0, 20)" << endl;
            auto res4r = xoodyak_application5_4round( MODE );
            for ( int i = 0; i < 96; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res4r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;

            cout << "Bias of 5-round Xoodoo with difference Delta(0, 13, 14)" << endl;
            auto res5r = xoodyak_application5_5round( MODE );
            for ( int i = 0; i < 96; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 6 )
        {
            cout << "Application 6: the bias of 4-round Xoodyak with Delta(0, 72) with conditions" << endl;
            auto res5r = xoodyak_application6( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                    cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                cout << endl;
            }
            cout << endl;
        }
        else if ( APP == 7 )
        {
            cout << "Application 7: the bias of 5-round Xoodyak with Delta(0, 9, 36) with conditions" << endl;
            auto res5r = xoodyak_application7( MODE );
            for ( int i = 0; i < 48; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                {
                    //cout << 4 * i + j << "\t" << fixed << setprecision( max_precision ) << res5r[ 4 * i + j ] << "\t";
                    cout << 4 * i + j << "\t" << res5r[ 4 * i + j ] << "\t";
                }
                cout << endl;
            }
            cout << endl;
        }
    } 

}

