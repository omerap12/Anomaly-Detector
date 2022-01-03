
#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float a = 0;
    for (int i = 0; i<size; i++) {
        a = a + x[i];
    }
    return a/size;
}
float spanValue(float *x, int size,int power) {
    float answer = 0;
    for (int i = 0; i<size; i++) {
        answer = answer + pow(x[i], power);
    }
    return (1/float(size))*answer;
}


// returns the variance of X and Y
float var(float* x, int size){
    float a = spanValue(x,size,2);
    float b = pow(spanValue(x,size,1),2);
    return a-b;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float xy[size];
    for (int i = 0; i<size; i++) {
        xy[i] = x[i]*y[i];
    }
    return spanValue(xy,size,1)-((spanValue(x,size,1))*(spanValue(y,size,1)));
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float pearson_correlation_coefficient = (cov(x, y, size)/ (sqrt(var(x, size))* sqrt(var(y, size))));
    return pearson_correlation_coefficient;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x[size];
    float y[size];

    for (int i = 0; i<size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x,y,size)/ var(x,size);
    float b = avg(y,size) - a * avg(x,size);
    return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l = linear_reg(points,size);
    return dev(p,l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float value = p.y-l.f(p.x);
    if (value < 0) {
        return -value;
    }
    return value;
}
