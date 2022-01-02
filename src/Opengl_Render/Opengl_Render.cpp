#include "Opengl_Render.h"
#include <Windows.h>


void Opengl_Render::init(char *bgr_model_filename)
{
    this->WIN_WIDTH = 1920;
    this->WIN_HEIGHT = 1080;

    this->cameraTar = glm::vec3(-0.100000, 0.100000, -0.200000);
    this->cameraPos = glm::vec3(-4.787254, -2.590959, 2.571197);
    this->cameraUp = glm::vec3(0.036480, -0.747074, -0.663739);
    this->fov =  45.0f;

    // timing
    this->deltaTime = 0.0f;	// time between current frame and last frame
    this->lastFrame = 0.0f;

    this->verts = new VertsRGB[3700000];
    this->faces = new int3[7300000];
    load_ply_model(bgr_model_filename, this->verts, this->faces, this->vert_num, this->face_num);
}


void Opengl_Render::loop()
{
    glInit();

    while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 4 * this->vert_num, verts, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 4 * this->face_num, faces, GL_DYNAMIC_DRAW);

        // configure vertex attribute: position and color
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        shaderModel->use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        shaderModel->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraTar, cameraUp);
        shaderModel->setMat4("view", view);
        shaderModel->setMat4("model", glm::mat4(1.0f));

        glBindVertexArray(VAO);
        glPointSize(2.5);
        glDrawArrays(GL_POINTS, 0, this->vert_num);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 3 * this->face_num, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(50);
    }

    delete [] this->verts;
    delete [] this->faces;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
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

    // create VAO and VBO, bind VAO first, then bind and set vertex buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
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

