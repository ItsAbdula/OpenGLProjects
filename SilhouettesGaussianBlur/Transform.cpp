#include "Transform.h"

Transform::Transform()
{
    translate = glm::vec3(0.0f, 0.0f, 0.0f);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);

    updateDirectionalVector();
}

glm::vec3 Transform::getTranslate()
{
    return translate;
}
glm::vec3 Transform::getRotate()
{
    return rotate;
}
glm::vec3 Transform::getScale()
{
    return scale;
}
void Transform::setTransform(glm::vec3 _translate)
{
    translate = _translate;

    updateDirectionalVector();
}
void Transform::setRotate(glm::vec3 _rotate)
{
    rotate = _rotate;

    updateDirectionalVector();
}
void Transform::setScale(glm::vec3 _scale)
{
    scale = _scale;

    updateDirectionalVector();
}

glm::mat4 Transform::getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, translate);

    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

void Transform::updateDirectionalVector()
{
    glm::vec3 _front;
    _front.x = cos(glm::radians(rotate.y)) * cos(glm::radians(rotate.x));
    _front.y = sin(glm::radians(rotate.x));
    _front.z = sin(glm::radians(rotate.y)) * cos(glm::radians(rotate.x));

    front = glm::normalize(_front);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Transform::getFront()
{
    return front;
}

glm::vec3 Transform::getUp()
{
    return up;
}

glm::vec3 Transform::getRight()
{
    return right;
}

void Transform::move(glm::vec3 _delta)
{
    translate += _delta;

    updateDirectionalVector();
}

void Transform::move(glm::vec3 _direction, glm::vec1 _velocity)
{
    translate += glm::vec3(_velocity *_direction.x, _velocity *_direction.y, _velocity *_direction.z);

    updateDirectionalVector();
}