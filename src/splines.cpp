// ------------------- THIS IS WHERE THE BEZIER STUFF WILL BE ------------- //
#include <cmath>
#include <iostream>
#include <vector>
#include "main.h"

using namespace std;
typedef double db;

db bezX(db x1,db x2,db x3,db x4,db t){  // THIS IS THE EQUATION OF A BEZIER's X COORD AT A TIME t GIVEN COEFFICIENTS (DWB COEFFICIENTS)
  return x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4;
}

db bezY(db y1,db y2,db y3,db y4,db t){
  return y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4;
}

void thomas(double* a, double* b, double* c, double* d, int n) {  // THIS IS A COPY PASTE OF WIKIPEDIA BUT IT WORKS! ALSO DWBI ITS LINEAR STUFF IDEK HOW IT WORKS TBH
    /*
    // n is the number of unknowns

    |b0 c0 0 ||x0| |d0|
    |a1 b1 c1||x1|=|d1|
    |0  a2 b2||x2| |d2|

    1st iteration: b0x0 + c0x1 = d0 -> x0 + (c0/b0)x1 = d0/b0 ->

        x0 + g0x1 = r0               where g0 = c0/b0        , r0 = d0/b0

    2nd iteration:     | a1x0 + b1x1   + c1x2 = d1
        from 1st it.: -| a1x0 + a1g0x1        = a1r0
                    -----------------------------
                          (b1 - a1g0)x1 + c1x2 = d1 - a1r0

        x1 + g1x2 = r1               where g1=c1/(b1 - a1g0) , r1 = (d1 - a1r0)/(b1 - a1g0)

    3rd iteration:      | a2x1 + b2x2   = d2
        from 2nd it. : -| a2x1 + a2g1x2 = a2r2
                       -----------------------
                       (b2 - a2g1)x2 = d2 - a2r2
        x2 = r2                      where                     r2 = (d2 - a2r2)/(b2 - a2g1)
    Finally we have a triangular matrix:
    |1  g0 0 ||x0| |r0|
    |0  1  g1||x1|=|r1|
    |0  0  1 ||x2| |r2|

    Condition: ||bi|| > ||ai|| + ||ci||

    in this version the c matrix reused instead of g
    and             the d matrix reused instead of r and x matrices to report results
    Written by Keivan Moradi, 2014
    */
    n--; // since we start from x0 (not x1)
    c[0] /= b[0];
    d[0] /= b[0];

    for (int i = 1; i < n; i++) {
        c[i] /= b[i] - a[i]*c[i-1];
        d[i] = (d[i] - a[i]*d[i-1]) / (b[i] - a[i]*c[i-1]);
    }

    d[n] = (d[n] - a[n]*d[n-1]) / (b[n] - a[n]*c[n-1]);

    for (int i = n; i-- > 0;) {
        d[i] -= c[i]*d[i+1];
    }
}

void resetMatrix(db* bot, db* mid, db* top, int n){
    // ASSIGNMENT OF A TRIDIAGONAL MATRIC (BASICALLY 3 DIAGONALS IN A MATRIX FORMAT WITH EVERYTHING NOT ON THE DIAGONAL AS 0) (DWBI)
    bot[0] = 0;
    bot[n-1] = 2;
    for(int i = 1; i < n-1; i++){
        bot[i] = 1;
    }

    mid[0] = 2;
    mid[n-1] = 7;
    for(int i = 1; i < n-1; i++){
        mid[i] = 4;
    }

    top[n-1] = 0;
    for(int i = 0; i < n-1; i++){
        top[i] = 1;
    }

}
void multiBez(db* x, db* y, int n){

    db bot[n];
    db mid[n];
    db top[n];

    db rX[n];
    db rY[n];

    db p0x[n];
    db p0y[n];

    db p1x[n];
    db p1y[n];

    db p2x[n];
    db p2y[n];

    db p3x[n];
    db p3y[n];

    resetMatrix(bot,mid,top,n);

    rX[0] = x[0] + 2*x[1];
    rY[0] = y[0] + 2*y[1];
    for(int i = 1; i < n-1; i++){
        rX[i] = 4*x[i] + 2 *x[i+1];
        rY[i] = 4*y[i] + 2 *y[i+1];
    }

    rX[n-1] = 8*x[n-1] + x[n];
    rY[n-1] = 8*y[n-1] + y[n];

    for(int i = 0; i < n; i++){
        p0x[i] = x[i];
        p0y[i] = y[i];
    }


    thomas(bot,mid,top,rX,n);

    resetMatrix(bot,mid,top,n);

    thomas(bot,mid,top,rY,n);

    for(int i = 0; i < n; i++){
        p1x[i] = rX[i];
        p1y[i] = rY[i];
    }

    for(int i = 0; i < n-1; i++){
        p2x[i] = 2*x[i+1] - p1x[i+1];
        p2y[i] = 2*y[i+1] - p1y[i+1];
    }
    p2x[n-1] = (x[n] + p1x[n-1])/2;
    p2y[n-1] = (y[n] + p1y[n-1])/2;

    for(int i = 1; i < n+1; i++){
        p3x[i-1] = x[i];
        p3y[i-1] = y[i];
    }
    //THE p0x STUFF IS THE POINTS FOR THE BEZIER, p1 and p2 ARE FOCII, BUT INSERTING THEM INTO THE BEZIER WILL GIVE U A SMOOTH CURVE
    for(int i = 0; i < n; i++){
        cout << "(( " << p0x[i] << "," << p0y[i] << " )" << ",";
        cout << "( " << p1x[i] << "," << p1y[i] << " )" << ",";
        cout << "( " << p2x[i] << "," << p2y[i] << " )" << ",";
        cout << "( " << p3x[i] << "," << p3y[i] << " ))" << ",";
        cout << endl;
    }
}

int main() {
	db Kx[5] = {175,188,144, 91,53};
    db Ky[5] = {689,598,548,593,673};

    multiBez(Kx,Ky, 4);
	return 0;
}
