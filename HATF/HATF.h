#ifndef __HATF_H__
#define __HATF_H__

#include"polynomial.h"
#include<vector>
#include<map>
using namespace std;

extern int BACK;

extern int THREADNUM;

const int CLASSIC = 0;
const int DISJOINT = 1;

class HATF
{
    public:
        HATF( int ORDER, int size, int ROUND, int level );
        ~HATF();

        void setS( int i, const Polynomial & s );

        Polynomial getX( int i );
        Polynomial getS( int i );

        int getOrder();

        double getBias( const string & s ) 
        { 
            if ( s == "" ) 
                return 0.5;
            else if ( s == "1" )
                return -0.5;
            else if ( _BM.find( s ) != _BM.end() )
                return _BM[s]; 
            else if ( _BM.find( ( Polynomial( s ) + Polynomial("1") ).getPoly() ) != _BM.end() )
                return -1 * _BM[ ( Polynomial(s ) + Polynomial( "1" ) ).getPoly() ];
            else
            {
                cerr << endl;
                cerr << "No element : " << s << endl;
                for ( auto const & it : _BM )
                    if ( it.first == s )
                        cout << it.first << endl;
                exit( EXIT_FAILURE );
            }
        }
        void setBias( const string & s, double v ) { _BM[s] = v; }

        virtual void init (const vector<int> & D ) = 0;
        Polynomial getNewVar( int r, int pos, int order );
        void Sub( int R, int mode, int START, int END, bool flag = false );
        void SubBias( int R, int mode, int START, int END, bool flag = false );

        Polynomial createVar( string x, int index = -1 )
        {
            if ( index < 0 )
                return Polynomial( x );
            else
                return Polynomial( x + to_string( index ) );
        }

        map<string, string>  CalBias( int mode, const map<string, string> & valueMap );

        virtual map<string, string> getValueMap( const vector<int> & D, int value ) = 0;
        virtual void resetBM() = 0;

        const map<string, double> & getBM() const
        {
            return _BM;
        }

        vector<Polynomial> _x;
        vector<Polynomial> _S;
        int _ORDER;
        map<string, double> _BM;
        int _SIZE;
        int _ROUND;

        map<string, string> ** _TM;
        int _LEVEL;
};


void WorkerQuadratic( int R, int i, HATF & hatf, Polynomial & localS, map<string, string> & localTM, int mode, bool flag );

void WorkerBias( int R, int pos, const map< string, string> & TM, 
        const map<string,double> & BM, 
        map<string, double> & localBM, 
        const map<string, string> & valueMap,
        map<string, string> & localVMap, int mode );

Polynomial Subs( const Polynomial& p, const map< string, string > & MAP );

void WorkerQuadraticBias( int R, int i, HATF & hatf, Polynomial & localS, map<string, string> & localTM, const map<string, double> & BM, map<string, double> & localBM, int mode, bool flag );
#endif
