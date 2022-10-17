#include "axbopenglwidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>



QPoint lastPos(0,0);
int perFrame = 100;// delta time / frame
float _near = 0.1f;
float _far = 100.0f;
AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    /*
        Cho phép kích hoạt sử dụng event keypress
        nếu khong có dòng này mà vẫn có code
        event key press thì nó cũng không chạy được
     */
    setFocusPolicy(Qt::StrongFocus);
    /* Cho phép kích hoạt sự kiện liên quan đến mouse*/
    //setMouseTracking(true);
    m_camera= Camera(QVector3D(0.0f, 2.0f, 9.0f));

    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();

}

void AXBOpenGLWidget::on_TimeOut()
{
    update();
}

float points[] = {
    -0.5f,  0.5f, // top-left
     0.5f,  0.5f, // top-right
     0.5f, -0.5f, // bottom-right
    -0.5f, -0.5f  // bottom-left
};

void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // shader
    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/obj.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/Shaders/Resources/Shaders/obj.geom");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/obj.frag");
    shaderProgram.link();
    success = shaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << shaderProgram.log();

    lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/light.vert");
    lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/light.frag");
    lightshaderProgram.link();
    success = lightshaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << lightshaderProgram.log();

    floorshaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/floor.vert");
    floorshaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/floor.frag");
    floorshaderProgram.link();
    success = floorshaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << floorshaderProgram.log();

    // Texture
    texDiffuse = new QOpenGLTexture(QImage(":/Images/Resources/Images/diffuseMapTex.png").mirrored());
    texSpecular = new QOpenGLTexture(QImage(":/Images/Resources/Images/co.png").mirrored());

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    model.setToIdentity();
}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

QVector3D material_ambient= QVector3D( 1.0f, 0.5f, 0.31f);
//QVector3D material_diffuse=QVector3D(1.0f, 0.5f, 0.31f);
//QVector3D material_specular=QVector3D(0.5f, 0.5f, 0.5f);

QVector3D light_ambient= QVector3D(0.9f, 0.9f, 0.9f);
QVector3D light_diffuse= QVector3D(0.5f, 0.5f, 0.5f);
QVector3D light_specular= QVector3D(1.0f, 1.0f, 1.0f);
QVector3D light_color = QVector3D(1.0f, 1.0f,0.0f);
QVector3D light_pos= QVector3D(0.0f, -1.0f, 5.0f);

void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    projection.setToIdentity();
    projection.perspective(m_camera.Zoom, (float)width()/height(), 0.1f, 100.0f);
    view.setToIdentity();
    view= m_camera.GetViewMatrix();
    model.setToIdentity();

    shaderProgram.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);
}

void AXBOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    makeCurrent();
    if(event->buttons() && Qt::LeftButton){
        float winz; // độ sâu của điểm trên viewport khi pick
        int screenX= event->pos().x();
        //  event->pos:toạ độ điểm lấy theo điểm gốc là (trên,trái)
        // sử dụng glReadPixels thì toạ độ điểm x,y phải tính từ điểm gốc (dưới,trái).
        int screenY= this->height()- event->pos().y();
        glReadPixels(screenX, screenY,1,1, GL_DEPTH_COMPONENT,GL_FLOAT,&winz);

        // Chuyển đổi toạ độ ViewSpace(màn hình) sang toạ độ NDC
        float x= (2*event->pos().x())/this->width()-1.0f;
        float y = 1.0f-(2.0f*event->pos().y())/this->height();
        float z= winz*2.0f -1.0f;

        if(winz<1.0f){
            float w= (2.0 * _near*_far)/(_far+_near-z*(_far- _near));
            qDebug() << "x: " << x << ",y: " << y << ",z: " <<z << ",w: " << w ;
        }
    }
    doneCurrent();
}


void AXBOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    auto posIn= event->pos();
    // reversed since y-coordinates go from bottom to top
    int xoffset = posIn.x() - lastPos.x();
    int yoffset = lastPos.y() - posIn.y();

    lastPos=posIn;
    m_camera.ProcessMouseMovement(xoffset, yoffset);
    update();
}

void AXBOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    m_camera.ProcessMouseScroll( event->angleDelta().y()/120);
    update();
}

void AXBOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float detatime2= 2.5*perFrame/1000;
    switch (event->key()) {
    case Qt::Key_W:m_camera.ProcessKeyboard(FORWARD,detatime2); break;
    case Qt::Key_S:m_camera.ProcessKeyboard(BACKWARD,detatime2); break;
    case Qt::Key_D:m_camera.ProcessKeyboard(RIGHT,detatime2); break;
    case Qt::Key_A:m_camera.ProcessKeyboard(LEFT,detatime2); break;
    }
    update();
}

Mesh *AXBOpenGLWidget::processMesh(float *vertices, int size, unsigned int textureId)
{
    vector<Vertex> _vertices;
    vector<unsigned int> _indices;
    vector<Texture> _textures;
    for (int i=0; i<size; i++) {
        Vertex vert;
        vert.Position[0]=vertices[i*5+0];
        vert.Position[1]=vertices[i*5+1];
        vert.Position[2]=vertices[i*5+2];

        vert.TexCoords[0]=vertices[i*5+3];
        vert.TexCoords[1]=vertices[i*5+4];
        _vertices.push_back(vert);
        _indices.push_back(i);
    }
    Texture tex;
    tex.id= textureId;
    tex.type ="texture_specular";
    _textures.push_back(tex);
    return  new Mesh(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(), _vertices, _indices,_textures);
}
