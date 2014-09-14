#include <QtOpenGL>
#include "Project.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT // must include this if you use Qt signals/slots


public:
    GLWidget(QWidget *parent = NULL)
        : QGLWidget(parent) {}

    CProject* m_pProject;

    Path* path;
    int NodeID;

protected:
    // Set up the rendering context, define display lists etc.:
    void initializeGL();
    // draw the scene:
    void paintGL();
    // setup viewport, projection etc.:
    void resizeGL (int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *);

    void drawCube(float x, float y, float z, float w, float h, float d, float r, float g, float b);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

private:

    int rot_x;
    int rot_y;
    int rot_z;

    int last_rot_x;
    int last_rot_y;
    int last_rot_z;

    float scale;

    float aspect;

private:

    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    Node*	    getNode	(int NodeID);
    __BoxRow*   GetBox  (int id);

};

