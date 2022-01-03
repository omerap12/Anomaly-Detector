#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include "minCircle.h"
using namespace std;

/**
 * @param a point
 * @param b point
 * @return distance between 2 points
 */
float dist(const Point& a, const Point& b)
{
    float distance = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return distance;
}


/**
 * check if point inside or near the circle
 * @param circle circle
 * @param point point
 * @return true / false
 */
bool isInside(const Circle& circle, const Point& point)
{
    if (dist(circle.center, point) <= circle.radius)
        return true;
    return false;
}


/**
 * get circle center from 3 points
 * @param bx x value of point b
 * @param by y value of point b
 * @param cx x value of point c
 * @param cy y value of point c
 * @return center point of circle
 */
Point circleCenter(float bx, float by, float cx, float cy)
{
    float b = bx * bx + by * by;
    float c = cx * cx + cy * cy;
    float d = bx * cy - by * cx;
    float x = (cy * b - by * c) / (2 * d);
    float y = (bx * c - cx * b) / (2 * d);
    return Point(x,y);
}


/**
 * create circle from 3 given points
 * @param a given point
 * @param b given point
 * @param c given point
 * @return unique circle
 */
Circle circleFrom3Points(const Point& a, const Point& b, const Point& c)
{
    float bx = b.x - a.x;
    float by = b.y - a.y;
    float cx = c.x - a.x;
    float cy = c.y - a.y;
    Point center = circleCenter(bx, by, cx, cy);
    center.x += a.x;
    center.y += a.y;
    return Circle(center, dist(center, a));
}


/**
 * create circle from 2 given points
 * @param a given point
 * @param b given point
 * @return circle
 */
Circle circleFrom2Points(const Point& a, const Point& b)
{
    // create middle point of a and b
    float middleX = (a.x + b.x) / 2;
    float middleY = (a.y + b.y) / 2;
    Point c = Point(middleX, middleY);

    // set radius to be half the distance of the points a and b
    return Circle(c, dist(a, b)/2);

}


/**
 * @param points points
 * @return minimum circle
 */
Circle minCircle(vector<Point>& points)
{
    // when there are no points
    if (points.empty()) {
        return Circle(Point(0,0),0);
    }
    // when there is one point
    else if (points.size() == 1) {
        return Circle(points[0], 0);
    }
    // when there are 2 points
    else if (points.size() == 2) {
        return circleFrom2Points(points[0], points[1]);
    }
    bool isValidCircle = true;
    // To check if MEC can be determined by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circleFrom2Points(points[i], points[j]);
            // check if circle is valid - if all the points into it
            for (const Point& p : points){
                if (!isInside(c, p)){
                    isValidCircle = false;
                }
            }
            if (isValidCircle)
                return c;
        }
    }
    return circleFrom3Points(points[0], points[1], points[2]);
}


/**
 * using Welzel's algorithm
 * @param points input points
 * @param p points on the circle boundary
 * @param size number of points in points that are not yet processed
 * @return minimum circle
 */
Circle helper(vector<Point>& points, vector<Point> p, int size)
{
    // if all points processed or |r| = 3
    if (size == 0 || p.size() == 3) {
        return minCircle(p);
    }
    // create random points
    int idx = rand() % size;
    Point r = points[idx];
    // Put the picked point at the end of points
    swap(points[idx], points[size - 1]);
    // get the minimum circle d from the
    Circle d = helper(points, p, size - 1);
    // if d contains r, return d
    if (isInside(d, r)) {
        return d;
    }
    // otherwise, must be on the boundary of the minimum circle
    p.push_back(r);
    // return the minimum for points
    return helper(points, p, size - 1);
}

/**
 * welzel algorithm
 * @param points given pointes
 * @return circle
 */
Circle welzl(const vector<Point>& points)
{
    vector<Point> points_copy = points;
    random_shuffle(points_copy.begin(), points_copy.end());
    return helper(points_copy, {}, points_copy.size());
}


/**
 * find minimum circle
 * @param points given points
 * @param size number of given points
 * @return minimum circle
 */
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> pointsVec;
    // convert array of points to vector
    for (int i = 0; i<size; i++) {
        pointsVec.push_back(*points[i]);
    }
    return welzl(pointsVec);
}
