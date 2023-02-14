#include"polynomial.h"
#include<iostream>
#include<cstdlib>

using namespace std;

void printMatrix( int ** M, int dim )
{
    for ( int i = 0; i < dim; i++ )
    {
        for ( int j = 0; j < dim; j++ )
            cout << M[i][j] << "\t";
        cout << endl;
    }
}


void getMinor( int ** M, int dim, int row, int col, int** MM )
{
    if ( dim == 1 )
    {
        cerr << "Dimensioan cannot be 1 " << endl;
        exit( EXIT_FAILURE );
    }

    int myrow = 0;
    for ( int i = 0; i < dim; i++ )
    {
        if ( i != row )
        {
            int mycol = 0;
            for ( int j = 0; j < dim; j++ )
            {
                if ( j != col )
                {
                    MM[myrow][mycol] = M[i][j];

                    mycol += 1;

                    //cout << myrow << " " << mycol << endl;
                }
            }

            myrow += 1;
        }
    }

    /*
    for ( int i = 0; i < dim - 1; i++ )
    {
        for ( int j = 0; j < dim - 1; j++ )
            cout << MM[i][j] << " ";
        cout << endl;
    }

    cout << endl;
    */

}

inline void swapRow( int ** M, int dim, int row1, int row2 )
{
    for ( int i = 0; i < dim; i++ )
    {
        int tmp = M[row1][i]; 
        M[row1][i] = M[row2][i];
        M[row2][i] = tmp;
    }
}

inline void xorRow( int * M, int * OM, int dim)
{
    for ( int i = 0; i < dim; i++ )
        M[i] = M[i] ^ OM[i];
}


int detPerm ( int ** M, int dim )
{
    if ( dim == 1 )
        return M[0][0];
    if ( dim == 2 )
        return M[0][0] * M[1][1] ^ M[0][1] * M[1][0];

    for ( int col = 0; col < dim; col++ )
    {
        for ( int row = col; row < dim; row++ )
        {
            if ( M[row][col] == 1 )
            {
                if ( row == col )
                    ;
                else
                    swapRow( M, dim, col, row );

                for ( int row1 = row + 1; row1 < dim; row1++ )
                {
                    if ( M[row1][col] ==  1 )
                    {
                        for ( int col1 = col; col1 < dim; col1 ++ ) 
                            M[row1][col1] ^= M[row][col1];
                    }
                }
            }
        }
    }

    for ( int i = 0; i < dim; i++ )
        if ( M[i][i] == 0 )
            return 0;
    return 1;
}


int det ( int ** M, int dim )
{
    if ( dim == 1 )
        return M[0][0];
    if ( dim == 2 )
        return M[0][0] * M[1][1] ^ M[0][1] * M[1][0];

    int res = 0;

    int ** MM = new int*[dim - 1];
    for ( int m = 0; m < dim - 1; m++ )
        MM[m] = new int[ dim - 1 ];

    for ( int i = 0; i < dim; i++ )
    {
        //cout << i << endl;
        if ( M[0][i] == 1 )
        {
            getMinor( M, dim, 0, i, MM );
            res ^= det( MM, dim - 1 );
        }
    }

    for ( int m = 0; m < dim - 1; m++ )
        delete [] MM[m];
    delete [] MM;

    return res;
}

void invertPerm( int ** M, int dim, int ** IM )
{
    if ( dim == 1 )
        IM[0][0] = M[0][0];
    else
    {
        for ( int i = 0; i < dim; i++ )
            for ( int j = 0; j < dim; j++ )
            {
                if ( i == j )
                    IM[i][j] = 1;
                else
                    IM[i][j] = 0;
            }
                    

        for ( int col = 0; col < dim; col++ )
        {
            for ( int row = col; row < dim; row++ )
            {
                if ( M[row][col] == 1 )
                {
                    if ( row == col )
                        ;
                    else
                    {
                        swapRow( M, dim, col, row );
                        swapRow( IM, dim, col, row );
                    }

                    break;
                }
            }


            for ( int row = col + 1; row < dim; row++ )
            {
                if ( M[row][col] ==  1 )
                {
                    xorRow( M[row], M[col], dim ); 
                    xorRow( IM[row], IM[col], dim ); 
                    /*
                    for ( int col1 = col; col1 < dim; col1 ++ ) 
                    {
                        M[row][col1] ^= M[col][col1];
                        IM[row][col1] ^= IM[col][col1];
                    }
                    */
                }
            }
        }

        // use the diagonal element to zero the above 1s
        for ( int rowcol = 0; rowcol < dim; rowcol++ )
        {
            for ( int aboveRow = 0; aboveRow < rowcol; aboveRow++ )
            {
                if ( M[aboveRow][rowcol] == 1 ) 
                {
                    xorRow( M[aboveRow], M[rowcol], dim ); 
                    xorRow( IM[aboveRow], IM[rowcol], dim ); 
                    /*
                    for ( int col = rowcol; col < dim; col++ )
                    {
                        M[aboveRow][col] ^= M[rowcol][col];
                        IM[aboveRow][col] ^= IM[rowcol][col];
                    }
                    */
                }
            }
        }
    }

}


void invert( int ** M, int dim, int ** IM )
{
    int ** MM = new int*[dim - 1];
    for ( int m = 0; m < dim - 1; m++ )
        MM[m] = new int[ dim - 1 ];

    for ( int i = 0; i < dim; i++ )
        for ( int j = 0; j < dim; j++ )
        {
            //cout << "getMinor" << endl;
            getMinor( M, dim, i, j, MM );

            //cout << "getMinor finish" << endl;

            //cout << "det" << endl;

            //IM[j][i] = det( MM, dim - 1 );
            IM[j][i] = detPerm( MM, dim - 1 );
            //cout << "det finish" << endl;
        }

    for ( int m = 0; m < dim - 1; m++ )
        delete [] MM[m];
    delete [] MM;
}

void mulMatrix( int ** M0, int **M1, int dim, int ** res )
{
    for ( int i = 0; i < dim; i++ )
        for ( int j = 0; j < dim; j++ )
            res[i][j] = 0;

    for ( int i = 0; i < dim; i++ )
        for  ( int j = 0; j < dim; j++ )
            for ( int k = 0; k < dim; k++ )
                res[i][j] ^= M0[i][k] * M1[k][j]; 
}

int getIndex( const vector<Polynomial> & V, const Polynomial & xt )
{
    auto it = find( V.begin(), V.end(), xt );

    if ( it == V.end() )
    {
        cerr << "Not found the element: " << xt << endl;
        exit( EXIT_FAILURE );
    }
    else
    {
        int index = it - V.begin();
        return index;
    }
}

void genI( const vector<Polynomial> & V, 
           const Polynomial & xt, 
           const Polynomial & fu,  
           int dim, int ** E )
{
    for ( int i = 0; i < dim; i++ )
        for ( int j = 0; j < dim; j++ )
        {
            if ( i == j )
                E[i][j] = 1;
            else
                E[i][j] = 0;
        }

    int col = getIndex( V, xt );

    int * C = new int [ dim ]; 
    for ( int i = 0; i < dim; i++ )
        C[i] = 0;

    vector<Polynomial> fuV = fu.getVariables();

    for ( auto const & v : fuV )
        C[ getIndex( V, v ) ] = 1;

    for ( int i = 0; i < dim; i++ )
        E[i][col] = C[i]; 


    //cout << "genI " << endl;
    //printMatrix( E, dim );

    delete [] C;
}

Polynomial Algorithm1( const Polynomial & ff, const vector<Polynomial> & V, int ** IM )
{
    Polynomial f ( ff.getPoly() );
    int dim = V.size();
    //cout << dim << endl;

    // Matrix
    int ** M = new int* [ dim ];
    for ( int i = 0; i < dim; i++ )
        M[i] = new int [dim];

    // Indentity
    int ** E = new int* [ dim ];
    for ( int i = 0; i < dim; i++ )
        E[i] = new int [dim];

    // Temp M
    int ** TM = new int* [ dim ];
    for ( int i = 0; i < dim; i++ )
        TM[i] = new int [dim];

    for ( int i = 0; i < dim; i++ )
        for ( int j = 0; j < dim; j++ )
            if ( i == j ) M[i][j] = 1;
            else M[i][j] = 0;

    Polynomial v = f.pickIndex();

    //int count = 0;
    //cout << f << endl;
    while ( f.sigma( v ) >= 2 ) 
    {
        //cout << count ++ << endl;
        auto fu = f.graded_part(2).getCoef( v );

        //cout << fu << endl;

        auto xt = *fu.getVariables().begin();
        //cout << xt << endl;

        //cout << xt << " " << f.getCoef( v ) <<  endl;

        f = f.substitution( fu, xt ); 

        //cout << f << endl;
        //cout << f.getCoef( v ) <<  endl;

        genI( V, xt, fu, dim, E );
        mulMatrix( E, M, dim, TM ); 

        for ( int i = 0; i < dim; i++ )
            for ( int j = 0; j < dim; j++ )
                M[i][j] = TM[i][j];

        //cout << "first " << endl;
        if ( f.sigma( xt ) >= 2 )
        {
            auto fu = f.graded_part(2).getCoef( xt );
            //auto xt = *fu.getVariables().begin();
            f = f.substitution( fu, v ); 

            genI( V, v, fu, dim, E );
            mulMatrix( E, M, dim, TM ); 

            for ( int i = 0; i < dim; i++ )
                for ( int j = 0; j < dim; j++ )
                    M[i][j] = TM[i][j];

        }

        //cout << "first finished" << endl;
        //cout << "Pick " << endl;
        v = f.pickIndex();
        //cout << f << " " << v << endl;
        //cout << "Pick finish " << endl;
    }


    //cout << "Invert" << endl;
    invertPerm( M, dim, IM ); 

    for ( int i = 0; i < dim; i++ )
        delete [] M[i];
    delete [] M;

    for ( int i = 0; i < dim; i++ )
        delete [] E[i];
    delete [] E;

    for ( int i = 0; i < dim; i++ )
        delete [] TM[i];
    delete [] TM;

    //cout << "Finish" << endl;

    return f;
}

vector<Polynomial> mulMatrixV( const vector<Polynomial> & V, int ** M, int dim )
{
    vector<Polynomial> VV;
    for ( int i = 0; i < dim; i++ )
    {
        Polynomial p ( "" );
        for ( int j = 0; j < dim; j++ )
            if ( M[j][i] == 1 )
                p = p + V[j];
        VV.emplace_back( p );
    }

    return VV;
}

double calBiasIso( double B[2], int label )
{
    // label is 0
    double bias = 0;
    if ( label == 0 )
    {
        bias = ( 0.5 + B[0] ) * ( 0.5 + B[1] ) + 
               ( 0.5 + B[0] ) * ( 0.5 - B[1] ) + 
               ( 0.5 - B[0] ) * ( 0.5 + B[1] ); 
    }
    else if ( label == 1 )
    {
        bias = ( 0.5 + B[0] ) * ( 0.5 + B[1] ) + 
               ( 0.5 + B[0] ) * ( 0.5 - B[1] ) + 
               ( 0.5 - B[0] ) * ( 0.5 - B[1] ); 
    }
    else if ( label == 2 )
    {
        bias = ( 0.5 + B[0] ) * ( 0.5 + B[1] ) + 
               ( 0.5 - B[0] ) * ( 0.5 + B[1] ) + 
               ( 0.5 - B[0] ) * ( 0.5 - B[1] ); 
    }
    else
    {
        bias = ( 0.5 + B[0] ) * ( 0.5 + B[1] );
    }

    return bias - 0.5;
}

double calBiasDisjoint( const Polynomial & f, map<string, double> & T ) 
{
    auto Qd = f.graded_part(2).getMonomials();
    auto M = f.graded_part(1).getMonomials();

    vector<double> BIAS; 

    Polynomial fff ( "" );

    for ( auto const & qd : Qd )
    {
        Polynomial ff ( qd.getPoly() );

        auto V = qd.getVariables();

        double B[2] = { T[ V[0].getPoly() ], T [ V[1].getPoly() ] };

        //for ( auto const & v : V )
        int label = 0;
        for ( int i = 0; i < 2; i++ )
        {
            if ( find( M.begin(), M.end(), V[i] ) != M.end() )
            {
                ff = ff + V[i];
                if ( i == 0 )
                    label ^= 0x1;
                else
                    label ^= 0x2;
            }
        }

        //cout << ff << " " <<  label << endl;

        BIAS.emplace_back( calBiasIso( B, label ) );

        fff = fff + ff;
    }

    auto L = f + fff;

    if ( L.getPoly() == string("") )
        ;
    else if ( L.getDegree() == 0 )
        BIAS.emplace_back( -0.5 );
    else
    {
        BIAS.emplace_back( L.calBiasLinear( T ) );
    }

    double bias = 1 << ( BIAS.size() - 1 );

    for ( auto const & b : BIAS )
        bias *= b;


    return bias;
}

double calBiasRandom( const Polynomial & f )
{
    if ( f.getPoly() == "" )
        return 0.5;

    auto V = f.getVariables();
    int dim = V.size();

    int ** M = new int*[dim];
    for ( int m = 0; m < dim; m++ )
        M[m] = new int[ dim ];

    //Polynomial Algorithm1( Polynomial f, const vector<Polynomial> & V, int ** IM )
    //auto pos1 = chrono::steady_clock::now();
    auto g = Algorithm1( f, V, M );
    //auto pos2 = chrono::steady_clock::now();

    //cout << "Pos1 - 2 " << chrono::duration_cast<chrono::nanoseconds>(pos2 - pos1).count() << endl;

    auto VV = g.getVariables();

    map<string, double> T;
    for ( auto const & vv : VV )
        T[ vv.getPoly() ] = 0;

    double bias = calBiasDisjoint( g, T );

    //auto pos3 = chrono::steady_clock::now();

    //cout << "Pos 2 - 3 " << chrono::duration_cast<chrono::nanoseconds>(pos3 - pos2).count() << endl;

    for ( int m = 0; m < dim; m++ )
        delete [] M[m];
    delete [] M;

    return bias;
}





