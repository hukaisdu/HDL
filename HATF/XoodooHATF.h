#ifndef __XOODOOHATF_H__
#define __XOODOOHATF_H__

#include"polynomial.h"
#include"HATF.h"
#include<vector>
#include<map>

using namespace std;

class XoodooHATF: public HATF
{
    public: 
        XoodooHATF( int ORDER, int size, int ROUND,  int level );

        map<string, string> getValueMap( const vector<int> & D, int value ) 
        {
            map<string, string> M;
            return M;
        }
        void initWithString(string diff[3]);

        void initV( unsigned long diff0,
                    unsigned long diff1,
                    unsigned long diff2,
                    unsigned long diff3,
                    unsigned long diff4,
                    unsigned long diff5
                    );
        void initFromMiddle( vector<int> & D, int v, bool flag );



        map<string, string> getValueMapV0( int value );
        map<string, string> getValueMapV1( int value );
        map<string, string> getValueMapV2( int value );
        map<string, string> getValueMapV3( );
        map<string, string> getValueMapV4( );

        void RunPE(); 
        void setCondition( vector<int> & D, vector<int> & V );
        void initVec(int UV[], int U[], int V[] );
        void initUVW(int UVW[], int UV[], int UW[], int VW[], int U[], int V[], int W[] );
        void initDim(int ** U );
        void initRelated( const vector<int> & D );

        void resetBM();
        void RunChiP(int R, int ROUND );

        void initKey( const vector<int> & D, int k );
        void init( const vector<int> & D );
        void RunRound( int R, int ROUND );
        void RunRoundLast(int R, int ROUND );
        void RunChi();
};

void Enc( Polynomial *** A, int ROUND );

#endif
