#pragma once
#include <vector>
#include <glm.hpp>
#include "Shader.h"
//#include "Entity.h"
#include "WindowDefs.h"
#include "Cameras.h"
#include <gtx/euler_angles.hpp>
#define TRI 3
#define QUAD 4
struct Transform {
    Transform() {
        translation = glm::vec3(0, 0, 0);
        rotation = glm::vec3(0, 0, 0);
        scale = glm::vec3(1, 1, 1);
    }
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 getModelMatrix() {
        const glm::mat4 translatemat = glm::translate(glm::mat4(1.0), translation);
        const glm::mat4 scalemat = glm::scale(glm::mat4(1.0), scale);
        const glm::mat4 rotatemat = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));

        return (translatemat * rotatemat * scalemat);
    }
};
enum class TextureType{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    NORMALMAP
};
struct Texture {
    TextureType type;
    unsigned int id;
};
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float shininess_strength;
    std::vector<Texture> textures;
};
struct Vertex {
    float x,y,z;
    float normx,normy,normz;
    float u, v;
    
};
class Mesh {
public:
    std::vector<Vertex>             vertices;
    std::vector<unsigned int>       faceIndices;
    int                             materialIndex;
    bool                            hasnormals;
    bool                            hasuvs;
    void                            genBuffers(void);
    GLuint                          getVAO() { return myVAO; }
    void                            draw(Shader * shader, bool hasTextures, Material material);
private:
    
    GLuint myVAO;
};
struct ModelDrawNode {
    ModelDrawNode() {
        myTransform = glm::mat4(1.0);
        myParent = nullptr;
    }
    std::vector<ModelDrawNode*> myChildren;
    ModelDrawNode*              myParent;
    std::vector<Mesh*>          myMeshes;
    glm::mat4                   myTransform;
};
class Model {
public:
    friend class ModelLoader;
    Model();
    ~Model();
    void                  draw(Shader*,Transform);
    void                  recursiveDraw(ModelDrawNode * targetParent, glm::mat4 accTransform, Shader* shader);
    void                  setUniforms(Transform, Shader*);
    void                  genBuffers();
    bool                  hasTextures;
    Material*             getActiveMaterial(const int meshindex){ return &materials[meshes[meshindex].materialIndex]; }
    void                  pushMaterial(Material m) { materials.push_back(m); }
    void                  pushMesh(Mesh m) { meshes.push_back(m); }

    
private:
    std::vector<Mesh>     meshes;
    std::vector<Material> materials;
    ModelDrawNode*        drawnode = nullptr;

};

