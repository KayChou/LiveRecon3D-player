#include <iostream>
#include "PointCloud.h"

int main()
{
    char filename[100];
    sprintf(filename, "../model/copyroom.ply");

    VertsRGB* verts = new VertsRGB[3700000];
    int3* faces = new int3[7300000];
    int vert_num;
    int face_num;
    load_ply_model(filename, verts, faces, vert_num, face_num);

    return 0;
}