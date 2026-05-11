#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translation matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Ex. 1

        //glm::mat4 translation_matrix = glm::mat4
        //(
        //    1, 0, 0, translateX,
        //    0, 1, 0, translateY,
        //    0, 0, 1, translateZ,
        //    0, 0, 0, 1
        //);

        glm::mat4 translation_matrix = glm::mat4
        (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translateX, translateY, translateZ, 1
        );

        return translation_matrix;
    }

    // Scaling matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Ex. 1


        glm::mat4 scale_matrix = glm::mat4
        (
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, scaleZ, 0,
            0, 0, 0, 1
        );


        return scale_matrix;
    }

    // Rotation matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Ex. 1
        // Reprezinta rotatia paralela cu planul XOY
        // practic rotatie fata de axa OZ

        //glm::mat4 rotateOZ_matrix = glm::mat4
        //(
        //    cos(radians), -1*sin(radians), 0, 0,
        //    sin(radians), cos(radians), 0, 0,
        //    0, 0, 1, 0,
        //    0, 0, 0, 1
        //);

        glm::mat4 rotateOZ_matrix = glm::mat4
        (
            cos(radians), sin(radians), 0, 0,
            -1*sin(radians), cos(radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        return rotateOZ_matrix;
    }

    // Rotation matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Ex. 1

        //glm::mat4 rotateOY_matrix = glm::mat4
        //(
        //    cos(radians), 0, sin(radians), 0,
        //    0, 1, 0, 0,
        //    -1*sin(radians), 0, cos(radians), 0,
        //    0, 0, 0, 1
        //);

        glm::mat4 rotateOY_matrix = glm::mat4
        (
            cos(radians), 0, -1*sin(radians), 0,
            0, 1, 0, 0,
            sin(radians), 0, cos(radians), 0,
            0, 0, 0, 1
        );

        return rotateOY_matrix;
    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Ex. 1

        //glm::mat4 rotateOX_matrix = glm::mat4
        //(
        //    1, 0, 0, 0,
        //    0, cos(radians), -1*sin(radians), 0,
        //    0, sin(radians), cos(radians), 0,
        //    0, 0, 0, 1
        //);

        glm::mat4 rotateOX_matrix = glm::mat4
        (
            1, 0, 0, 0,
            0, cos(radians), sin(radians), 0,
            0, -1*sin(radians), cos(radians), 0,
            0, 0, 0, 1
        );

        return rotateOX_matrix;
    }

    // Perspective projection matrix
    inline glm::mat4 Perspective(
        float fovy, float aspect, float zNear, float zFar)
    {
        // TODO(student): Ex. 2

        glm::mat4 perspective_matrix = glm::mat4
        (
            1/(tan(fovy/2)*aspect), 0, 0, 0,
            0, 1/(tan(fovy/2)), 0, 0,
            0, 0, (-1*(zFar+zNear))/(zFar - zNear), -1,
            0, 0, (-2 * (zFar * zNear)) / (zFar - zNear), 0
        );

        return perspective_matrix;
    }

    // View matrix
    inline glm::mat4 View(
        const glm::vec3& position,
        const glm::vec3& forward,
        const glm::vec3& right,
        const glm::vec3& up)
    {
        // TODO(student): Ex. 4


        //column-major! 

        glm::mat4 pos_matrix = glm::mat4
        (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -1 * position.x, -1 * position.y, -1 * position.z, 1
        );

        glm::mat4 fru_matrix = glm::mat4
        (
            right.x, up.x, forward.x, 0,
            right.y, up.y, forward.y, 0,
            right.z, up.z, forward.z, 0,
            0, 0, 0, 1
        );

        glm::mat4 view_matrix = fru_matrix * pos_matrix;

        return view_matrix;
    }

}   // namespace transform3D
