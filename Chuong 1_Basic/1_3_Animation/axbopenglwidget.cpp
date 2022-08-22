#include "axbopenglwidget.h"
#include <QDebug>
AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_Timer.setInterval(500); // 50ms
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(timeoutFunc()));
    m_Timer.start();
    m_Time.start();
}
float vertices[]= {
    0.5f, 0.5f,0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f,   0.0f
};
unsigned int indices[]={
    0,1,3,
    1,2,3
};
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f,1.0f);\n"
                                   "}\0";


void AXBOpenGLWidget::drawShape(AXBOpenGLWidget::Shape shape)
{
    makeCurrent();
    m_shape= shape ;
    update();
    doneCurrent();

}

void AXBOpenGLWidget::setWireframe(bool wireframe)
{
    makeCurrent();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::timeoutFunc()
{
    if  (m_shape==None) return;
    makeCurrent();
    int timeValue = QTime(0, 0).secsTo(m_Time);
   // int timeValue= QTime::currentTime().msecsSinceStartOfDay();
    qDebug() << "time: " << timeValue;
    float greenValue =(sin(timeValue/2.0)+0.5f);
    update();
    doneCurrent();
}

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
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,3*sizeof (float),(void*)0);
    // Bươc 5: Gán AttribPointer(VAO) sang layout (location=0) của VertexShader(GPU)
    glEnableVertexAttribArray(0);

    // Bước 6: Tạm ngưng ghi dữ liệu VAO và VBO(ghi dữ liệu xong- giải phóng trợ lý)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // bước 5_2: EBO
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ===Vertex Shader =====
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success; char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL  ,infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }
    // == Fagment Shader ===
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader,GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragShader,512,NULL  ,infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }
    // == Shader program ==
    shaderProgram =glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram,fragShader );
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram,GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shaderProgram,512,NULL  ,infoLog);
        qDebug() << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog;
    }
    // == Xóa Vertex shader, và fragment shader ==
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}
void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(m_shape==Rect){
        glUseProgram( shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
    }
    qDebug() << "PAINT GL";

}
