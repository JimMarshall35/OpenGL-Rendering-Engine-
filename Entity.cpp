#include "Entity.h"
#include <glm.hpp>
#include <gtx/euler_angles.hpp>
#include "ModelLoader.h"
#include <iostream>
#include "ErrorHandliong.h"

Entity::Entity()
{
}


Entity::~Entity()
{
}


void Entity::setModel(std::string key) {

    modelname = key;
    model = ModelLoader::Instance()->getModel(key);
}




void Entity::draw(void)
{
    //setUniforms();
    GLClearErrors();
    model->draw(shader,transform);
    GLPrintErrors();
}
/*
void Entity::setUniforms() {
    GLClearErrors();
    const glm::mat4 translate = glm::translate(glm::mat4(1.0), transform.translation);
    const glm::mat4 scale  = glm::scale(glm::mat4(1.0), transform.scale);
    const glm::mat4 rotate = glm::eulerAngleXYZ(glm::radians(transform.rotation.x), glm::radians(transform.rotation.y), glm::radians(transform.rotation.z));

    glm::mat4 modelmatrix = translate * rotate * scale;
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
*/
