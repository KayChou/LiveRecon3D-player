#pragma once
#include "typedef.h"


void load_bgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num);

void load_fgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num);

void load_fgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num, bool binary);