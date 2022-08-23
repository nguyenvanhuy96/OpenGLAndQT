#include "axbopenglwidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>



QPoint lastPos(0,0);
int perFrame = 100;// delta time / frame
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
    m_camera= Camera(QVector3D(0.0f, 0.0f, 3.0f));

    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();

}

void AXBOpenGLWidget::on_TimeOut()
{
    update();
}
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // shader
    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/obj.vert");
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

    // Texture
    texDiffuse = new QOpenGLTexture(QImage(":/Images/Resources/Images/diffuseMapTex.png").mirrored());
    texSpecular = new QOpenGLTexture(QImage(":/Images/Resources/Images/specularMapTex.png").mirrored());

    // Sử dụng mesh
    m_mesh= processMesh();

}
Mesh* AXBOpenGLWidget::processMesh()
{
    vector<Vertex> _vertices;
    vector<unsigned int> _indices;
    vector<Texture> _textures;
    // ==== Cach 1============
    for(int i=0; i<36; i++){
        Vertex vert;
        vert.Position[0]=vertices[i*8+0];
        vert.Position[1]=vertices[i*8+1];
        vert.Position[2]=vertices[i*8+2];

        vert.Normal[0]=vertices[i*8+3];
        vert.Normal[1]=vertices[i*8+4];
        vert.Normal[2]=vertices[i*8+5];

        vert.TexCoords[0]=vertices[i*8+6];
        vert.TexCoords[1]=vertices[i*8+7];

        _vertices.push_back(vert);
        _indices.push_back(i);
    }
    // --- cach 2---------
    //    memcmp(&_vertices[0],vertices,sizeof (vertices));
    //    for(int i=0; i<36; i++){
    //        _indices.push_back(i);
    //    }
    Texture tex;
    tex.id = texDiffuse->textureId();
    tex.type ="texture_diffuse";  //  material.texture_diffuse trong shader
    _textures.push_back(tex);

    tex.id = texSpecular->textureId();
    tex.type ="texture_specular"; //  material.texture_specular trong shader
    _textures.push_back(tex);

    return  new Mesh(
                QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(),
                _vertices, _indices, _textures
                );
}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

QVector3D material_ambient= QVector3D( 1.0f, 0.5f, 0.31f);
//QVector3D material_diffuse=QVector3D(1.0f, 0.5f, 0.31f);
//QVector3D material_specular=QVector3D(0.5f, 0.5f, 0.5f);

QVector3D light_ambient= QVector3D(0.2f, 0.2f, 0.2f);
QVector3D light_diffuse= QVector3D(0.5f, 0.5f, 0.5f);
QVector3D light_specular= QVector3D(1.0f, 1.0f, 1.0f);
QVector3D light_color = QVector3D(1.0f, 1.0f,0.0f);
QVector3D light_pos= QVector3D(0.0f, -1.0f, 5.0f);

void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    projection.setToIdentity();
    projection.perspective(m_camera.Zoom, (float)width()/height(), 0.1f, 100.0f);
    view.setToIdentity();
    view= m_camera.GetViewMatrix();
    //model.setToIdentity();
    model.rotate(10, QVector3D(1.0f, 1.0f, 0.0f));

    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("model", model);
    // material
    shaderProgram.setUniformValue("material.ambient", material_ambient);
    shaderProgram.setUniformValue("material.shininess", 32.0f);
    // view pos
    shaderProgram.setUniformValue("viewPos", m_camera.Position);
    // light
    shaderProgram.setUniformValue("light.ambient",light_ambient);
    shaderProgram.setUniformValue("light.diffuse", light_diffuse);
    shaderProgram.setUniformValue("light.specular", light_specular);
    shaderProgram.setUniformValue("light.lightColor", light_color);
    shaderProgram.setUniformValue("light.lightPos", light_pos);


    m_mesh->Draw(shaderProgram); // sử dụng mesh để draw

    lightshaderProgram.bind();
    modelLight.setToIdentity();
    modelLight.scale(0.2f);
    modelLight.translate(light_pos);
    lightshaderProgram.setUniformValue("projection", projection);
    lightshaderProgram.setUniformValue("view", view);
    lightshaderProgram.setUniformValue("model", modelLight);


    m_mesh->Draw(lightshaderProgram); // sử dụng mesh để draw
}

void AXBOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
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
