#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QSize>
#include <QMatrix4x4>
#include <QVector3D>
#include "requestinfo.h"
#include "infowrapper.h"


class GLWidget : public QOpenGLWidget, private QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent);
    QSize minimumSizeHint();
    QSize sizeHint();
    ~GLWidget();
public slots:
    void updataRequest(RequestInfo req);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
private:
    void cleanup();
    void setAllUnifromCompute();
    QOpenGLShaderProgram *m_programComputeVertices = nullptr;
    QOpenGLShaderProgram m_programRender;
    QOpenGLShaderProgram m_programComputeRearrange;



    float m_objR = 0;
    GLuint m_VAO = 0;
    GLuint m_test = 0;
    GLuint m_verticesData = 0;
    GLuint m_trigTable = 0;
    GLuint m_edgeVertTable = 0;
    GLuint m_debug = 0;
    GLuint m_temp = 0;

    GLuint m_isolevelLoc = 0;
    GLuint m_widthLoc = 0;

    GLuint m_worldLoc = 0;
    GLuint m_projLoc = 0;
    GLuint m_cameraLoc = 0;
    QMatrix4x4 m_world;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;

    GLuint m_xrange = 4;
    GLuint m_yrange = 4;
    GLuint m_zrange = 4;

    float m_width = 0.25;
    float m_isolevel = 0.0;
    unsigned int m_divide_num = 20;
    float m_offset = m_width * m_divide_num / 2;
    unsigned int m_centerNum = m_divide_num * m_divide_num * m_divide_num;
    const unsigned int m_work_group_size =  50;

    unsigned int m_verticesNum = 0;

    bool m_renewflag = false;
    float m_h = 20.0f;
    InfoWrapper *m_wrapper = nullptr;
};

#endif // GLWIDGET_H
