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
//int BACK = 0;
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
    int ROUND;
    int ORDER;
    vector<int> D;


    /* default setting */
    cipher = ascon;
    ROUND = 4;
    ORDER = 2;
    D.push_back( 0 );
    D.push_back( 60 );
    THREADNUM = 8;
    /*-------- */

    string help ("Usage: ./main --cipher [ascon|xoodyak|xoodoo] --round R --order l --difference d0,d1,...,d_{l-1} --thread t" );

    if ( argc == 1 )
    {
        cout << "Default setting: " << endl;
    }
    else
    {
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
                if ( argv[i] == string( "--round" ) )
                {
                    ROUND = stoi( argv[i+1] );
                    i++;
                }
                if ( argv[i] == string( "--order" ) )
                {
                    ORDER = stoi( argv[i+1] );
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
                if ( argv[i] == string( "--difference" ) )
                {
                    D.clear();

                    auto diff = split( argv[i+1], "," );
                    for ( auto const & it : diff )
                        D.push_back( stoi( it ) );

                    if ( D.size() != ORDER )
                    {
                        cout << "Number of differences must be equal to the order!" << endl;
                        exit( EXIT_FAILURE );
                    }
                    i++;
                }
            }

            cout << "User's setting: " << endl;

        }
    }

    printSetting( cipher, ROUND, ORDER, D, THREADNUM );

    if ( cipher == ascon )
    {
        auto res = getBiasAscon( ROUND, ORDER, D, MODE );
        for (int i = 0; i < 64; i++ )
        {
            cout << i << "\t" << fixed << setprecision( max_precision) << res[i] << endl;
        }
    }
    else if ( cipher == xoodyak )
    {
        auto res = getBiasXoodyak( ROUND, ORDER, D, MODE );
        for (int i = 0; i < 192; i++ )
        {
            cout << i << "\t" << fixed << setprecision( max_precision) << res[i] << endl;
        }
    }
    else
    {
        auto res = getBiasXoodoo( ROUND, ORDER, D, MODE );
        for (int i = 0; i < 384; i++ )
        {
            cout << i << "\t" << fixed << setprecision( max_precision) << res[i] << endl;
        }
    }
} 

