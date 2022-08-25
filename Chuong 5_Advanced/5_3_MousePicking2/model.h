#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

class Model
{
public:
    vector<Texture> textures_loaded;

    Model(QOpenGLFunctions_3_3_Core *glFuncs,const char *path){
        this->glFuncs= glFuncs;
        loadModel(path);
    };
    void Draw(QOpenGLShaderProgram &shader){
        for(unsigned int i=0; i <meshes.size(); i++){
            meshes[i].Draw(shader);
        }
    }
    float m_minX=100, m_maxX=-100, m_minY=100, m_maxY=0, m_minZ=100, m_maxZ=-100;
private:
    QOpenGLFunctions_3_3_Core *glFuncs;

    vector<Mesh> meshes;
    string directory;
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(const char *path, const string &directory);
};

#endif // MODEL_H
