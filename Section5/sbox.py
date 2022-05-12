from sage.all import *
from sage.rings.polynomial.pbori.pbori import *
from sage.rings.polynomial.pbori import *

R = declare_ring( [ Block( 'x', 5 ) ], globals() )

x0 = R( x(0) )
x1 = R( x(1) )
x2 = R( x(2) )
x3 = R( x(3) )
x4 = R( x(4) )
#x4 = x3 + 1

#x4 = x0 + x2 + 1
#x3 = x2  

y0 = x4*x1 + x3 + x2*x1 + x2 + x1*x0 + 1

I = { x0, x1, x0 + x1 } = { x0, x1 } = { x0, x0 + x1 }


y0 - ( y0 / x1 ) * x1

y0 / x1 = 0
y1 / x1 = 0

y1 = x4 + x3*x1 + x2*x1 + x2 + x0 + 1
#y2 = x1

# x0, x2, x3, x4 condition, a set of equations
# equations hold, x1 is elimitaed from y0 and y1

print( y0 )
print( y1 )


#y2 = x4*x3 + x4 + x2 + x1 + 1
#y3 = x4*x0 + x4 + x3*x0 + x3 + x2 + x1 + x0
#y4 = x4*x1 + x4 + x3 + x1*x0 + x1

I = set()

I.add( y0 / x1 ) # coefficient of x1:  x4 + x2 + x0 + 1 = 0
I.add( y1 / x1 ) # coefficient of x1:  x3 + x2 + 1 = 0
#I.add( 1 ) # coefficient of x1:  x3 + x2 + 1 = 0

#I.add( y2 / x0 )
#I.add( y3 / x0 )
#I.add( y4 / x0 )

#print ( y0 )
#print ( y1 )
#print ( y2 )
#print ( y3 )
#print ( y4 )

ID = ideal( list ( I ) )

print( ID )

y0D = y0.reduce( ID )
y1D = y1.reduce( ID )

print( y0D )
print( y1D )


