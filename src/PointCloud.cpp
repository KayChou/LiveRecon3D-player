#include "PointCloud.h"

void load_bgr_model(char* filename, VertsRGBUV* verts, int3* faces, int &v_num, int &f_num)
{
    char str[100];
    uint8_t alpha;
    int edges;
    int r, g, b;
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%s", str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &v_num);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &f_num);
    fscanf(fp, "%s %s %s %s %s", str, str, str, str, str);
    fscanf(fp, "%s %s %s %s %s", str, str, str, str, str);
    fscanf(fp, "%s", str);
    fread(&(alpha), sizeof(uint8_t), 1, fp);
    printf("loading mesh with %d verts and %d faces\n", v_num, f_num);

    for(int i = 0; i < v_num; i++) {
        fscanf(fp, "%f %f %f\n", &verts[i].x, &verts[i].y, &verts[i].z);
        // fscanf(fp, "%d %d %d %d\n", &r, &g, &b, &alpha);
        
        verts[i].r = 1;
        verts[i].g = 0;
        verts[i].b = 0;
        verts[i].u = 0;
        verts[i].v = 0;

        // printf("%f %f %f %f %f %f\n", verts[i].x, verts[i].y, verts[i].z, verts[i].r, verts[i].g, verts[i].b);
    }

    float uv_coord[3][2];
    for(int i = 0; i < f_num; i++) {
        fscanf(fp, "%d %d %d %d %d %f %f %f %f %f %f\n", 
                    &edges, 
                    &faces[i].x, &faces[i].y, &faces[i].z,
                    &edges,
                    &uv_coord[0][0],
                    &uv_coord[0][1],
                    &uv_coord[1][0],
                    &uv_coord[1][1],
                    &uv_coord[2][0],
                    &uv_coord[2][1]);
        verts[faces[i].x].u = uv_coord[0][0];
        verts[faces[i].x].v = uv_coord[0][1];
        verts[faces[i].y].u = uv_coord[1][0];
        verts[faces[i].y].v = uv_coord[1][1];
        verts[faces[i].z].u = uv_coord[2][0];
        verts[faces[i].z].v = uv_coord[2][1];
    }

    fclose(fp);
    printf("Loading finished\n");
}


void load_fgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num)
{
    char str[100];
    uint8_t alpha;
    uint8_t edges;
    int r, g, b;
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%s", str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &v_num);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &f_num);
    fscanf(fp, "%s %s %s %s %s", str, str, str, str, str);
    fscanf(fp, "%s", str);
    fread(&(alpha), sizeof(uint8_t), 1, fp);
    printf("loading mesh with %d verts and %d faces\n", v_num, f_num);

    for(int i = 0; i < v_num; i++) {
        fscanf(fp, "%f %f %f\n", &verts[i].x, &verts[i].y, &verts[i].z);
        fscanf(fp, "%d %d %d\n", &r, &g, &b);

        verts[i].z -= 2.9f;
        verts[i].r = (float)(std::min((r + 30), 255) / 255.f);
        verts[i].g = (float)(std::min((g + 30), 255) / 255.f);
        verts[i].b = (float)(std::min((b + 30), 255) / 255.f);
    }

    for(int i = 0; i < f_num; i++) {
        fscanf(fp, "%d %d %d %d\n", &edges, &faces[i].x, &faces[i].y, &faces[i].z);
    }

    fclose(fp);
    printf("Loading finished\n");
}


void load_fgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num, bool binary)
{
    char str[100];
    uint8_t alpha;
    uint8_t edges;
    int r, g, b;
    FILE *fp = fopen(filename, "rb");
    
    fread(&v_num, sizeof(int), 1, fp);
    fread(&f_num, sizeof(int), 1, fp);
    // printf("loading mesh with %d verts and %d faces\n", v_num, f_num);

    fread(verts, sizeof(float), 6 * v_num, fp);
    fread(faces, sizeof(int), 3 * f_num, fp);

    fclose(fp);
}