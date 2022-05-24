from sage.all import *
from sage.rings.polynomial.pbori.pbori import *
from sage.rings.polynomial.pbori import *

R = declare_ring( [ Block( 'x', 5 ) ], globals() )

x0 = R( x(0) )
x1 = R( x(1) )
x2 = R( x(2) )
x3 = R( x(3) )
x4 = R( x(4) )

y0 = x4*x1 + x3 + x2*x1 + x2 + x1*x0 + x1 + x0
y1 = x4*x0 + x4 + x3*x0 + x3 + x2 + x1 + x0



I = set()

I.add( y0 / x1 )

print( y0.subs( { x4 : x2 + x0 } ) )
print( y1.subs( { x4 : x2 + x0 } ) )






