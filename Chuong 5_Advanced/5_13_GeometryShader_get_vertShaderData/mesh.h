#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <QVector3D>
#include <QVector2D>
using namespace std;

struct Vertex{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};
struct Texture{
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
public:
    // Mesh data
    vector<Vertex> verties;
    vector<unsigned int> indices;
    vector<Texture> textures;
    void Draw(QOpenGLShaderProgram &shader);
    Mesh(QOpenGLFunctions_3_3_Core *glFuns,
         vector<Vertex> vertives, vector<unsigned int> indices,
         vector<Texture> textures);
private:
    unsigned int VAO,VBO, EBO;
    void setupMesh();
    QOpenGLFunctions_3_3_Core *m_glFuns;
};

#endif // MESH_H
