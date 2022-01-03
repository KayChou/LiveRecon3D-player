#include "Opengl_Render.h"
#include <Windows.h>


void Opengl_Render::init()
{
    this->WIN_WIDTH = 1920;
    this->WIN_HEIGHT = 1080;
    this->frame_cnt = 0;

    this->cameraTar = glm::vec3(-0.100000, 0.100000, -0.200000);
    this->cameraPos = glm::vec3(-4.787254, -2.590959, 2.571197);
    this->cameraUp = glm::vec3(0.036480, -0.747074, -0.663739);
    this->fov =  45.0f;

    // timing
    this->deltaTime = 0.0f;	// time between current frame and last frame
    this->lastFrame = 0.0f;

    this->verts_bgr = new VertsRGB[3700000];
    this->verts_fgr = new VertsRGB[3700000];
    this->faces_bgr = new int3[7300000];
    this->faces_fgr = new int3[7300000];
    load_bgr_model((char*)"../model/bgr/copyroom_simple.ply", this->verts_bgr, 
                                                              this->faces_bgr, 
                                                              this->vert_num_bgr, 
                                                              this->face_num_bgr);
    load_fgr_model((char*)"../model/fgr/mesh_0.ply", this->verts_fgr, 
                                                     this->faces_fgr, 
                                                     this->vert_num_fgr, 
                                                     this->face_num_fgr);
}


void Opengl_Render::loop()
{
    glInit();

    char filename[100];

    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        sprintf(filename, "../model/fgr/mesh_%d.ply", frame_cnt);
        load_fgr_model(filename, this->verts_fgr, 
                                 this->faces_fgr, 
                                 this->vert_num_fgr, 
                                 this->face_num_fgr);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        bgr_model->set_data((float*)this->verts_bgr, (int*)this->faces_bgr, this->vert_num_bgr, this->face_num_bgr);
        fgr_model->set_data((float*)this->verts_fgr, (int*)this->faces_fgr, this->vert_num_fgr, this->face_num_fgr);

        shaderModel->use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        shaderModel->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraTar, cameraUp);
        shaderModel->setMat4("view", view);
        shaderModel->setMat4("model", glm::mat4(1.0f));

        bgr_model->draw();
        fgr_model->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        frame_cnt = (frame_cnt + 1) % 300;
        Sleep(50);
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}


void Opengl_Render::destroy()
{
    delete [] this->verts_bgr;
    delete [] this->verts_fgr;
    delete [] this->faces_bgr;
    delete [] this->faces_fgr;
    delete bgr_model;
    delete fgr_model;
    delete shaderModel;
    delete bgr_model;
}


void Opengl_Render::glInit()
{
    //init glfw and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw: create window
    this->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LiveRecon3D", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl; fflush(stdout);
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all opengl function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl; fflush(stdout);
    }

    shaderModel = new Shader("../include/Opengl_Render/vertexShader.vert", "../include/Opengl_Render/fragShader.frag");
    bgr_model = new opengl_mesh();
    fgr_model = new opengl_mesh();
}

// ================================================================================
// key board input
// ================================================================================
void Opengl_Render::processInput()
{
    float translationSpeed = 0.1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if(glm::length(cameraPos - cameraTar) > cameraSpeed){
            cameraPos += cameraSpeed * glm::normalize(cameraTar - cameraPos);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * glm::normalize(cameraTar - cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS | glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 temp = cameraPos + cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraTar-cameraPos));
        cameraPos = cameraTar + glm::length(cameraPos - cameraTar) * glm::normalize(temp - cameraTar);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS | glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 temp = cameraPos - cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraTar-cameraPos));
        cameraPos = cameraTar + glm::length(cameraPos - cameraTar) * glm::normalize(temp - cameraTar);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        glm::vec3 right = glm::cross(cameraUp, cameraTar - cameraPos);
        glm::vec3 temp = (cameraPos + cameraSpeed * cameraUp);
        cameraPos = cameraTar + glm::length(cameraTar - cameraPos) * glm::normalize(temp - cameraTar);
        cameraUp = glm::normalize(glm::cross(cameraTar - cameraPos, right));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        glm::vec3 right = glm::cross(cameraUp, cameraTar - cameraPos);
        glm::vec3 temp = (cameraPos - cameraSpeed * cameraUp);
        cameraPos = cameraTar + glm::length(cameraTar - cameraPos) * glm::normalize(temp - cameraTar);
        cameraUp = glm::normalize(glm::cross(cameraTar - cameraPos, right));
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){ // translate x +
        cameraPos += glm::vec3(translationSpeed, 0.0f, 0.0f);
        cameraTar += glm::vec3(translationSpeed, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){ // translate y +
        cameraPos += glm::vec3(0.0f, translationSpeed, 0.0f);
        cameraTar += glm::vec3(0.0f, translationSpeed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){ // translate z +
        cameraPos += glm::vec3(0.0f, 0.0f, translationSpeed);
        cameraTar += glm::vec3(0.0f, 0.0f, translationSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){ // translate x -
        cameraPos -= glm::vec3(translationSpeed, 0.0f, 0.0f);
        cameraTar -= glm::vec3(translationSpeed, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){ // translate y -
        cameraPos -= glm::vec3(0.0f, translationSpeed, 0.0f);
        cameraTar -= glm::vec3(0.0f, translationSpeed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){ // translate z -
        cameraPos -= glm::vec3(0.0f, 0.0f, translationSpeed);
        cameraTar -= glm::vec3(0.0f, 0.0f, translationSpeed);
    }
    // std::cout << "Target: " << glm::to_string(cameraTar) << std::endl;
    // std::cout << "Position: " << glm::to_string(cameraPos) << std::endl;
    // std::cout << "Up: " << glm::to_string(cameraUp) << std::endl;
}


// ================================================================================
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ================================================================================
void Opengl_Render::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

