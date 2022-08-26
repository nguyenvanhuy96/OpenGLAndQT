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

AXBOpenGLWidget::~AXBOpenGLWidget()
{
    glDeleteFramebuffers(1, &fbo);
}

void AXBOpenGLWidget::on_TimeOut()
{
    update();
}

float transparentVertices[] = {
    // positions         // texture Coords
    -1.0f,  -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -0.5f,  0.0f,  0.0f,  1.0f,

    -1.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,-0.5f,  0.0f,  1.0f,  1.0f,
    -0.5f, -1.0f,  0.0f,  1.0f,  0.0f,
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

    displayshaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/fbuffer.vert");
    displayshaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/fbuffer.frag");
    displayshaderProgram.link();
    success = displayshaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << displayshaderProgram.log();


    //Bước 1: Khởi tạo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // generate texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Attack textureColorbuffer vào framebuffer
    // Ở đây có thể hiểu sau khi glFramebufferTexture2D thì
    // textureColorbuffer chính là id của texture của viewBuffer
    //(đối tượng vẽ trong framebuffer- thường là màn hình hiển thị)
    // Khi thay đổi góc, vị trí,dữ liệu ..thì texture tự update dữ liệu.
    // Vì vậy khi set texture chỉ cần gán duy nhất 1 lần như texture thông thường.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // attach rbo
    //  unsigned int rbo;
    //    glGenRenderbuffers(1, &rbo);
    //    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    //    // use a single renderbuffer object for both a depth AND stencil buffer.
    //    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height());
    //    // Attack render buffer
    //    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    //    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    // Kích hoạt buffer mặc định, thao tác vẽ như bình thường
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    // Sử dụng mesh
    m_model= new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(),
                       "C:/Users/HUY NGUYEN/Desktop/OpenGL vs QT/OpenGLAndQT/Chuong 4_Assimp/backpack/backpack.obj");

    // Gán texture như thông thường. Sử dụng phương thức draw thì nó tự bind texture này
    // mà trong vòng lặp while không cần phải sử dụng bind() nữa
    //=> Đối tượng đó sẽ được hiển thị những gì được vẽ trong framebuffer
    windowMesh = processMesh(transparentVertices, 6, textureColorbuffer);

}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    projection.setToIdentity();
    projection.perspective(m_camera.Zoom, (float)width()/height(), 0.1f, 100.0f);
    view.setToIdentity();
    view= m_camera.GetViewMatrix();
    model.setToIdentity();


    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Sau khi kích hoạt fbo, đoạn code nằm ở dưới sẽ được vẽ vào trong fbo
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("model", model);
    // material
    shaderProgram.setUniformValue("material.ambient", QVector3D( 1.0f, 0.5f, 0.31f));
    shaderProgram.setUniformValue("material.shininess", 32.0f);
    // view pos
    shaderProgram.setUniformValue("viewPos", m_camera.Position);
    // light
    shaderProgram.setUniformValue("light.ambient",QVector3D(0.2f, 0.2f, 0.2f));
    shaderProgram.setUniformValue("light.diffuse",QVector3D(0.5f, 0.5f, 0.5f));
    shaderProgram.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("light.lightColor", QVector3D(1.0f, 1.0f,0.0f));
    shaderProgram.setUniformValue("light.lightPos", QVector3D(0.0f, -1.0f, 5.0f));
    // Draw Model
    m_model->Draw(shaderProgram); // sử dụng model để draw

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    // Sau khi kích hoạt framebuffer mặc định, những đoạn code sau sẽ được vẽ lên màn hình chính
    displayshaderProgram.bind();
    modelLight.setToIdentity();
    displayshaderProgram.setUniformValue("projection", projection);
    displayshaderProgram.setUniformValue("view", view);
    displayshaderProgram.setUniformValue("model", modelLight);
    /*
     * Vì windowMesh lúc trước người dùng đã gán texture(framebuffer) vào đối tượng này rồi
     * Nên các hiệu ứng hiển thị Post-processing sẽ được viết vào trong displayshaderProgram.
    */
    windowMesh->Draw(displayshaderProgram);

    shaderProgram.bind();
    m_model->Draw(shaderProgram);
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


