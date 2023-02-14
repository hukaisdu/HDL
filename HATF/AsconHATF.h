#ifndef __AsconHATF_H__
#define __AsconHATF_H__

#include"polynomial.h"
#include"HATF.h"
#include<vector>
#include<map>

using namespace std;

extern int THREADNUM;

class AsconHATF:public HATF
{
    public:
        AsconHATF( int ORDER, int size, int rd, int level = 0 );

        void init( const vector<int> & D );

        void initValue( vector<int> & D, int v  );

        void initOne( vector<int> & D  );
        void initMid( vector<int> & D  );
        void initVec( vector< vector< int> > & D, vector<int> & row  );

        void RunSboxAC( int conIndex );
        void RunMatrix();
        void RunFirstSboxAC( int conIndex );
        void RunMatrixS( int conIndex );

        void RunPA( int conIndex );
        void RunPB();
        void RunRF( int conIndex );
        void initUV( int **UV, int rr  );
        void initValueMap( vector<int> & D, const map< string, string > & MAP );


        void RunSboxPN();
        void RunSboxPNLast();
        void resetBM();
        //void Sub( int R, int START = 0, int END = 5, bool flag = false );
        //void SubNoBias( int R, int START = 0, int END = 5, bool flag = false );
        //void Sub_1( int R );
        //void Sub_1_Bias( int R );
        map<string, string> getValueMap( const vector<int> & D, int value );
        map<string, string> getValueMap1( int value );

};

#endif
