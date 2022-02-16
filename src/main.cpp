#include <iostream>
#include "PointCloud.h"
#include "Opengl_Render.h"

int main()
{
    Opengl_Render *module_opgl_render = new Opengl_Render();

    module_opgl_render->init();
    module_opgl_render->loop();
    module_opgl_render->destroy();

    delete module_opgl_render;
    return 0;
}