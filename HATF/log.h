#ifndef __LOG_H__
#define __LOG_H__

#include<vector>
#include<fstream>
#include<string>
#include<limits>
#include<iomanip>
#include<iostream>
#include"HATF.h"

using namespace std;

void log( string folder, int ROUND, int ORDER, int mode, const vector<int> & D, const vector<double>  & vec );

void printSteps( int cur, int tol );

#endif
