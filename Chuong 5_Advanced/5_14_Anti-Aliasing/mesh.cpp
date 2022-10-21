#include "mesh.h"

Mesh::Mesh(QOpenGLFunctions_3_3_Core *glFuns, vector<Vertex> vertives, vector<unsigned int> indices, vector<Texture> textures)
{
    this->verties= vertives;
    this->indices= indices;
    this->textures = textures;
    this->m_glFuns= glFuns;
    setupMesh();
}

void Mesh::setupMesh()
{
    m_glFuns->glGenVertexArrays(1,&VAO);
    m_glFuns->glGenBuffers(1,&VBO);
    m_glFuns->glGenBuffers(1,&EBO);

    m_glFuns->glBindVertexArray(VAO);
    m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    m_glFuns->glBufferData(GL_ARRAY_BUFFER, verties.size()*sizeof (Vertex), &verties[0], GL_STATIC_DRAW);

    m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    m_glFuns->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof (unsigned int), &indices[0], GL_STATIC_DRAW);

    // Set atribute
    m_glFuns->glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)0);
    m_glFuns->glEnableVertexAttribArray(0);

    //    m_glFuns->glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)(3*sizeof (float)));
    //    m_glFuns->glEnableVertexAttribArray(1);

    //    m_glFuns->glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,8*sizeof (float),(void*)(6*sizeof (float)));
    //    m_glFuns->glEnableVertexAttribArray(2);

    m_glFuns->glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,sizeof (Vertex),(void*)offsetof(Vertex,Normal));
    m_glFuns->glEnableVertexAttribArray(1);

    m_glFuns->glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,sizeof (Vertex),(void*)offsetof(Vertex,TexCoords));
    m_glFuns->glEnableVertexAttribArray(2);

    //Giai phong
    //    m_glFuns->glBindBuffer(GL_ARRAY_BUFFER, 0); // Không cần đống EBO.Dữ liệu tự nén vào khi kết thúc VAO
    // m_glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // không cần đóng VBO. Dữ liệu tự nén vào khi kết thúc VAO
  m_glFuns->glBindVertexArray(0);
}


void Mesh::Draw(QOpenGLShaderProgram &shader)
{
    unsigned int diffuseNr =1;
    unsigned int specularNr =1;
    for(unsigned int i=0; i< textures.size(); i++){
        m_glFuns->glActiveTexture(GL_TEXTURE0+i);
        string number;
        string name=textures[i].type;
        if (name== "texture_diffuse"){
            number = std::to_string(diffuseNr++);
        }else if(name == "texture_specular"){
            number= std::to_string(specularNr++);
        }
        shader.setUniformValue(("material."+name+number).c_str(),i);
        m_glFuns->glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }
    m_glFuns->glBindVertexArray(VAO);
   //m_glFuns->glDrawArrays(GL_TRIANGLES,0,verties.size());
    m_glFuns->glDrawElements(GL_TRIANGLES,verties.size(),GL_UNSIGNED_INT, 0);
}
