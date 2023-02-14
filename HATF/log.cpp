#include<iostream>
#include"HATF.h"
#include"log.h"

using namespace std;

void printSteps( int cur, int tol )
{
    char s[52] = { 0 };

    int curx = int ( ( (cur+1)*50.0 / tol ) );

    for ( int i = 0; i < curx; i++ )
        s[i] = '#';
    s[curx] = 0;
    cout << '[' << s << ']' << setprecision(3) << ( curx*1.0 /50 * 100 ) << "%" << '\r' << flush; 
}

void log( string folder, int ROUND, int ORDER, int mode, const vector<int> & D, const vector<double>  & vec )
{
    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};
    string name ( folder + string( "/LOG_" ) );
    name += to_string( ROUND );

    name += string( "_" );
    name += to_string( ORDER );
    name += string( "_" );

    if ( mode == CLASSIC )
        name += string( "CLASSIC_" );
    else
        name += string( "DISJOINT_" );

    for ( auto it : D )
    {
        name += to_string( it );
        name += "_";
    }

    name += string( ".log" );

    ofstream os;
    os.open( name, ios::out );

    for ( size_t i = 0; i < vec.size(); i++ )
    {
        //os << i << "\t" << vec[i] << endl;
         os << i << ":\t";
         os << fixed << setprecision( max_precision ) << vec[i] << endl;
   }

    os.close();
}

