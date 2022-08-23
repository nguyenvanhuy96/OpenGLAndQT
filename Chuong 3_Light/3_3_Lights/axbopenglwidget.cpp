#include "axbopenglwidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QtMath>
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
// positions all containers
QVector<QVector3D>  cubePositions = {
    QVector3D( 0.0f,  0.0f,  0.0f),
    QVector3D( 2.0f,  5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D( 2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f,  3.0f, -7.5f),
    QVector3D( 1.3f, -2.0f, -2.5f),
    QVector3D( 1.5f,  2.0f, -2.5f),
    QVector3D( 1.5f,  0.2f, -1.5f),
    QVector3D(-1.3f,  1.0f, -1.5f)
};
// positions of the point lights
QVector<QVector3D> pointLightPositions = {
    QVector3D( 0.7f,  0.2f,  2.0f),
    QVector3D( 2.3f, -3.3f, -4.0f),
    QVector3D(-4.0f,  2.0f, -12.0f),
    QVector3D( 0.0f,  0.0f, -3.0f)
};
void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // shader
    // VAO và VBO
    // Bước 1: khởi tạo Id của trợ lỹ
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    // Bước 2: khởi tạo trợ lý
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Bước 3: Set dữ liệu cho VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    // Bước 4: Giải thích dữ liệu cho GPU (attribpointer)
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)(3*sizeof (float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)(6* sizeof (float)));
    glEnableVertexAttribArray(2);

    // Bước 6: Tạm ngưng ghi dữ liệu VAO và VBO(ghi dữ liệu xong- giải phóng trợ lý)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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


shaderProgram.bind();
// Texture
texDiffuse = new QOpenGLTexture(QImage(":/Images/Resources/Images/diffuseMapTex.png").mirrored());
texSpecular = new QOpenGLTexture(QImage(":/Images/Resources/Images/specularMapTex.png").mirrored());
// set cổng trong shader
shaderProgram.setUniformValue("material.diffuse",0);
shaderProgram.setUniformValue("material.specular", 1);

}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

//QVector3D material_ambient= QVector3D( 1.0f, 0.5f, 0.31f);
//QVector3D material_diffuse=QVector3D(1.0f, 0.5f, 0.31f);
//QVector3D material_specular=QVector3D(0.5f, 0.5f, 0.5f);

//QVector3D light_ambient= QVector3D(0.2f, 0.2f, 0.2f);
//QVector3D light_diffuse= QVector3D(0.5f, 0.5f, 0.5f);
//QVector3D light_specular= QVector3D(1.0f, 1.0f, 1.0f);
//QVector3D light_color = QVector3D(1.0f, 1.0f,0.0f);
//QVector3D light_pos= QVector3D(0.0f, -1.0f, 5.0f);

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
    // model.rotate(10, QVector3D(1.0f, 1.0f, 0.0f));

    //      light properties
    //     lightingShader.setFloat("light.constant", 1.0f);
    //     lightingShader.setFloat("light.linear", 0.09f);
    //     lightingShader.setFloat("light.quadratic", 0.032f);
    //     lightingShader.setFloat("material.shininess", 32.0f);

    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", view);

    // material
    shaderProgram.setUniformValue("material.ambient", 1.0f, 0.5f, 0.31f);
    texDiffuse->bind(0);
    texSpecular->bind(1);
    shaderProgram.setUniformValue("material.shininess", 32.0f);
    // view pos
    shaderProgram.setUniformValue("viewPos", m_camera.Position);

    // directional light
    shaderProgram.setUniformValue("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shaderProgram.setUniformValue("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setUniformValue("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shaderProgram.setUniformValue("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shaderProgram.setUniformValue("pointLights[0].lightPos", pointLightPositions[0]);
    shaderProgram.setUniformValue("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setUniformValue("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setUniformValue("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("pointLights[0].quadratic", 0.032f);
    shaderProgram.setUniformValue("pointLights[0].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[0].constant", 1.0f);


    // point light 2
    shaderProgram.setUniformValue("pointLights[1].lightPos", pointLightPositions[1]);
    shaderProgram.setUniformValue("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setUniformValue("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setUniformValue("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("pointLights[1].quadratic", 0.032f);
    shaderProgram.setUniformValue("pointLights[1].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[1].constant", 1.0f);


    // point light 3
    shaderProgram.setUniformValue("pointLights[2].lightPos", pointLightPositions[2]);
    shaderProgram.setUniformValue("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setUniformValue("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setUniformValue("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("pointLights[2].quadratic", 0.032f);
    shaderProgram.setUniformValue("pointLights[2].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[2].constant", 1.0f);


    // point light 4
    shaderProgram.setUniformValue("pointLights[3].lightPos", pointLightPositions[3]);
    shaderProgram.setUniformValue("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setUniformValue("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setUniformValue("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("pointLights[3].quadratic", 0.032f);
    shaderProgram.setUniformValue("pointLights[3].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[3].constant", 1.0f);


    // spotLight
    shaderProgram.setUniformValue("spotLight.lightPos", pointLightPositions[3]);
    shaderProgram.setUniformValue("spotLight.direction", m_camera.Front);
    shaderProgram.setUniformValue("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shaderProgram.setUniformValue("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("spotLight.constant", 1.0f);
    shaderProgram.setUniformValue("spotLight.linear", 0.09f);
    shaderProgram.setUniformValue("spotLight.quadratic", 0.032f);
    float cutoffvalue = cos(12.5f *3.14159/180);
    shaderProgram.setUniformValue("spotLight.cutOff",cutoffvalue );
    float outercutoffvalue = cos(15.0f *3.14159/180);
    shaderProgram.setUniformValue("spotLight.outerCutOff", outercutoffvalue);


    // draw cubes
    foreach (QVector3D pos, cubePositions) {
        model.setToIdentity();
        model.translate(pos);
        shaderProgram.setUniformValue("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // draw point lights
    lightshaderProgram.bind();
    lightshaderProgram.setUniformValue("projection", projection);
    lightshaderProgram.setUniformValue("view", view);
    foreach (QVector3D pos, pointLightPositions) {
        modelLight.setToIdentity();
        modelLight.translate(pos);
        modelLight.scale(0.2f);
        lightshaderProgram.setUniformValue("model", modelLight);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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
