#include "model.h"
using namespace std;
#include <QDebug>
#include <QOpenGLTexture>
void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      qDebug() << "ERROR::ASSIMP::" << import.GetErrorString()<< endl;
        return;
    }
    directory = path.substr(0,path.find_last_of('/'));
    processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i=0; i< node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }
    // the do the same for each of its children
    for(unsigned int i=0; i< node->mNumChildren; i++){
        processNode(node->mChildren[i],scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        Vertex vert;

        QVector3D vector;
        vector.setX( mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vert.Position= vector;

        vector.setX( mesh->mNormals[i].x);
        vector.setY( mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vert.Normal= vector;

        QVector2D vec= QVector2D(0.0f, 0.0f);
        if(mesh->mTextureCoords[0]){
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
        }
        vert.TexCoords= vec;
        vertices.push_back(vert);
    }
    // faces
    for (unsigned int i=0;i< mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j=0;j< face.mNumIndices;j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // material
    if(mesh->mMaterialIndex>=0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps= loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return  Mesh(glFuncs,vertices,indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
     vector<Texture> textures;
     for(unsigned int i=0; i< mat->GetTextureCount(type); i++){
         aiString str;
         mat->GetTexture(type, i, &str);

         bool skip = false;
         for(unsigned int j=0 ; j< textures_loaded.size(); j++){
             if(std::strcmp(textures_loaded[j].path.data(), str.C_Str())==0){
                 textures.push_back(textures_loaded[j]);
                 skip =true;
                 break;
             }
         }
         if(!skip){
             Texture texture;
             texture.id = TextureFromFile(str.C_Str(), directory);
             texture.type= typeName;
             texture.path = str.C_Str();
             textures.push_back(texture);
             textures_loaded.push_back(texture);
         }
     }
     return textures;

}

unsigned int Model::TextureFromFile(const char *path, const string &directory){
    string fileName = string(path);
    fileName = directory + '/' + fileName;
    QOpenGLTexture *texture = new QOpenGLTexture(QImage(fileName.c_str()).mirrored());
    if(texture == NULL) qDebug() << "texture is NULL";
    else qDebug() << fileName.c_str() << "loaded";
    return  texture->textureId();
}
