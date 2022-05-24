#include<iostream>
#include<random>
#include<vector>

using namespace std;

int main()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> disint( 0, 1 );

    vector<int> vec;

    for ( int i = 0; i < 256; i++ )
        vec.push_back( i );
    
    int s = 0;

    for ( int i = 0; i < 64; i++ )
    {
        if ( ( vec[i] | vec[i + 14] | vec[i + 64] | vec[i + 64 + 14] ) == 0 )
            s += 1;
    }

    cout << s << endl;
}
