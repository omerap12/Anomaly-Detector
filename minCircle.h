#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}

};
Circle findMinCircle(Point** points,size_t size);
Circle welzl_helper(vector<Point>& P, vector<Point> R, int n);
Circle min_circle_trivial(vector<Point>& P);
Circle circle_from(const Point& A, const Point& B);
Circle circle_from(const Point& A, const Point& B, const Point& C);
Point get_circle_center(float bx, float by, float cx, float cy);
bool is_inside(const Circle& c, const Point& p);
float dist(const Point& a, const Point& b);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
