#include "glwidget.h"
#include <QtGui/QImage>

#include <math.h>

/*
 * Sets up the OpenGL rendering context, defines display lists, etc.
 * Gets called once before the first time resizeGL() or paintGL() is called.
 */
void GLWidget::initializeGL(){
    //activate the depth buffer
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    rot_x = 0;
    rot_y = 0;
    rot_z = 0;

    last_rot_x = 0;
    last_rot_y = 0;
    last_rot_z = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    scale = 1.0;
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        updateGL();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}
 
 
/*
 *  Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
 *  (and also when it is shown for the first time because all newly created widgets get a resize event automatically).
 */
void GLWidget::resizeGL (int width, int height){
    glViewport( 0, 0, (GLint)width, (GLint)height );
 
    /* create viewing cone with near and far clipping planes */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 30.0);
 
    glMatrixMode( GL_MODELVIEW );
}
 
 
 
/*
 * Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
 */
void GLWidget::paintGL(){
 
    //delete color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-20.0f); //move along z-axis
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glScalef(scale, scale, scale);
 
    drawCube(2,0,0,2.0,1.0,1.0,.8,.8,.8);  
    drawCube(-2.1,0,0,2.0,1.0,1.0,1.0,0,0);  
 
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    e->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
    updateGL();
}

void GLWidget::drawCube(float x, float y, float z, float w, float h, float d, float r, float g, float b)
{
    float t = 0.8f;
    /* create 3D-Cube */
    glBegin(GL_QUADS);

    //front
    glColor4f(r,g,b,t);

    glVertex3f( w - x, h - y, d - z);
    glVertex3f(-w - x, h - y, d - z);
    glVertex3f(-w - x,-h - y, d - z);
    glVertex3f( w - x,-h - y, d - z);


    //back

    glColor4f(r,g,b,t);

    glVertex3f( w - x, h - y,-d - z);
    glVertex3f(-w - x, h - y,-d - z);
    glVertex3f(-w - x,-h - y,-d - z);
    glVertex3f( w - x,-h - y,-d - z);


    //top
    glColor4f(r,g,b,t);

    glVertex3f(-w - x, h - y, d - z);
    glVertex3f( w - x, h - y, d - z);
    glVertex3f( w - x, h - y,-d - z);
    glVertex3f(-w - x, h - y,-d - z);


    //bottom
    glColor4f(r,g,b,t);

    glVertex3f( w - x,-h - y, d - z);
    glVertex3f( w - x,-h - y,-d - z);
    glVertex3f(-w - x,-h - y,-d - z);
    glVertex3f(-w - x,-h - y, d - z);

    //right
    glColor4f(r,g,b,t);

    glVertex3f( w - x, h - y, d - z);
    glVertex3f( w - x,-h - y, d - z);
    glVertex3f( w - x,-h - y,-d - z);
    glVertex3f( w - x, h - y,-d - z);


    //left
    glColor4f(r,g,b,t);

    glVertex3f(-w - x, h - y, d - z);
    glVertex3f(-w - x,-h - y, d - z);
    glVertex3f(-w - x,-h - y,-d - z);
    glVertex3f(-w - x, h - y,-d - z);


    glEnd();
}
