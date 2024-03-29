/*
 * Helpers.hpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Travis Lewis
 */

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <vector>
#include <iostream>

using namespace std;

#include "vec.h"
#include "math.h"

#include "Thing.hpp"

#define PI 3.1415962

struct Point2D
{
	int x, y;
};

Thing createUnitCube()
{
	Thing cube;
	cube.type = Thing::CUBE;
	cube.points.push_back(vec4(-0.5, -0.5, -0.5, 1));
	cube.points.push_back(vec4(-0.5, -0.5, 0.5, 1));
	cube.points.push_back(vec4(-0.5, 0.5, -0.5, 1));
	cube.points.push_back(vec4(-0.5, 0.5, 0.5, 1));

	cube.points.push_back(vec4(0.5, -0.5, -0.5, 1));
	cube.points.push_back(vec4(0.5, -0.5, 0.5, 1));
	cube.points.push_back(vec4(0.5, 0.5, -0.5, 1));
	cube.points.push_back(vec4(0.5, 0.5, 0.5, 1));

	//which vectors pairs to draw as lines:
	// (0,1), (0,2), (0,4), (1,3), (1,5), (2,3), (2,6), (3,7), (4,5),
	// (4,6), (5,7), (6,7)
	//^those are the indices of the points array to use.

	return cube;
}

Thing createUnitCylinder(int n)
{
	float theta = (2*PI) / n;

	Thing t;
	t.type = Thing::CYLINDER;

//	float pythag = sqrt((0.5*0.5) + (0.5*0.5));

	/*the commented out lines that use pythag make a cylinder that's quite a
	bit wider than the unit cube. The uncommented lines work fairly well,
	the resulting cylinder is a little short in the x axis, but still
	better than the pythag one. */

	for(int i = 0 ; i < n ; i++)
	{
		float x = 0.5 * cos((float)i * theta + (theta/2.0));
		float z = 0.5 * sin((float)i * theta + (theta/2.0));
//		float x = pythag * cos((float)i * theta);
//		float z = pythag * sin((float)i * theta);
		vec4 v(x,0.5,z,1.0);
		t.points.push_back(v);
	}

	for(int i = 0 ; i < n ; i++)
	{
		float x = 0.5 * cos((float)i * theta + (theta/2.0));
		float z = 0.5 * sin((float)i * theta + (theta/2.0));
//		float x = pythag * cos((float)i * theta);
//		float z = pythag * sin((float)i * theta);
		vec4 v(x,-0.5,z,1.0);
		t.points.push_back(v);
	}
	return t;
}

Thing createUnitCone(int n) //n = number of points that make up the base
{
	Thing t;
	t.type = Thing::CONE;

	t.points.push_back(vec4(0.0, 0.5, 0.0, 1.0)); //top of cone

	float theta = (2*PI)/(float)n;

	for(int i = 0 ; i < n ; i++)
	{
		float x = 0.5 * cos((float)i * theta + (theta/2.0));
		float z = 0.5 * sin((float)i * theta + (theta/2.0));
		vec4 v(x, -0.5, z, 1.0);
		t.points.push_back(v);
	}
	return t;
}

std::vector<Point2D> getPointsFromLine2D(int x1, int y1, int x2, int y2)
{
	std::vector<Point2D> points;

	//make sure x1 < x2
	if(x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	//get slope/distance information
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = ((float)dy) / ((float)dx);

	if(dx == 0 && dx != 0)
	{ //vertical line
		int inc = (y1 < y2) ? 1 : -1;
		for(int i = y1 ; i != y2 ; i += inc)
		{
			Point2D p;
			p.x = x1;
			p.y = i;
			points.push_back(p);
		}
	}
	else if(dy == 0 && dx != 0)
	{ // horizontal line
		//x1 < x2 so we don't need to check that
		for(int i = x1 ; i < x2 ; i++)
		{
			Point2D p;
			p.x = i;
			p.y = y1;
			points.push_back(p);
		}
	}
	else if(m <=1 && m >= -1)
	{ // non-steep line, iterate over x
		float ypos = (float)y1;
		for(int x = x1 ; x < x2 ; x++)
		{
			Point2D p;
			p.x = x;
			p.y = (int)ypos;
			points.push_back(p);
			ypos += m;
		}
	}
	else if(m > 1 || m < -1)
	{ // steep line, iterate over y
		m = 1.0/m; //invert slope for vertical use
		float xpos;
		if(y1 < y2)
		{
			xpos = (float)x1;
			for(int y = y1 ; y < y2 ; y++)
			{
				Point2D p;
				p.x = (int)xpos;
				p.y = y;
				points.push_back(p);
				xpos += m;
			}
		}
		else
		{
			xpos = (float)x2;
			for(int y = y2 ; y < y1 ; y++)
			{
				Point2D p;
				p.x = (int)xpos;
				p.y = y;
				points.push_back(p);
				xpos += m;
			}
		}
	}

	return points;
}

vec3 vec4Tovec3(vec4 &v)
{
	vec3 vec;
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;

	return vec;
}

std::vector<vec3> vec4Tovec3(std::vector<vec4> list)
{
	std::vector<vec3> converted;

	for(std::vector<vec4>::iterator it = list.begin(), end = list.end() ;
			it != end ; ++it)
	{
		vec3 v;
		v.x = it->x;
		v.y = it->y;
		v.z = it->z;
		converted.push_back(v);
	}
	return converted;
}

vec2 vec4Tovec2(vec4 &v)
{
	vec2 vec;
	vec.x = v.x;
	vec.y = v.y;
	return vec;
}

std::vector<vec2> vec4Tovec2(std::vector<vec4> list)
{
	std::vector<vec2> converted;

	for(std::vector<vec4>::iterator it = list.begin(), end = list.end() ;
			it != end ; ++it)
	{
		vec2 v;
		v.x = it->x;
		v.y = it->y;
		converted.push_back(v);
	}
	return converted;
}

vec2 vec3Tovec2(vec3 &v)
{
	vec2 vec;
	vec.x = v.x;
	vec.y = v.y;

	return vec;
}

std::vector<vec2> vec3Tovec2(std::vector<vec3> list)
{
	std::vector<vec2> converted;

	for(std::vector<vec3>::iterator it = list.begin(), end = list.end() ;
			it != end ; ++it)
	{
		vec2 v;
		v.x = it->x;
		v.y = it->y;
		converted.push_back(v);
	}
	return converted;
}

#endif /* HELPERS_HPP_ */
