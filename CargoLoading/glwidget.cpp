#include "glwidget.h"
#include <QtGui/QImage>

#include <math.h>

/*
 * Sets up the OpenGL rendering context, defines display lists, etc.
 * Gets called once before the first time resizeGL() or paintGL() is called.
 */
void GLWidget::initializeGL(){
    //activate the depth buffer
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    m_pProject = NULL;

    path = NULL;
    NodeID = -1;

    aspect = 1;
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

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = - event->x() + lastPos.x();
    int dy = - event->y() + lastPos.y();

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
    aspect = (float)width/((height == 0) ? 1 : (float)height);

    glViewport( 0, 0, (GLint)width, (GLint)height );
 
    /* create viewing cone with near and far clipping planes */
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum( -1, 1, -1, 1, .5, 10.0);
    

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
    //glTranslatef(0.0f,0.0f,-20.0f); //move along z-axis
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glScalef(scale/(1+((aspect-1)/2)), scale*(1+((aspect-1)/2)), scale);

    if(path && m_pProject)
    {
        if(!path->m_Carrier){
            return;
        }

        float norm = sqrt(pow(path->m_Carrier->Width,2)+ pow(path->m_Carrier->Height,2) + pow(path->m_Carrier->Depth,2)) / 4;

        if (!path->m_Carrier->m_BoxList.empty())
        {
            float W = path->m_Carrier->Width  / (norm * .9f);
            float H = path->m_Carrier->Height / (norm * .9f);
            float D = path->m_Carrier->Depth  / (norm * .9f);
            W*=.9f;
            H*=.9f;
            D*=.9f;

            glTranslatef(-W/2,-H/2,-D/2);

            float scaleFactor = norm / 10000;

            drawCube(0,0,0,W,scaleFactor,scaleFactor,0,1,0);
            drawCube(0,H,0,W,scaleFactor,scaleFactor,0,1,0);
            drawCube(0,0,D,W,scaleFactor,scaleFactor,0,1,0);
            drawCube(0,H,D,W,scaleFactor,scaleFactor,0,1,0);

            drawCube(0,0,0,scaleFactor,H,scaleFactor,1,0,0);
            drawCube(0,0,D,scaleFactor,H,scaleFactor,1,0,0);
            drawCube(W,0,0,scaleFactor,H,scaleFactor,1,0,0);
            drawCube(W,0,D,scaleFactor,H,scaleFactor,1,0,0);

            drawCube(0,0,0,scaleFactor,scaleFactor,D,0,0,1);
            drawCube(W,0,0,scaleFactor,scaleFactor,D,0,0,1);
            drawCube(0,H,0,scaleFactor,scaleFactor,D,0,0,1);
            drawCube(W,H,0,scaleFactor,scaleFactor,D,0,0,1);

            drawCube(-scaleFactor,-2*scaleFactor,-scaleFactor,W,scaleFactor,D,0,0,0.4f);

            for (int j = 0; j < path->m_Carrier->m_BoxList.size() ; j++)
            {
                __BoxInContainer* tB = path->m_Carrier->m_BoxList.at(j);

                Node * TempNode = getNode(tB->NodeId);
                if (!TempNode)
                {
                    continue;
                }

                float r = 1;
                float g = 1;
                float b = 1;

                if(TempNode!=NULL)
                {
                    __BoxRow* Box = GetBox(TempNode->m_Order->BoxID);
                    if(Box != NULL)
                    {
                        QString Color =	Box->Color;
                        QStringList rgbList = Color.split(" ");
                        r = rgbList.at(0).toFloat();
                        g = rgbList.at(1).toFloat();
                        b = rgbList.at(2).toFloat();
                    }
                    else
                    {
                        continue;
                    }
                }

                drawCube(float(tB->x)/norm,float(tB->z)/norm,float(tB->y)/norm,float(tB->w*.9f)/norm,float(tB->d*.9f)/norm,float(tB->h*.9f)/norm,r,g,b);  
            }
        }
    }

 
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    e->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
    updateGL();
}

void GLWidget::drawCube(float x, float y, float z, float w, float h, float d, float r, float g, float b)
{
    float t = 0.4f;

    w *= .5f;
    h *= .5f;
    d *= .5f;

    glTranslatef(x+w,y+h,z+d);
    glBegin(GL_QUADS);

    //front
    glColor4f(r,g,b,t);

    glVertex3f( w , h , d);
    glVertex3f(-w , h , d);
    glVertex3f(-w ,-h , d);
    glVertex3f( w ,-h , d);


    //back

    glColor4f(r,g,b,t);

    glVertex3f( w , h ,-d);
    glVertex3f(-w , h ,-d);
    glVertex3f(-w ,-h ,-d);
    glVertex3f( w ,-h ,-d);


    //top
    glColor4f(r,g,b,t);

    glVertex3f(-w , h , d);
    glVertex3f( w , h , d);
    glVertex3f( w , h ,-d);
    glVertex3f(-w , h ,-d);


    //bottom
    glColor4f(r,g,b,t);

    glVertex3f( w ,-h , d);
    glVertex3f( w ,-h ,-d);
    glVertex3f(-w ,-h ,-d);
    glVertex3f(-w ,-h , d);

    //right
    glColor4f(r,g,b,t);

    glVertex3f( w , h , d);
    glVertex3f( w ,-h , d);
    glVertex3f( w ,-h ,-d);
    glVertex3f( w , h ,-d);


    //left
    glColor4f(r,g,b,t);

    glVertex3f(-w , h , d);
    glVertex3f(-w ,-h , d);
    glVertex3f(-w ,-h ,-d);
    glVertex3f(-w , h ,-d);


    glEnd();
    glTranslatef(-(x+w),-(y+h),-(z+d));
}

Node*	GLWidget::getNode	(int NodeID)
{
    if (!m_pProject)
    {
        return NULL;
    }
    for(int i = 0 ; i < m_pProject->m_AllPathes.count() ; i++)
    {
        Path* tempPath = m_pProject->m_AllPathes.at(i);
        for(int j = 0 ; j < tempPath->m_Node.count() ; j++)
        {
            Node* tempNode = tempPath->m_Node.at(j);
            if(tempNode->id == NodeID)
                return tempNode;
        }

    }
    return NULL;
}

__BoxRow* GLWidget::GetBox(int id)
{
    if (!m_pProject)
    {
        return NULL;
    }
    for(int i=0;i<m_pProject->m_CargoList.count();i++)
    {
        __BoxRow* box=m_pProject->m_CargoList.at(i);
        if(box->id==id)
            return box;
    }
    return NULL;
}