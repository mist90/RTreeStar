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

#include "RTreeViewer.h"
#include <QApplication>



#define MIN_X					(-100.0)
#define MIN_Y					(-100.0)
#define MIN_Z					(-100.0)
#define MAX_X					(100.0)
#define MAX_Y					(100.0)
#define MAX_Z					(100.0)
#define MAX_SIZE				(10.0)
#define NUM_TETRAEDRS			(100)


int main(int argc, char *argv[])
{
	QApplication	a(argc, argv);

	RTreeViewer		*w = new RTreeViewer(MathVector3D<double>(MIN_X, MIN_Y, MIN_Z), MathVector3D<double>(MAX_X, MAX_Y, MAX_Z), NUM_TETRAEDRS, MAX_SIZE);
	w->show();

	return a.exec();
}
