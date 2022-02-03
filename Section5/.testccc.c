
    #include<stdio.h>
    #include<math.h>
    #define BIT 3

    int main()
    {
        int s = 0;
        int y = 0;
        for ( unsigned long long i = 0; i < ( 1L << BIT ); i++ )
        {
            int x[BIT] = { 0 };
            for ( int j = 0; j < BIT; j++ )
                x[j] = i >> ( BIT - 1 - j ) & 0x1;
    y = x[0]*x[2]^x[0]*x[1];

            if ( y == 0 )
                s ++;
        }

        printf ( "%d\n",  s ); 
    }  
    