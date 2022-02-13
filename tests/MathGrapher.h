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

/************************************************************************************************
 *    Класс выполняет отрисовку мат. объектов в трехмерном пространстве при помощи OpenGL       *
 ************************************************************************************************/
#ifndef MATHGRAPHER_H
#define MATHGRAPHER_H

#include <QtOpenGL/QGLWidget>
#include <QColor>
#include <vector>
#include <memory>
#include <math.h>
#include "MathVector3D.h"
#include "../MathMBR.h"
#define AXIS_LEN                    (0.2)

/* Класс, задающий допустимую область. Представляет собой параллелепипед. */
class MathGrapherRegion: public MathMBR<double, 3>
{
public:
									MathGrapherRegion() {}
									MathGrapherRegion(const MathVector3D<double>& begin, const MathVector3D<double>& end);
									MathGrapherRegion(const MathMBR<double, 3>& mbr): MathMBR<double, 3>(mbr) {}
	const MathVector3D<double>		begin() const ;                                      /* начало области */
	double							dx() const;                                          /* размер по оси X */
	double							dy() const;                                          /* размер по оси Y */
	double							dz() const;                                          /* размер по оси Z */
};

/* Объект, добавляемый в MathGrapher */
class MathGrapherObject
{
public:
									MathGrapherObject() {}
	virtual							~MathGrapherObject() {}
	virtual size_t					getSurface(std::vector<double>& vertex, std::vector<double>& normals, const double& size) const = 0;
	virtual GLenum					getTypeSurfaces() const = 0;
	virtual MathGrapherRegion		getRegion() const = 0;
};

class MathGrapherList
{
public:
	MathGrapherList(GLenum typeArray): type(typeArray) {}
	std::vector<double>             vertex;
	std::vector<double>             colors;
	std::vector<double>             normals;
	GLenum                          type;
};

class MathGrapherLayer
{
public:
									MathGrapherLayer() {isVisible = true;}
									~MathGrapherLayer() {}
	void							addObject(MathGrapherObject *object, const QColor& color);
	void							clear();
	double*							getVertex(size_t num);
	double*							getNormals(size_t num);
	double*							getColors(size_t num);
	GLenum							getType(size_t num) const;
	size_t							getNumTypes() const;
	size_t							getNumVertex(size_t num) const;
	MathGrapherRegion				getRegion() const;
	void							update(const double& size);
	bool							visible() const;
	void							setVisible(bool yes);
	void							switchVisible();
private:
	std::vector<std::shared_ptr<MathGrapherObject> > objects;
	std::vector<QColor>             colors;
	std::vector<MathGrapherList>    generatedData;
	bool                            isVisible;
};

/* Класс для отрисовки элементов в трехмерном пространстве */
class MathGrapher : public QGLWidget
{
    Q_OBJECT
public:
    /* Если использовать этот конструктор, то область и начало отсчета вычисляются автоматически */
                                    MathGrapher();

    /* Настройка области отрисовки и начала отсчета */
    size_t                          addLayer();
    bool                            setLayer(size_t numLayer);
	void							prevLayer();
	void							nextLayer();
    size_t                          numberLayer() const;
    /* Добавление объекта и настройка их отображения */
    void                            addObject(MathGrapherObject *object, const QColor &color);
    void                            setSizeObjects(const double& size);
    void                            redraw();
    void                            setVisibleLayer(size_t numberLayer, bool yes);
    bool                            isVisibleLayer(size_t numberLayer) const;
	void							switchVisible();

    bool                            saveImage(const QString& fileName);
    void                            clear();
private:
    void                            setDefaultParameters();
    double                          horAngle;
    double                          verAngle;
    double                          scale;
    double                          dxMoveRegion, dyMoveRegion, dzMoveRegion;
    int                             mouseCurX;
    int                             mouseCurY;
    double                          mulX;
    double                          mulY;
    /* параметры сцены */
    MathGrapherRegion               availableRegion;
    double                          sizeObjects;
    /* элементы сцены */
    std::vector<MathGrapherLayer>   layers;
    size_t                          curLayer;
protected:
	virtual void					postDraw();
	virtual void					initializeGL();
	virtual void					resizeGL(int width, int height);
	virtual void					paintGL();
	virtual void					mousePressEvent(QMouseEvent *event);
	virtual void					mouseMoveEvent(QMouseEvent *event);
	virtual void					wheelEvent(QWheelEvent *event);
	virtual void					keyPressEvent(QKeyEvent *event);

};

#endif // MATHGRAPHER_H
