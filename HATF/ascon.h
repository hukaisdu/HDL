#ifndef __ASCON_H__
#define __ASCON_H__
#include<vector>
using std::vector;
vector<double> getBiasAscon( int ROUND, int ORDER, const vector<int> & D, int MODE );
vector<double> ascon_applicatoin1_round4( int MODE );
vector<double> ascon_applicatoin1_round5( int MODE );
vector<double> ascon_applicatoin2( int MODE );
vector<double> ascon_applicatoin3( int MODE );
vector<double> ascon_applicatoin4( int MODE );
vector<double> ascon_applicatoin5( int MODE );
vector<double> ascon_applicatoin6( int MODE );

#endif
