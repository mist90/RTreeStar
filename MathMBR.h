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

#ifndef MATHRTREESTARMBR_H
#define MATHRTREESTARMBR_H

#include <stddef.h>
#include <string>

//#define MATH_RTREE_STAR_MBR_DEBUG

template<class NumberType, size_t dims>
class MathMBR
{
public:
	class Exception
	{
	public:
								Exception(const std::string& errorStr): errString(errorStr) {}
	private:
		std::string				errString;
	};
							MathMBR();
							MathMBR(const MathMBR& mbr);
							MathMBR(MathMBR&& mbr);
	void					clear();
	void					setDim(const NumberType& min, const NumberType& max, const size_t& numDim);
	const NumberType&		minDim(const size_t& numDim) const;
	const NumberType&		maxDim(const size_t& numDim) const;
	void					operator+=(const MathMBR& mbr);
	MathMBR					operator+(const MathMBR& mbr) const;
	MathMBR					operator&(const MathMBR& mbr) const;
	bool					isIntersect(const MathMBR& mbr) const;
	bool					isInside(const MathMBR& mbr) const;
	bool					isInside(const NumberType& key, const size_t& numDim) const;
	bool					operator==(const MathMBR& mbr) const;
	void					operator=(const MathMBR& mbr);
	void					operator=(MathMBR&& mbr);
	NumberType				volume() const;
	NumberType				perimeter() const;
	NumberType				distance(const MathMBR& mbr) const;
	NumberType				overlapVolume(const MathMBR& mbr) const;
	NumberType				unionVolume(const MathMBR& mbr) const;
private:
	NumberType				min[dims];
	NumberType				max[dims];
	bool					isEmpty;
};

/* class MBR */
template<class NumberType, size_t dims>
MathMBR<NumberType, dims>::MathMBR(): isEmpty(true)
{
	for(size_t i = 0; i < dims; i++) {
		min[i] = NumberType(0);
		max[i] = NumberType(0);
	}
}

template<class NumberType, size_t dims>
MathMBR<NumberType, dims>::MathMBR(const MathMBR& mbr)
{
	if(!mbr.isEmpty)
		for(size_t i = 0; i < dims; i++) {
			min[i] = mbr.min[i];
			max[i] = mbr.max[i];
		}
	isEmpty = mbr.isEmpty;
}

template<class NumberType, size_t dims>
MathMBR<NumberType, dims>::MathMBR(MathMBR&& mbr)
{
	if(!mbr.isEmpty)
		for(size_t i = 0; i < dims; i++) {
			min[i] = std::move(mbr.min[i]);
			max[i] = std::move(mbr.max[i]);
		}
	isEmpty = mbr.isEmpty;
}

template<class NumberType, size_t dims>
void MathMBR<NumberType, dims>::clear()
{
	isEmpty = true;
}

template<class NumberType, size_t dims>
void MathMBR<NumberType, dims>::setDim(const NumberType &min, const NumberType &max, const size_t &numDim)
{
#ifdef MATH_RTREE_STAR_MBR_DEBUG
	if (numDim >= dims)
		throw Exception("Dimension index is exceed");
#endif
	this->min[numDim] = min;
	this->max[numDim] = max;
	isEmpty = false;
}

template<class NumberType, size_t dims>
const NumberType& MathMBR<NumberType, dims>::minDim(const size_t &numDim) const
{
#ifdef MATH_RTREE_STAR_MBR_DEBUG
	if (numDim >= dims || isEmpty)
		throw Exception("Dimension index is exceed");
#endif
	return min[numDim];
}

template<class NumberType, size_t dims>
const NumberType& MathMBR<NumberType, dims>::maxDim(const size_t &numDim) const
{
#ifdef MATH_RTREE_STAR_MBR_DEBUG
	if (numDim >= dims || isEmpty)
		throw Exception("Dimension index is exceed");
#endif
	return max[numDim];
}

template<class NumberType, size_t dims>
void MathMBR<NumberType, dims>::operator+=(const MathMBR& mbr)
{
	if (mbr.isEmpty)
		return;
	if (!isEmpty) {
		for (size_t i = 0; i < dims; i++) {
			if (mbr.min[i] < min[i])
				min[i] = mbr.min[i];
			if (mbr.max[i] > max[i])
				max[i] = mbr.max[i];
		}
	} else {
		for (size_t i = 0; i < dims; i++) {
			min[i] = mbr.min[i];
			max[i] = mbr.max[i];
		}
		isEmpty = mbr.isEmpty;
	}
}

template<class NumberType, size_t dims>
MathMBR<NumberType, dims> MathMBR<NumberType, dims>::operator+(const MathMBR& mbr) const
{
	MathMBR		ret(*this);

	ret += mbr;

	return ret;
}

template<class NumberType, size_t dims>
MathMBR<NumberType, dims> MathMBR<NumberType, dims>::operator&(const MathMBR& mbr) const
{
	MathMBR		ret;

	if (isEmpty || mbr.isEmpty)
		return ret;
	for (size_t i = 0; i < dims; i++) {
		if (mbr.min[i] > max[i])
			return MathMBR();
		if (mbr.max[i] < min[i])
			return MathMBR();

		if (mbr.min[i] > min[i])
			ret.min[i] = mbr.min[i];
		else
			ret.min[i] = min[i];
		if (mbr.max[i] < max[i])
			ret.max[i] = mbr.max[i];
		else
			ret.max[i] = max[i];
	}
	ret.isEmpty = false;

	return ret;
}

template<class NumberType, size_t dims>
bool MathMBR<NumberType, dims>::isIntersect(const MathMBR& mbr) const
{
	if (isEmpty || mbr.isEmpty)
		return false;
	for (size_t i = 0; i < dims; i++) {
		if (mbr.min[i] > max[i])
			return false;
		if (mbr.max[i] < min[i])
			return false;
	}

	return true;
}

template<class NumberType, size_t dims>
bool MathMBR<NumberType, dims>::isInside(const MathMBR& mbr) const
{
	if (isEmpty || mbr.isEmpty)
		return false;
	for (size_t i = 0; i < dims; i++) {
		if (mbr.min[i] < min[i])
			return false;
		if (mbr.max[i] > max[i])
			return false;
	}

	return true;
}

template<class NumberType, size_t dims>
bool MathMBR<NumberType, dims>::isInside(const NumberType &key, const size_t &numDim) const
{
#ifdef MATH_RTREE_STAR_MBR_DEBUG
	if (numDim >= dims)
		throw Exception("Dimension index is exceed");
#endif
	if (isEmpty)
		return false;

	if (key >= min[numDim] && key <= max[numDim])
		return true;
	else
		return false;
}

template<class NumberType, size_t dims>
bool MathMBR<NumberType, dims>::operator==(const MathMBR& mbr) const
{
	if (isEmpty != mbr.isEmpty)
		return false;
	if (!isEmpty)
		for (size_t i = 0; i < dims; i++)
			if (mbr.min[i] != min[i] || mbr.max[i] != max[i])
				return false;
	return true;
}

template<class NumberType, size_t dims>
void MathMBR<NumberType, dims>::operator=(const MathMBR& mbr)
{
	if (!mbr.isEmpty)
		for (size_t i = 0; i < dims; i++) {
			min[i] = mbr.min[i];
			max[i] = mbr.max[i];
		}
	isEmpty = mbr.isEmpty;
}

template<class NumberType, size_t dims>
void MathMBR<NumberType, dims>::operator=(MathMBR&& mbr)
{
	if (!mbr.isEmpty)
		for (size_t i = 0; i < dims; i++) {
			min[i] = std::move(mbr.min[i]);
			max[i] = std::move(mbr.max[i]);
		}
	isEmpty = mbr.isEmpty;
}

template<class NumberType, size_t dims>
NumberType MathMBR<NumberType, dims>::volume() const
{
	if (isEmpty)
		return NumberType(0);
	NumberType ret = max[0] - min[0];

	for (size_t i = 1; i < dims; i++)
		ret *= (max[i] - min[i]);

	return ret;
}

template<class NumberType, size_t dims>
NumberType MathMBR<NumberType, dims>::perimeter() const
{
	if (isEmpty)
		return NumberType(0);
	NumberType ret = max[0] - min[0];

	for (size_t i = 1; i < dims; i++)
		ret += (max[i] - min[i]);

	return std::move(ret*NumberType(2));
}

template<class NumberType, size_t dims>
NumberType MathMBR<NumberType, dims>::distance(const MathMBR& mbr) const
{
#ifdef MATH_RTREE_STAR_MBR_DEBUG
	if (isEmpty)
		throw Exception("Empty MBR hasn't spatial location");
#endif
	NumberType	ret = NumberType(0);

	for (size_t i = 0; i < dims; i++) {
		NumberType		delta = mbr.min[i] + mbr.max[i] - min[i] - max[i];

		ret += delta*delta;
	}

	return std::move(ret/NumberType(4));
}

template<class NumberType, size_t dims>
NumberType MathMBR<NumberType, dims>::overlapVolume(const MathMBR& mbr) const
{
	NumberType						ret(0);
	const NumberType				*pmin, *pmax;

	if (!mbr.isEmpty && !isEmpty) {
		pmin = (mbr.min[0] > min[0])? &mbr.min[0] : &min[0];
		pmax = (mbr.max[0] < max[0])? &mbr.max[0] : &max[0];
		if (*pmin >= *pmax)
			return ret;
		ret = (*pmax - *pmin);
		for (size_t i = 1; i < dims; i++) {
			pmin = (mbr.min[i] > min[i])? &mbr.min[i] : &min[i];
			pmax = (mbr.max[i] < max[i])? &mbr.max[i] : &max[i];
			if(*pmin >= *pmax)
				return NumberType(0);
			ret *= (*pmax - *pmin);
		}
	}

	return ret;
}

template<class NumberType, size_t dims>
NumberType MathMBR<NumberType, dims>::unionVolume(const MathMBR& mbr) const
{
	NumberType						ret(0);
	const NumberType				*pmin, *pmax;

	if (isEmpty && mbr.isEmpty)
		return ret;
	if (isEmpty)
		return mbr.volume();
	if (mbr.isEmpty)
		return volume();

	pmin = (mbr.min[0] < min[0])? &mbr.min[0] : &min[0];
	pmax = (mbr.max[0] > max[0])? &mbr.max[0] : &max[0];

	ret = *pmax - *pmin;
	for (size_t i = 1; i < dims; i++) {
		pmin = (mbr.min[i] < min[i])? &mbr.min[i] : &min[i];
		pmax = (mbr.max[i] > max[i])? &mbr.max[i] : &max[i];

		ret *= *pmax - *pmin;
	}

	return ret;
}

#endif // MATHRTREESTARMBR_H
