#include <stdint.h>
#include <iostream>

typedef struct Voxel
{
    float tsdf;
    float x;
    float y;
    float z;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Voxel;


typedef struct int3
{
    int x, y, z;
} int3;


typedef struct VertsRGB
{
    float x, y, z;
    float r, g, b;
} VertsRGB;


typedef struct ButtonStatus
{
    bool pause = false;
    bool next_frame = false;
    bool prev_frame = false;
    bool background = false;
} ButtonStatus;