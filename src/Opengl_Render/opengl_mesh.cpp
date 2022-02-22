#include "opengl_mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


opengl_mesh::opengl_mesh(bool has_texture)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    this->has_texture = has_texture;

    if(has_texture) {
        int nrChannels;
        stbi_set_flip_vertically_on_load(true);
        this->texture_data = stbi_load("../model/modular-environment/modular.jpg", &width, &height, &nrChannels, 0);
    }
}


void opengl_mesh::set_data(float* verts, int* faces, int v_num, int f_num)
{
    if(this->has_texture == false) {
        this->v_num = v_num;
        this->f_num = f_num;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float) * v_num, verts, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(float) * f_num, faces, GL_DYNAMIC_DRAW);

        // configure vertex attribute: position and color
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else {
        this->v_num = v_num;
        this->f_num = f_num;
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * v_num, verts, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(float) * f_num, faces, GL_DYNAMIC_DRAW);

        // load and create a texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // configure vertex attribute: position and color
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
}


void opengl_mesh::draw()
{
    if(this->has_texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glBindVertexArray(VAO);
    // glPointSize(1.5);
    // glDrawArrays(GL_POINTS, 0, this->v_num);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 3 * this->f_num, GL_UNSIGNED_INT, 0);
}