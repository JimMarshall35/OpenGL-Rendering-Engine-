#include "ModelDefs.h"
#include "ErrorHandliong.h"


Model::Model()
{
}

Model::~Model()
{
}

void Model::draw(Shader* shader, Transform transform)
{

    setUniforms(transform, shader);
    glm::mat4 model = transform.getModelMatrix();
    recursiveDraw(drawnode, model, shader);
    /*
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh mesh = meshes[i];
        Material mat = materials[mesh.materialIndex];
        mesh.draw(shader, hasTextures, mat);
    }
    */
}
void Model::recursiveDraw(ModelDrawNode* targetParent, glm::mat4 accTransform,Shader* shader) {
    // to do - accumulate transforms properly
    accTransform = accTransform*targetParent->myTransform;
    for (size_t i = 0; i < targetParent->myMeshes.size(); i++) {
        Mesh* mesh = targetParent->myMeshes[i];
        Material mat = materials[mesh->materialIndex];
        shader->setMat4("model", accTransform);
        mesh->draw(shader, hasTextures, mat);
    }
    for (size_t i = 0; i < targetParent->myChildren.size(); i++)
    {
        ModelDrawNode* child = targetParent->myChildren[i];
        recursiveDraw(child, accTransform, shader);
    }
}
void Model::setUniforms(Transform transform, Shader* shader)
{
    GLClearErrors();



    const glm::mat4 modelmatrix = transform.getModelMatrix();
    shader->setMat4("model", modelmatrix);
    GLPrintErrors("set uniforms - model");


    shader->setMat4("view", Cameras::instance().getActive()->GetViewMatrix());//Cameras::instance().getActive()->GetViewMatrix());
    GLPrintErrors("set uniforms - view");
    glm::mat4 projectionmatrix = glm::perspective(
        (float)glm::radians(FOV),
        ASPECT_RATIO,
        0.1f,
        100.0f
    );
    shader->setMat4("projection", projectionmatrix);
    GLPrintErrors("set uniforms - projection");
}

void Model::genBuffers()
{
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* m = &meshes[i];
        m->genBuffers();
    }
}




void Mesh::genBuffers(void)
{
    GLuint VBO;
    GLuint EBO;
    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndices.size() * sizeof(unsigned int), &faceIndices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    if (hasnormals) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normx));
        glEnableVertexAttribArray(1);
    }
    if (hasuvs) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
        glEnableVertexAttribArray(2);
    }

    myVAO = VAO;
}

void Mesh::draw(Shader * shader, bool hasTextures, Material material)
{
    
    shader->use();
    GLClearErrors();
    if (hasuvs && hasTextures) {
        for (int i = 0; i < material.textures.size(); i++) {
            Texture m = material.textures[i];
            switch (m.type) {
            case TextureType::SPECULAR:
                shader->setInt("material.specular", 1);
                glActiveTexture(GL_TEXTURE1);
                GLPrintErrors("glActiveTexture setting specular texture");
                glBindTexture(GL_TEXTURE_2D, m.id);
                GLPrintErrors("glBindTexture setting specular texture");
                break;
            case TextureType::AMBIENT:
            case TextureType::DIFFUSE:

                shader->setInt("material.diffuse", 0);
                glActiveTexture(GL_TEXTURE0);
                GLPrintErrors("glActiveTexture setting diffuse texture");
                glBindTexture(GL_TEXTURE_2D, m.id);
                GLPrintErrors("glBindTexture setting diffuse texture");
                break;
            
            }
            shader->setFloat("material.shininess", material.shininess);
            GLPrintErrors("setting shininess");
        }
    }
    else {
        shader->setVec3("material.ambient", material.ambient);
        GLPrintErrors("setting ambient colour");
        shader->setVec3("material.diffuse", material.diffuse);
        GLPrintErrors("setting diffuse colour");
        shader->setVec3("material.specular", material.specular);
        GLPrintErrors("setting specular colour");
        shader->setFloat("material.shininess", material.shininess);
        GLPrintErrors("setting shininess");
    }

    shader->setVec3("viewPos", Cameras::instance().getActive()->Position);
    GLPrintErrors(" setting shininess");
    glBindVertexArray(myVAO);
    GLPrintErrors("glBindVertexArray");
    glDrawElements(GL_TRIANGLES, faceIndices.size(), GL_UNSIGNED_INT, 0);
    GLPrintErrors("glDrawElements");
    glBindVertexArray(0);
    GLPrintErrors("glBindVertexArray");
}


