#ifndef __QUADRATIC_H__
#define __QUADRATIC_H__


Polynomial Algorithm1( const Polynomial & f, const vector<Polynomial> & V, int ** IM );
vector<Polynomial> mulMatrixV( const vector<Polynomial> & V, int ** M, int dim );
void printMatrix( int ** M, int dim );
double calBiasDisjoint( const Polynomial & f, map<string, double> & T );
double calBiasRandom( const Polynomial & f );

#endif
