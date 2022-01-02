#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include "shader.h"
#include "PointCloud.h"


class Opengl_Render
{
private:
    unsigned int WIN_WIDTH;
    unsigned int WIN_HEIGHT;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

private:
    GLFWwindow* window;
    glm::vec3 cameraPos;
    glm::vec3 cameraTar;
    glm::vec3 cameraUp;

    float fov;
    float deltaTime;
    float lastFrame;

    Shader *shaderModel;

private:
    VertsRGB* verts;
    int3 *faces;
    int vert_num;
    int face_num;

public:
    void init(char *bgr_model_filename);
    void loop();

    void processInput();
    void glInit();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};