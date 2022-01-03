#include <iostream>
#include "PointCloud.h"
#include "Opengl_Render.h"

int main()
{
    char filename[100];
    sprintf(filename, "../model/copyroom_simple.ply");

    Opengl_Render *module_opgl_render = new Opengl_Render();

    module_opgl_render->init(filename);
    module_opgl_render->loop();
    module_opgl_render->destroy();

    return 0;
}