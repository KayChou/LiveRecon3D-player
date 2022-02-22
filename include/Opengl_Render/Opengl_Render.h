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
#include "opengl_mesh.h"


class Opengl_Render
{
private:
    unsigned int WIN_WIDTH;
    unsigned int WIN_HEIGHT;
    
public:
    opengl_mesh *bgr_model;
    opengl_mesh *fgr_model;

private:
    GLFWwindow* window;
    glm::vec3 cameraPos;
    glm::vec3 cameraTar;
    glm::vec3 cameraUp;

    float fov;
    float deltaTime;
    float lastFrame;

    Shader *shaderModel;
    Shader *shaderBG;

private:
    VertsRGBUV* verts_bgr;
    VertsRGB* verts_fgr;
    int3 *faces_bgr;
    int3* faces_fgr;
    int vert_num_bgr, vert_num_fgr;
    int face_num_bgr, face_num_fgr;

private:
    int frame_cnt;
    ButtonStatus btn_status;

public:
    void init();
    void loop();
    void destroy();

    void processInput();
    void glInit();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};