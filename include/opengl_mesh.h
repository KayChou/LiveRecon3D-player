#pragma once
#include <glad/glad.h>
#include "config.h"

class opengl_mesh
{
public:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int texture;

private:
    int v_num;
    int f_num;
    bool has_texture;

    int width, height;

    uint8_t *texture_data;

public:
    opengl_mesh(bool has_texture);
    void set_data(float* verts, int* faces, int v_num, int f_num);
    void draw();
};