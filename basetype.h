#ifndef BASETYPE_H_2013_12_8
#define BASETYPE_H_2013_12_8

#include <cmath>
#include <ostream>
namespace base_type{

template <class T, class DistType = T>
class Point2T
{
public:
	typedef T        value_type;
	typedef DistType distance_type;

public:
	Point2T()
		: x(T())
		, y(T())
		, ID(0)
	{
	}

	Point2T(T xx, T yy)
		: x(xx)
		, y(yy)
		, ID(0)
	{
	}

	Point2T(const Point2T<T> &p)
		: x(p.x)
		, y(p.y)
		, ID(0)
	{
	}

	Point2T<T> &operator = (const Point2T<T> &p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	bool operator == (const Point2T<T> &p) const
	{
		return x == p.x && y == p.y;
	}


public:
	distance_type EucDist(const Point2T<T> &p) const
	{
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}

	distance_type BlkDist(const Point2T<T> &p) const
	{
		return abs(x - p.x) + abs(y - p.y);
	}

public:
	const Point2T<T> operator += (const Point2T<T> &p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	const Point2T<T> operator -= (const Point2T<T> &p)
	{
		x -= p.x;
		y -= p.y;
		return this;
	}

	template <class MultiType>
	const Point2T<T> operator *= (MultiType m)
	{
		x *= m;
		y *= m;
		return this;
	}

	template <class MultiType>
	const Point2T<T> operator /= (MultiType m)
	{
		if (m == 0)
		{
			throw "divide by zero";
		}
		x /= m;
		y /= m;
		return *this;
	}

#ifdef _CONSOLE
	friend std::ostream& operator << (std::ostream &os, const Point2T<T> &p)
	{
		return os << "(" << p.x << "," << p.y << ")";
	}
#endif

public:
	T x;
	T y;
	int ID;
};



template <class T>
const Point2T<T> operator + (const Point2T<T> &a, const Point2T<T> &b)
{
	Point2T r(a);
	a += b;
	return r;
}

template <class T>
const Point2T<T> operator - (const Point2T<T> &a, const Point2T<T> &b)
{
	Point2T r(a);
	a -= b;
	return r;
}

template <class T, class MultiType>
const Point2T<T> operator * (const Point2T<T> &a, MultiType m)
{
	Point2T r(a);
	a *= m;
	return r;
}


template <class T, class MultiType>
const Point2T<T> operator * ( MultiType m, const Point2T<T> &a)
{
	Point2T r(a);
	a *= m;
	return r;
}


template <class T>
typename Point2T<T>::distance_type EucDist(const Point2T<T> &a, const Point2T<T> &b)
{
	return a.EucDist(b);
}

template <class T>
typename Point2T<T>::distance_type BlkDist(const Point2T<T> &a, const Point2T<T> &b)
{
	return a.BlkDist(b);
}




template <class T, class DistType = T>
class Point2WithIDT : public Point2T<T, DistType>
{
public:
	int ID;
};

}
#endif