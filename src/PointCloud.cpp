#include "PointCloud.h"

void load_bgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num)
{
    char str[100];
    uint8_t alpha;
    uint8_t edges;
    int r, g, b;
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%s", str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &v_num);
    fscanf(fp, "%s %s %s", str, str, str);
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
        fscanf(fp, "%d %d %d %d\n", &r, &g, &b, &alpha);
        
        verts[i].r = (float)(r / 255.f);
        verts[i].g = (float)(g / 255.f);
        verts[i].b = (float)(b / 255.f);

        // printf("%f %f %f %f %f %f\n", verts[i].x, verts[i].y, verts[i].z, verts[i].r, verts[i].g, verts[i].b);
    }

    for(int i = 0; i < f_num; i++) {
        fscanf(fp, "%d %d %d %d\n", &edges, &faces[i].x, &faces[i].y, &faces[i].z);
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