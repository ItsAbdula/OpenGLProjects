#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    void updateDirectionalVector();

public:
    Transform();

    glm::vec3 getTranslate();
    glm::vec3 getRotate();
    glm::vec3 getScale();
    void setTransform(glm::vec3 _translate);
    void setRotate(glm::vec3 _rotate);
    void setScale(glm::vec3 _scale);

    glm::vec3 getFront();
    glm::vec3 getUp();
    glm::vec3 getRight();

    glm::mat4 getModelMatrix();

    void move(glm::vec3 _delta);
    void move(glm::vec3 _direction, glm::vec1 _velocity);
};

#endif