/***************************************************************************
 *   MIT License
 * Copyright (c) 2022 Mikhail Tegin
 * michail3110@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.                  *
 ***************************************************************************/

#ifndef MATHVECTOR2D_H
#define MATHVECTOR2D_H
#include <iostream>

template<class T>
class MathVector2D
{
public:
    MathVector2D(const T& x, const T& y): _x(x), _y(y) {}
	MathVector2D(const T&& x, const T&& y): _x(std::move(x)), _y(std::move(y)) {}
    MathVector2D(): _x(0), _y(0) {}
    MathVector2D(const MathVector2D<T> &p): _x(p._x), _y(p._y) {}
	MathVector2D(const MathVector2D<T> &&p): _x(std::move(p._x)), _y(std::move(p._y)) {}
    template<class U>
                            operator MathVector2D<U>() const
    {
        return MathVector2D<U>(U(_x), U(_y));
    }
    void                    operator=(const MathVector2D<T>& p)
    {
        _x = p._x;
        _y = p._y;
    }
    bool                    operator==(const MathVector2D<T>& p) const
    {
        return _x == p._x && _y == p._y;
    }
    MathVector2D<T>         operator+(const MathVector2D<T>& p) const
    {
        return MathVector2D<T>(_x + p._x, _y + p._y);
    }
    MathVector2D<T>         operator-(const MathVector2D<T>& p) const
    {
        return MathVector2D<T>(_x - p._x, _y - p._y);
    }
    MathVector2D<T>         operator-() const
    {
        return MathVector2D<T>(-_x, -_y);
    }
    T                       operator*(const MathVector2D<T>& p) const
    {
		return std::move(_x * p._x + _y * p._y);
    }
    MathVector2D<T>         operator*(const T& mul) const
    {
        return MathVector2D<T>(_x*mul, _y*mul);
    }
    MathVector2D<T>         operator/(const T& div) const
    {
        return MathVector2D<T>(_x/div, _y/div);
    }
    void                    setX(const T &x) {_x = x;}
    void                    setY(const T &y) {_y = y;}
	const T&				getX() const {return _x;}
	const T&				getY() const {return _y;}
    T                       lenght() const
    {
        return sqrt(_x*_x + _y*_y);
    }
    T                       lenghtSquared() const
    {
        return _x*_x + _y*_y;
    }
#ifdef M_PI
    T                       angle() const
    {
        if(_x == T(0) && _y == T(0)) return 0;
        if(_x == T(0)) return ((_y > T(0)) ? M_PI/T(2) : T(3)*M_PI/T(2));
        T theta = atan(_y/_x);
        if(_x > T(0)) return ((_y > T(0)) ? theta : theta + T(2)*M_PI);
        else return theta + M_PI;
    }
#endif
    MathVector2D<T>         unitVector() const
    {
        T len = lenght();

        if(len == T(0)) len = T(1);
        return MathVector2D<T>(_x/len, _y/len);
    }
    MathVector2D<T>         rightVector() const
    {
        return MathVector2D<T>(_y, -_x);
    }
    MathVector2D<T>         leftVector() const
    {
        return MathVector2D<T>(-_y, _x);
    }
#ifdef M_PI
    T                       betweenAngle(const MathVector2D<T>& nextVector) const
    {
        T deltaAngle = nextVector.angle() - angle();

        if(deltaAngle <= -M_PI) return deltaAngle + T(2)*M_PI;
        else if(deltaAngle > M_PI) return deltaAngle - T(2)*M_PI;
        return deltaAngle;
    }
    MathVector2D<T>         rotate(const MathVector2D<T>& pointCenter, const T& angle) const
    {
        T angleRad = angle*M_PI/T(180);

        return MathVector2D<T>(pointCenter.getX() + (_x - pointCenter.getX())*cos(angleRad) - (_y - pointCenter.getY())*sin(angleRad),
                       pointCenter.getY() + (_x - pointCenter.getX())*sin(angleRad) + (_y - pointCenter.getY())*cos(angleRad));
    }
#endif
    template<class U>
    friend U                LenSquaredPoints(const MathVector2D<U>& point1, const MathVector2D<U>& point2);
    template<class U>
    friend U                LenPoints(const MathVector2D<U>& point1, const MathVector2D<U>& point2);
private:
    T                       _x;
    T                       _y;
};

template<class T>
T                           LenSquaredPoints(const MathVector2D<T>& point1, const MathVector2D<T>& point2)
{
    T                       dx = point2._x - point1._x, dy = point2._y - point1._y;

    return dx*dx + dy*dy;
}

template<class T>
T                           LenPoints(const MathVector2D<T>& point1, const MathVector2D<T>& point2)
{
    T                       dx = point2._x - point1._x, dy = point2._y - point1._y;

    return sqrt(dx*dx + dy*dy);
}

template<class T>
static inline        std::ostream& operator << (std::ostream& os, const MathVector2D<T>& vector)
{
    return os << "(" << vector.getX() << "," << vector.getY() << ")";
}

template<class T>
class MathFixedVector2D
{
public:
    MathFixedVector2D(const T& x, const T& y, const MathVector2D<T>& begin): _beginPoint(begin), _x(x), _y(y) {}
	MathFixedVector2D(const T&& x, const T&& y, const MathVector2D<T>&& begin): _beginPoint(std::move(begin)), _x(std::move(x)), _y(std::move(y)) {}
    MathFixedVector2D(const MathVector2D<T>& begin, const MathVector2D<T>& end): _beginPoint(begin), _x(end.getX() - begin.getX()), _y(end.getY() - begin.getY()) {}
    MathFixedVector2D(const MathVector2D<T>& end): _beginPoint(), _x(end.getX()), _y(end.getY()) {}
    MathFixedVector2D(): _beginPoint(), _x(0), _y(0) {}
    MathFixedVector2D(const MathFixedVector2D<T> &p): _beginPoint(p._beginPoint), _x(p._x), _y(p._y) {}
    template<class U>
                            operator MathFixedVector2D<U>() const
    {
        return MathFixedVector2D<U>(U(_x), U(_y), U(_beginPoint));
    }
    void                    operator=(const MathFixedVector2D<T>& p)
    {
        _x = p._x;
        _y = p._y;
        _beginPoint = p._beginPoint;
    }
	void                     operator=(MathVector2D<T>&& p)
	{
		_x = std::move(p._x);
		_y = std::move(p._y);
	}
    MathFixedVector2D<T>    operator+(const MathVector2D<T>& p) const
    {
        return MathFixedVector2D<T>(_x + p._x, _y + p._y, _beginPoint);
    }
    MathFixedVector2D<T>    operator-(const MathVector2D<T>& p) const
    {
        return MathFixedVector2D<T>(_x - p._x, _y - p._y, _beginPoint);
    }
    MathFixedVector2D<T>    operator-() const
    {
        return MathFixedVector2D<T>(-_x, -_y, _beginPoint);
    }
    MathFixedVector2D<T>    operator*(const T& mul) const
    {
        return MathFixedVector2D<T>(_x*mul, _y*mul, _beginPoint);
    }
    MathFixedVector2D<T>    operator/(const T& div) const
    {
        return MathFixedVector2D<T>(_x/div, _y/div, _beginPoint);
    }
    void                    setX(const T &x) {_x = x;}
    void                    setY(const T &y) {_y = y;}
    void                    setBegin(const MathVector2D<T> &point) {_beginPoint = point;}
	const T&				getX() const {return _x;}
	const T&				getY() const {return _y;}
	const MathVector2D<T>&	getBegin() const {return _beginPoint;}
	MathVector2D<T>			getEnd() const
    {
        return MathVector2D<T>(_beginPoint.getX() + _x, _beginPoint.getY() + _y);
    }
    T                       lenght() const
    {
        return sqrt(_x*_x + _y*_y);
    }
    T                       lenghtSquared() const
    {
        return _x*_x + _y*_y;
    }
#ifdef M_PI
    T                       angle() const
    {
        if(_x == T(0) && _y == T(0)) return 0;
        if(_x == T(0)) return ((_y > T(0)) ? M_PI/T(2) : T(3)*M_PI/T(2));
        T theta = atan(_y/_x);
        if(_x > T(0)) return ((_y > T(0)) ? theta : theta + T(2)*M_PI);
        else return theta + M_PI;
    }
#endif
    MathFixedVector2D<T>    unitVector() const
    {
        T len = lenght();

        if(len == T(0)) len = T(1);
        return MathFixedVector2D<T>(_x/len, _y/len, _beginPoint);
    }
	MathVector2D<T>			freeVector() const
    {
        return MathVector2D<T>(_x, _y);
    }
    MathFixedVector2D<T>    rightVector() const
    {
        return MathFixedVector2D<T>(_y, -_x, _beginPoint);
    }
    MathFixedVector2D<T>    leftVector() const
    {
        return MathFixedVector2D<T>(-_y, _x, _beginPoint);
    }
private:
    MathVector2D<T>         _beginPoint;
    T                       _x;
    T                       _y;
};

#endif // MATHVECTOR2D_H
