#ifndef __XOODOO_H__
#define __XOODOO_H__

#include<vector>

using std::vector;

vector<double> getBiasXoodyak( int ROUND, int ORDER, const vector<int> & D, int MODE );
vector<double> getBiasXoodoo( int ROUND, int ORDER, const vector<int> & D, int MODE );
vector<double> xoodyak_application1_e128e256( int MODE );
vector<double> xoodyak_application1_e0e256( int MODE );
vector<double> xoodyak_application2( int MODE );
vector<double> xoodyak_application3( int MODE );
vector<double> xoodyak_application4( int MODE );
vector<double> xoodyak_application5_4round( int MODE );
vector<double> xoodyak_application5_5round( int MODE );
vector<double> xoodyak_application6( int MODE );
vector<double> xoodyak_application7( int MODE );

#endif
