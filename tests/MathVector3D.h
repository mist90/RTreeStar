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

#ifndef MATHVECTOR3D_H
#define MATHVECTOR3D_H
#include "MathVector2D.h"
#include "MathTransformMatrix3D.h"

template<class T>
class MathVector3D
{
public:
    MathVector3D(const T& x, const T& y, const T& z): _x(x), _y(y), _z(z) {}
	MathVector3D(const T&& x, const T&& y, const T&& z): _x(std::move(x)), _y(std::move(y)), _z(std::move(z)) {}
    MathVector3D(): _x(0), _y(0), _z(0) {}
    MathVector3D(const MathVector3D<T>& p): _x(p._x), _y(p._y), _z(p._z) {}
	MathVector3D(MathVector3D<T>&& p): _x(std::move(p._x)), _y(std::move(p._y)), _z(std::move(p._z)) {}
    template<class U>
                                operator MathVector3D<U>() const
    {
        return MathVector3D<U>(U(_x), U(_y), U(_z));
    }
    void                        operator=(const MathVector3D<T>& p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
    }
	void                        operator=(MathVector3D<T>&& p)
	{
		_x = std::move(p._x);
		_y = std::move(p._y);
		_z = std::move(p._z);
	}
    bool                        operator==(const MathVector3D<T>& p) const
    {
        return _x == p._x && _y == p._y && _z == p._z;
    }
    bool                        operator!=(const MathVector3D<T>& p) const
    {
        return _x != p._x || _y != p._y || _z != p._z;
    }
    MathVector3D<T>             operator+(const MathVector3D<T>& p) const
    {
        return MathVector3D<T>(_x + p._x, _y + p._y, _z + p._z);
    }
	void						operator+=(const MathVector3D<T>& p)
	{
		_x += p._x;
		_y += p._y;
		_z += p._z;
	}
    MathVector3D<T>             operator-(const MathVector3D<T>& p) const
    {
		return MathVector3D<T>(_x - p._x, _y - p._y, _z - p._z);
    }
	void						operator-=(const MathVector3D<T>& p)
	{
		_x -= p._x;
		_y -= p._y;
		_z -= p._z;
	}
    MathVector3D<T>             operator-() const
    {
		return MathVector3D<T>(-_x, -_y, -_z);
    }
    T                           operator*(const MathVector3D<T>& p) const  // скалярное произведение
    {
		return std::move(_x * p._x + _y * p._y + _z * p._z);
    }
    MathVector3D<T>             operator*(const T& mul) const
    {
		return MathVector3D<T>(_x*mul, _y*mul, _z*mul);
    }
    MathVector3D<T>             operator/(const T& mul) const
    {
		return MathVector3D<T>(_x/mul, _y/mul, _z/mul);
    }
    MathVector3D<T>             operator^(const MathVector3D<T>& p) const  // векторное произведение
    {
		return MathVector3D<T>(_y*p._z - _z*p._y,
							   _z*p._x - _x*p._z,
							   _x*p._y - _y*p._x);
    }
    void                        setX(const T& x) {_x = x;}
    void                        setY(const T& y) {_y = y;}
    void                        setZ(const T& z) {_z = z;}
	const T&					getX() const {return _x;}
	const T&					getY() const {return _y;}
	const T&					getZ() const {return _z;}
    T                           lenght() const
    {
        return sqrt(_x*_x + _y*_y + _z*_z);
    }
    T                           lenghtSquared() const
    {
        return _x*_x + _y*_y + _z*_z;
    }
    MathVector3D<T>             unitVector() const
    {
        T len = lenght();

        if(len == T(0)) len = T(1);
        return MathVector3D<T>(_x/len, _y/len, _z/len);
    }
    MathVector3D<T>             transform(const MathTransformMatrix3D<T>& matrix) const
    {
        return MathVector3D<T>(_x*matrix.element(0, 0) + _y*matrix.element(1, 0) + _z*matrix.element(2, 0) + matrix.element(3, 0),
                               _x*matrix.element(0, 1) + _y*matrix.element(1, 1) + _z*matrix.element(2, 1) + matrix.element(3, 1),
                               _x*matrix.element(0, 2) + _y*matrix.element(1, 2) + _z*matrix.element(2, 2) + matrix.element(3, 2));
    }
    MathVector3D<T>             rotateParallelAxisX(const MathVector3D<T>& pointCenter, const T& angle) const
    {
        return MathVector3D<T>(_x,
                              pointCenter.getY() + (_y - pointCenter.getY())*cos(angle) - (_z - pointCenter.getZ())*sin(angle),
                              pointCenter.getZ() + (_y - pointCenter.getY())*sin(angle) + (_z - pointCenter.getZ())*cos(angle));
    }
    MathVector3D<T>             rotateParallelAxisX(const MathVector3D<T>& pointCenter, const T& sinAngle, const T& cosAngle) const
    {
        return MathVector3D<T>(_x,
                              pointCenter.getY() + (_y - pointCenter.getY())*cosAngle - (_z - pointCenter.getZ())*sinAngle,
                              pointCenter.getZ() + (_y - pointCenter.getY())*sinAngle + (_z - pointCenter.getZ())*cosAngle);
    }
	MathVector3D<T>             rotateParallelAxisX(const T& sinAngle, const T& cosAngle) const
	{
		return MathVector3D<T>(_x,
							  _y*cosAngle - _z*sinAngle,
							  _y*sinAngle + _z*cosAngle);
	}
    MathVector3D<T>             rotateParallelAxisY(const MathVector3D<T>& pointCenter, const T& angle) const
    {
        return MathVector3D<T>(pointCenter.getX() + (_z - pointCenter.getZ())*sin(angle) + (_x - pointCenter.getX())*cos(angle),
                              _y,
                              pointCenter.getZ() + (_z - pointCenter.getZ())*cos(angle) - (_x - pointCenter.getX())*sin(angle));
    }
    MathVector3D<T>             rotateParallelAxisY(const MathVector3D<T>& pointCenter, const T& sinAngle, const T& cosAngle) const
    {
        return MathVector3D<T>(pointCenter.getX() + (_z - pointCenter.getZ())*sinAngle + (_x - pointCenter.getX())*cosAngle,
                              _y,
                              pointCenter.getZ() + (_z - pointCenter.getZ())*cosAngle - (_x - pointCenter.getX())*sinAngle);
    }
	MathVector3D<T>             rotateParallelAxisY(const T& sinAngle, const T& cosAngle) const
	{
		return MathVector3D<T>(_z*sinAngle + _x*cosAngle,
							  _y,
							  _z*cosAngle - _x*sinAngle);
	}
    MathVector3D<T>             rotateParallelAxisZ(const MathVector3D<T>& pointCenter, const T& angle) const
    {
        return MathVector3D<T>(pointCenter.getX() + (_x - pointCenter.getX())*cos(angle) - (_y - pointCenter.getY())*sin(angle),
                              pointCenter.getY() + (_x - pointCenter.getX())*sin(angle) + (_y - pointCenter.getY())*cos(angle),
                              _z);
    }
    MathVector3D<T>             rotateParallelAxisZ(const MathVector3D<T>& pointCenter, const T& sinAngle, const T& cosAngle) const
    {
        return MathVector3D<T>(pointCenter.getX() + (_x - pointCenter.getX())*cosAngle - (_y - pointCenter.getY())*sinAngle,
                              pointCenter.getY() + (_x - pointCenter.getX())*sinAngle + (_y - pointCenter.getY())*cosAngle,
                              _z);
    }
	MathVector3D<T>             rotateParallelAxisZ(const T& sinAngle, const T& cosAngle) const
	{
		return MathVector3D<T>(_x*cosAngle - _y*sinAngle,
							  _x*sinAngle + _y*cosAngle,
							  _z);
	}
    MathVector2D<T>             projectionXY() const
    {
        return MathVector2D<T>(_x, _y);
    }
    MathVector2D<T>             projectionXZ() const
    {
        return MathVector2D<T>(_x, _z);
    }
    MathVector2D<T>             projectionYZ() const
    {
        return MathVector2D<T>(_y, _z);
    }
    template<class U>
    friend U                    LenSquaredPoints(const MathVector3D<U>& point1, const MathVector3D<U>& point2);
    template<class U>
    friend U                    LenPoints(const MathVector3D<U>& point1, const MathVector3D<U>& point2);
private:
    T                           _x;
    T                           _y;
    T                           _z;
};

template<class T>
T                               LenSquaredPoints(const MathVector3D<T>& point1, const MathVector3D<T>& point2)
{
    T                           dx = point2._x - point1._x, dy = point2._y - point1._y, dz = point2._z - point1._z;

    return dx*dx + dy*dy + dz*dz;
}

template<class T>
T                               LenPoints(const MathVector3D<T>& point1, const MathVector3D<T>& point2)
{
    T                           dx = point2._x - point1._x, dy = point2._y - point1._y, dz = point2._z - point1._z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

template<class T>
std::ostream&           operator << (std::ostream& os, const MathVector3D<T>& vector)
{
    return os << "(" << vector.getX() << "," << vector.getY() << "," << vector.getZ() << ")";
}

template<class T>
class MathFixedVector3D
{
public:
    MathFixedVector3D(const T& x, const T& y, const T& z, const MathVector3D<T>& begin): _beginPoint(begin), _x(x), _y(y), _z(z) {}
	MathFixedVector3D(const T&& x, const T&& y, const T&& z, const MathVector3D<T>&& begin): _beginPoint(std::move(begin)), _x(std::move(x)), _y(std::move(y)), _z(std::move(z)) {}
    MathFixedVector3D(const MathVector3D<T>& begin, const MathVector3D<T>& end): _beginPoint(begin), _x(end.getX() - begin.getX()), _y(end.getY() - begin.getY()), _z(end.getZ() - begin.getZ()) {}
    MathFixedVector3D(const MathVector3D<T>& end): _beginPoint(), _x(end.getX()), _y(end.getY()), _z(end.getZ()) {}
    MathFixedVector3D(): _beginPoint(), _x(0), _y(0), _z(0) {}
    MathFixedVector3D(const MathFixedVector3D<T>& p): _beginPoint(p._beginPoint), _x(p._x), _y(p._y), _z(p._z) {}
    template<class U>
                                operator MathFixedVector3D<U>() const
    {
		return MathFixedVector3D<U>(U(_x), U(_y), U(_z), MathVector3D<U>(_beginPoint));
    }
    void                        operator=(const MathFixedVector3D<T>& p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        _beginPoint = p._beginPoint;
    }
    MathFixedVector3D<T>        operator+(const MathVector3D<T>& p) const
    {
        return MathFixedVector3D<T>(_x + p._x, _y + p._y, _z + p._z, _beginPoint);
    }
    MathFixedVector3D<T>        operator-(const MathVector3D<T>& p) const
    {
        return MathFixedVector3D<T>(_x - p._x, _y - p._y, _z - p._z, _beginPoint);
    }
    MathFixedVector3D<T>        operator-() const
    {
        return MathFixedVector3D<T>(-_x, -_y, -_z, _beginPoint);
    }
    MathFixedVector3D<T>        operator*(const T& mul) const
    {
        return MathFixedVector3D<T>(_x*mul, _y*mul, _z*mul, _beginPoint);
    }
    MathFixedVector3D<T>        operator/(const T& mul) const
    {
        return MathFixedVector3D<T>(_x/mul, _y/mul, _z/mul, _beginPoint);
    }
    MathFixedVector3D<T>        operator^(const MathVector3D<T>& p) const  // векторное произведение
    {
        return MathFixedVector3D<T>(_y*p.getZ() - _z*p.getY(),
                               _z*p.getX() - _x*p.getZ(),
                               _x*p.getY() - _y*p.getX(),
                               _beginPoint);
    }
    void                        setX(const T& x) {_x = x;}
    void                        setY(const T& y) {_y = y;}
    void                        setZ(const T& z) {_z = z;}
    void                        setBegin(const MathVector3D<T>& point) {_beginPoint = point;}
	const T&					getX() const {return _x;}
	const T&					getY() const {return _y;}
	const T&					getZ() const {return _z;}
	const MathVector3D<T>&		getBegin() const {return _beginPoint;}
    MathVector3D<T>             getEnd() const
    {
        return MathVector3D<T>(_beginPoint.getX() + _x, _beginPoint.getY() + _y, _beginPoint.getZ() + _z);
    }
    T                           lenght() const
    {
        return sqrt(_x*_x + _y*_y + _z*_z);
    }
    T                           lenghtSquared() const
    {
		return std::move(_x*_x + _y*_y + _z*_z);
    }
    MathFixedVector3D<T>        unitVector() const
    {
        T len = lenght();

        if(len == T(0)) len = T(1);
        return MathFixedVector3D<T>(_x/len, _y/len, _z/len, _beginPoint);
    }
    MathVector3D<T>             freeVector() const
    {
        return MathVector3D<T>(_x, _y, _z);
    }
    MathFixedVector3D<T>        transform(const MathTransformMatrix3D<T>& matrix) const
    {
        return MathFixedVector3D<T>(_x*matrix.element(0, 0) + _y*matrix.element(1, 0) + _z*matrix.element(2, 0) + matrix.element(3, 0),
                               _x*matrix.element(0, 1) + _y*matrix.element(1, 1) + _z*matrix.element(2, 1) + matrix.element(3, 1),
                               _x*matrix.element(0, 2) + _y*matrix.element(1, 2) + _z*matrix.element(2, 2) + matrix.element(3, 2),
                               _beginPoint.transform(matrix));
    }
    MathVector2D<T>             projectionXY() const
    {
        return MathFixedVector2D<T>(_x, _y, _beginPoint.projectionXY());
    }
    MathVector2D<T>             projectionXZ() const
    {
        return MathFixedVector2D<T>(_x, _z, _beginPoint.projectionXZ());
    }
    MathVector2D<T>             projectionYZ() const
    {
        return MathFixedVector2D<T>(_y, _z, _beginPoint.projectionYZ());
    }
private:
    MathVector3D<T>             _beginPoint;
    T                           _x;
    T                           _y;
    T                           _z;
};


#endif // MATHVECTOR3D_H
