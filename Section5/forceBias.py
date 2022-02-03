from __future__ import division
import os
import sys



def forceBias( poly ):
    S = poly.split( '+' )

    V = set()
    for ss in S:
        sss = ss.replace( '(', '' ).replace( ')', '' ).strip()
        ssss = sss.split( '*' )
        for x in ssss:
            V.add( x )

    VV = list ( V )

    VV.sort()

    poly = poly.replace( '(', '' ).replace( ')', '').strip() 

    XOR = poly.split( '+' )
    for indexor, xor in enumerate( XOR ):
        AND = xor.split ( '*' )
        for indexand, a in enumerate( AND ):
            for i in range( len( VV ) ):
                a = a.strip()
                VV[i] = VV[i].strip()
                #print ( a , VV[i] )
                if a.strip() == VV[i].strip():
                    AND[indexand] = 'x[%d]' % i
                    
        XOR[indexor] = '*'.join( AND )
    poly = '^'.join( XOR )

    '''
    for i in range( len( VV ) ):
        poly = poly.replace( VV[i], 'x[%d]'%i )

    poly = poly.replace( '+', '^') 
    '''

    return poly, len( VV )

    '''
    dim = len( VV )
    x = [0] * dim
    total = 0
    for i in range( 1 << dim ):
        #print ( i )
        for j in range( dim ):
            x[j] = i >> ( dim - 1 - j ) & 0x1
        total += 1 - eval( poly )

    return total / ( 1 << dim ) - 0.5 
    '''

def writeC( poly ):
    if poly == '1':
        return 0.5

    f = open( ".testccc.c", 'w' )

    s0 = r'''
    #include<stdio.h>
    #include<math.h>
    '''

    s1 = r'''
    int main()
    {
        int s = 0;
        int y = 0;
        for ( unsigned long long i = 0; i < ( 1L << BIT ); i++ )
        {
            int x[BIT] = { 0 };
            for ( int j = 0; j < BIT; j++ )
                x[j] = i >> ( BIT - 1 - j ) & 0x1;
    '''

    s2 = r'''
            if ( y == 0 )
                s ++;
        }

        printf ( "%d\n",  s ); 
    }  
    '''
    expression, dim  = forceBias( poly )

    #print ( dim )

    f.write( s0 )
    f.write( '#define BIT {}\n'.format( dim ) )
    f.write( s1 )
    f.write( 'y = {};\n'.format( expression ) )
    f.write( s2 )
    f.close()

    os.system( 'gcc .testccc.c -o .testccc -O3 -lm' )

    os.system( './.testccc > .testcccResult' )

    f = open( '.testcccResult', 'r' )

    a = int ( f.readline() )

    f.close()

    #print ( a )
    return a / ( 1 << dim ) - 0.5


if __name__ == '__main__':

    if len ( sys.argv ) > 1:
        poly = sys.argv[1]
    else:
        poly = 'k(63)*k(69)*T0(2399)*T2(2159) + k(69)*k(76)*T0(2399)*T3(479)*T3(1119) + T0(2399)*T2(2159)*T3(1119)*T3(2399)*T3(3519) + T0(2399)*T2(2159)*T3(1119)*T3(2399) + T0(2399)*T2(2159)*T3(1119)*T3(3519)*T3(5279) + T0(2399)*T2(2159)*T3(1119)*T3(3519) + T0(2399)*T2(2159)*T3(1119)*T3(5279) + T0(2399)*T2(2159)*T3(1119) + T0(2399)*T2(2159)*T3(3519) + T0(2399)*T3(1119)*T3(2399)*T3(4799) + T0(2399)*T3(1119)*T3(4799)*T3(5279) + T0(2399)*T3(1119)*T3(4799) + T0(2399)*T3(4799)'



    poly = poly.strip()



    a = writeC( poly )
    print ( a )






