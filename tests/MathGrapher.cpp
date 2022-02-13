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

#include <stdint.h>
#include <GL/gl.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "MathGrapher.h"
#include "GLFunctions.h"

MathGrapherRegion::MathGrapherRegion(const MathVector3D<double>& begin, const MathVector3D<double>& end)
{
	setDim(begin.getX(), end.getX(), 0);
	setDim(begin.getY(), end.getY(), 1);
	setDim(begin.getZ(), end.getZ(), 2);
}
const MathVector3D<double>		MathGrapherRegion::begin() const                                       /* начало области */
{
	return MathVector3D<double>(minDim(0), minDim(1), minDim(2));
}

double MathGrapherRegion::dx() const                                          /* размер по оси X */
{
	return maxDim(0) - minDim(0);
}

double MathGrapherRegion::dy() const                                          /* размер по оси Y */
{
	return maxDim(1) - minDim(1);
}

double MathGrapherRegion::dz() const                                          /* размер по оси Z */
{
	return maxDim(2) - minDim(2);
}

void MathGrapherLayer::addObject(MathGrapherObject *object, const QColor& color)
{
	objects.push_back(std::shared_ptr<MathGrapherObject>(object));
	colors.push_back(color);
}

void MathGrapherLayer::clear()
{
	objects.clear();
	colors.clear();
	generatedData.clear();
	isVisible = true;
}

double*	MathGrapherLayer::getVertex(size_t num)
{
	return generatedData[num].vertex.data();
}

double*	MathGrapherLayer::getNormals(size_t num)
{
	return generatedData[num].normals.data();
}

double* MathGrapherLayer::getColors(size_t num)
{
	return generatedData[num].colors.data();
}

GLenum MathGrapherLayer::getType(size_t num) const
{
	return generatedData[num].type;
}

size_t MathGrapherLayer::getNumTypes() const
{
	return generatedData.size();
}

size_t MathGrapherLayer::getNumVertex(size_t num) const
{
	return generatedData[num].vertex.size()/3;
}

MathGrapherRegion MathGrapherLayer::getRegion() const
{
	MathGrapherRegion region;

	for(size_t i = 0; i < objects.size(); i++)
		region += objects[i]->getRegion();
	return region;
}

void MathGrapherLayer::update(const double& size)
{
	size_t num, count;

	generatedData.clear();
	for(size_t i=0; i<objects.size(); i++) {
		generatedData.push_back(MathGrapherList(objects[i]->getTypeSurfaces()));
		num = generatedData.size() - 1;
		count = objects[i]->getSurface(generatedData[num].vertex, generatedData[num].normals, size);
		for(size_t j = 0; j < count; j++) {
			generatedData[num].colors.push_back(colors[i].red()/255.0);
			generatedData[num].colors.push_back(colors[i].green()/255.0);
			generatedData[num].colors.push_back(colors[i].blue()/255.0);
			generatedData[num].colors.push_back(colors[i].alpha()/255.0);
		}
	}
}

bool MathGrapherLayer::visible() const
{
	return isVisible;
}

void MathGrapherLayer::setVisible(bool yes)
{
	isVisible = yes;
}

void MathGrapherLayer::switchVisible()
{
	isVisible = (isVisible)? false : true;
}

MathGrapher::MathGrapher()
{
    setDefaultParameters();

    setMinimumSize(640,480);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

size_t MathGrapher::addLayer()
{
    layers.push_back(MathGrapherLayer());
    curLayer = layers.size() - 1;
    return layers.size() - 1;
}

bool MathGrapher::setLayer(size_t numLayer)
{
    if(numLayer >= layers.size()) return false;
    curLayer = numLayer;
    return true;
}

void MathGrapher::prevLayer()
{
	if (layers.empty())
		return;
	if (curLayer != 0)
		curLayer--;
	else
		curLayer = layers.size() - 1;
}

void MathGrapher::nextLayer()
{
	curLayer++;
	if (curLayer >= layers.size())
		curLayer = 0;
}

size_t MathGrapher::numberLayer() const
{
    return layers.size();
}

void MathGrapher::addObject(MathGrapherObject *object, const QColor& color)
{
    layers[curLayer].addObject(object, color);
}

void MathGrapher::setSizeObjects(const double &size)
{
    sizeObjects = size;
}

void MathGrapher::redraw()
{
    double size;

    availableRegion = MathGrapherRegion();
    for(size_t i=0; i<layers.size(); i++) availableRegion+=layers[i].getRegion();
    if(availableRegion.dx() + availableRegion.dy() + availableRegion.dz() == 0.0) scale = 1.0;
    else scale = 10.0/(availableRegion.dx() + availableRegion.dy() + availableRegion.dz());
    if(sizeObjects == 0.0)
    {   
        size = sqrt(availableRegion.dx()*availableRegion.dx() + availableRegion.dy()*availableRegion.dy() + availableRegion.dz()*availableRegion.dz())/600.0;
        if(size == 0.0) size = 0.01;
    }
    else size = sizeObjects;
    for(size_t i=0; i<layers.size(); i++) layers[i].update(size);
}

void MathGrapher::setVisibleLayer(size_t numberLayer, bool yes)
{
    layers[numberLayer].setVisible(yes);
}

bool MathGrapher::isVisibleLayer(size_t numberLayer) const
{
    return layers[numberLayer].visible();
}

void MathGrapher::switchVisible()
{
	layers[curLayer].switchVisible();
}

bool MathGrapher::saveImage(const QString &fileName)
{
    QImage image = grabFrameBuffer();

    return image.save(fileName);
}

void MathGrapher::clear()
{
    layers.clear();
    setDefaultParameters();
}

void MathGrapher::setDefaultParameters()
{
    horAngle = 0.0;
    verAngle = -90.0;
    scale = 1.0;
    dxMoveRegion = dyMoveRegion = 0.0;
    dzMoveRegion = -50.0;
    mulX = mulY = 1.0;

    availableRegion = MathGrapherRegion();

    layers.push_back(MathGrapherLayer());
    curLayer = 0;
    sizeObjects = 0;
}

void MathGrapher::postDraw()
{

}

void MathGrapher::initializeGL()
{
	GLfloat lightAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat lightSpec[] = {0.8f, 0.8f, 0.8f, 1.0f};

	GLfloat materialAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat materialSpecular[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat shinines = 128.0;

	glShadeModel(GL_SMOOTH);
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_RESCALE_NORMAL);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinines);
    glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    glEnable(GL_LIGHT0);
}

void MathGrapher::resizeGL(int width, int height)
{
    if(width > height)
        mulX = (double)(width) / (double)(height);
    else mulY = (double)(height) / (double)(width);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0*mulX, 1.0*mulX, -1.0*mulY, 1.0*mulY, 10.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
    updateGL();
}

void MathGrapher::paintGL()
{
    double sizeFont = AXIS_LEN/10.0;
    double dxCurrent, dyCurrent, dzCurrent;
	GLfloat lightPos[4] = {1.0, 1.0, -1.0, 1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    dxCurrent = -(availableRegion.begin().getX() + availableRegion.dx()/2.0);
    dyCurrent = -(availableRegion.begin().getY() + availableRegion.dy()/2.0);
    dzCurrent = -(availableRegion.begin().getZ() + availableRegion.dz()/2.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glTranslated(dxMoveRegion, dyMoveRegion, dzMoveRegion);
    glScaled(scale, scale, scale);
    glRotated(verAngle, 1.0, 0.0, 0.0);
    glRotated(horAngle, 0.0, 0.0, 1.0);
    glTranslated(dxCurrent, dyCurrent, dzCurrent);
    /* рисование координатных осей */
    glPushMatrix();
    glLoadIdentity();
    glTranslated(0, 0.0, -12.0);
    glTranslated(-mulX, -mulY, 0.0);
    glRotated(verAngle, 1.0, 0.0, 0.0);
    glRotated(horAngle, 0.0, 0.0, 1.0);

    glBegin(GL_LINES);
    glColor3d(0.0, 0.0, 0.0);
    /* Z */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, AXIS_LEN);
    /* Y */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, AXIS_LEN, 0.0);
    /* X */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(AXIS_LEN, 0.0, 0.0);

    /* рисование подписй к осям */
    glColor3d(0.0, 0.0, 255.0);
    /* Z */
    glVertex3d(0.0 - sizeFont, 0.0, AXIS_LEN + 4*sizeFont);
    glVertex3d(0.0 + sizeFont, 0.0, AXIS_LEN + 4*sizeFont);
    glVertex3d(0.0 + sizeFont, 0.0, AXIS_LEN + 4*sizeFont);
    glVertex3d(0.0 - sizeFont, 0.0, AXIS_LEN);
    glVertex3d(0.0 - sizeFont, 0.0, AXIS_LEN);
    glVertex3d(0.0 + sizeFont, 0.0, AXIS_LEN);
    /* Y */
    glVertex3d(0.0, AXIS_LEN - sizeFont, 0.0);
    glVertex3d(0.0, AXIS_LEN, - 2*sizeFont);
    glVertex3d(0.0, AXIS_LEN + sizeFont, 0.0);
    glVertex3d(0.0, AXIS_LEN - sizeFont, 0.0 - 4*sizeFont);
    /* X */
    glVertex3d(AXIS_LEN - sizeFont, 0.0, 0.0);
    glVertex3d(AXIS_LEN + sizeFont, 0.0, 0.0 - 4*sizeFont);
    glVertex3d(AXIS_LEN + sizeFont, 0.0, 0.0);
    glVertex3d(AXIS_LEN - sizeFont, 0.0, 0.0 - 4*sizeFont);
    glEnd();
    glPopMatrix();
    /* рисование объектов */
    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState((GL_NORMAL_ARRAY));
    for(size_t i=0; i<layers.size(); i++)
        if(layers[i].visible())
        {
            for(size_t j=0; j<layers[i].getNumTypes(); j++)
            {
                glVertexPointer(3, GL_DOUBLE, 0, layers[i].getVertex(j));
                glColorPointer(4, GL_DOUBLE, 0, layers[i].getColors(j));
                glNormalPointer(GL_DOUBLE, 0, layers[i].getNormals(j));
                glDrawArrays(layers[i].getType(j), 0, layers[i].getNumVertex(j));
            }
        }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_LIGHTING);

}

void MathGrapher::mousePressEvent(QMouseEvent *event)
{
    mouseCurX = event->pos().x();
    mouseCurY = event->pos().y();
}

void MathGrapher::mouseMoveEvent(QMouseEvent *event)
{
    int dx, dy;
    dx = event->pos().x() - mouseCurX;
    dy = event->pos().y() - mouseCurY;
    mouseCurX = event->pos().x();
    mouseCurY = event->pos().y();
    horAngle = horAngle + (double)dx * 3.14/18.0;
    verAngle = verAngle + (double)dy * 3.14/18.0;
    updateGL();
}

void MathGrapher::wheelEvent(QWheelEvent *event)
{
    scale *= pow(10, (double)event->delta()/800.0);
    updateGL();
}

void MathGrapher::keyPressEvent(QKeyEvent *event)
{
    size_t number = 0;

    switch(event->key())
    {
    case Qt::Key_Up:
        dyMoveRegion += 1.0;
        break;

    case Qt::Key_Down:
        dyMoveRegion -= 1.0;
        break;

    case Qt::Key_Left:
        dxMoveRegion -= 1.0;
        break;

    case Qt::Key_Right:
        dxMoveRegion += 1.0;
        break;

    case Qt::Key_1:
        number = 1; break;
    case Qt::Key_2:
        number = 2; break;
    case Qt::Key_3:
        number = 3; break;
    case Qt::Key_4:
        number = 4; break;
    case Qt::Key_5:
        number = 5; break;
    case Qt::Key_6:
        number = 6; break;
    case Qt::Key_7:
        number = 7; break;
    case Qt::Key_8:
        number = 8; break;
    case Qt::Key_9:
        number = 9; break;
	case Qt::Key_Minus:
		prevLayer(); break;
	case Qt::Key_Equal:
		nextLayer(); break;
	case Qt::Key_Space:
		switchVisible(); break;
    }
    if(number && numberLayer() >= number)
    {
        if(isVisibleLayer(number - 1)) setVisibleLayer(number - 1, false);
        else setVisibleLayer(number - 1, true);
    }


    updateGL();
}

//EOF
