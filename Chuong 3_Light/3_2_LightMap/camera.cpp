#include "camera.h"

Camera::Camera(QVector3D position, QVector3D up , float yaw , float pitch )
    : Front(QVector3D(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(QVector3D(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM)
{
    Position = QVector3D(posX, posY, posZ);
    WorldUp = QVector3D(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

QMatrix4x4 Camera::Camera::GetViewMatrix()
{
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.lookAt(Position, Position + Front, Up);
    return  mat;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
const float PI=3.141589;
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    QVector3D front;

    front.setX( cos(DegToRadian(Yaw)) * cos(DegToRadian(Pitch)));
    front.setY(sin(DegToRadian(Pitch)));
    front.setZ( sin(DegToRadian(Yaw)) * cos(DegToRadian(Pitch)));
    front.normalize();
    Front = front;
    // also re-calculate the Right and Up vector
    Right =QVector3D::crossProduct(Front, WorldUp);
    Right.normalize();
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = QVector3D::crossProduct(Right, Front);
    Up.normalize();
}

float Camera::DegToRadian(float degree)
{
    return  degree*PI/180;
}
