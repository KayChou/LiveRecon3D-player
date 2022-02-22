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
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %s", str, str, str);
    fscanf(fp, "%s %s %d\n", str, str, &f_num);
    fscanf(fp, "%s %s %s %s %s", str, str, str, str, str);
    fscanf(fp, "%s", str);
    fread(&(alpha), sizeof(uint8_t), 1, fp);
    printf("loading mesh with %d verts and %d faces\n", v_num, f_num);

    for(int i = 0; i < v_num; i++) {
        fscanf(fp, "%f %f %f %f %f\n", &verts[i].x, &verts[i].y, &verts[i].z, &verts[i].u, &verts[i].v);
        // printf("%f %f %f %f %f\n", verts[i].x, verts[i].y, verts[i].z, verts[i].u, verts[i].v);
    }

    for(int i = 0; i < f_num; i++) {
        fscanf(fp, "%d %d %d %d\n", 
                    &edges, 
                    &faces[i].x, &faces[i].y, &faces[i].z);
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


void save_fgr_model(char* filename, VertsRGB* verts, int3* faces, int &v_num, int &f_num)
{
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", v_num);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\nproperty uchar green\nproperty uchar blue\n");
    fprintf(fp, "element face %d\n", f_num);
    fprintf(fp, "property list uchar int vertex_indices\n");
    fprintf(fp, "end_header\n");

    for(int i = 0; i < v_num; i++) {
        float x = verts[i].x;
        float y = verts[i].y;
        float z = verts[i].z;
        int r = (int)(verts[i].r * 255);
        int g = (int)(verts[i].g * 255);
        int b = (int)(verts[i].b * 255);

        // fwrite(&x, sizeof(float), 1, fp);
        // fwrite(&y, sizeof(float), 1, fp);
        // fwrite(&z, sizeof(float), 1, fp);
        // fwrite(&volume[i].r, sizeof(uint8_t), 1, fp);
        // fwrite(&volume[i].g, sizeof(uint8_t), 1, fp);
        // fwrite(&volume[i].b, sizeof(uint8_t), 1, fp);
        fprintf(fp, "%f %f %f %d %d %d\n", x, y, z, r, g, b);
    }
    for(int i = 0; i < f_num; i++) {
        int x = faces[i].x;
        int y = faces[i].y;
        int z = faces[i].z;
        int tri_edge = 3;

        // fwrite(&tri_edge, sizeof(uint8_t), 1, fp);
        // fwrite(&x, sizeof(int), 1, fp);
        // fwrite(&y, sizeof(int), 1, fp);
        // fwrite(&z, sizeof(int), 1, fp);
        fprintf(fp, "3 %d %d %d\n", x, y, z);
    }

    fclose(fp);
}