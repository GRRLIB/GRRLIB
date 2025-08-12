/*------------------------------------------------------------------------------
Copyright (c) 2025 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

#include <grrlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static GRRLIB_EmbeddedFile* EmbeddedFile = NULL;

/**
 * Shortcut to access triangles values.
 */
#define T(x) model->triangles[(x)]

/**
 * Elements of a vertex.
 */
enum { X, Y, Z };

/**
 * General purpose node.
 */
typedef struct _GRRLIB_Node {
    u32    index;
    bool   averaged;
    struct _GRRLIB_Node* next;
} GRRLIB_Node;

/**
 * Find an embedded file by name.
 * @filename The name of the file to find.
 * @return A pointer to the embedded file, or NULL if not found.
 */
static const GRRLIB_EmbeddedFile* FindEmbedded(const char* filename) {
    if(filename == NULL) {
        return NULL;
    }
    for (const GRRLIB_EmbeddedFile* f = EmbeddedFile; f->name; ++f) {
        if (strcmp(f->name, filename) == 0) {
            return f;
        }
    }
    return NULL;
}

/**
 * Load a texture from a file or embedded resource.
 * @param filename The JPEG, PNG or Bitmap filename to load.
 * @return A GRRLIB_texImg structure filled with image information.
 *         If an error occurs NULL will be returned.
 */
static GRRLIB_texImg* GRRLIB_LoadTextureFromX(const char *dir, const char *filename) {
    if (dir == NULL) {
        const GRRLIB_EmbeddedFile* embedded_file = FindEmbedded(filename);
        return GRRLIB_LoadTexture(embedded_file->data);
    }
    else {
        char* filepath = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(filename) + 1));
        strcpy(filepath, dir);
        strcat(filepath, filename);
        GRRLIB_texImg* result = GRRLIB_LoadTextureFromFile(filepath);
        free(filepath);
        return result;
    }
}

/**
 * Find a group in the model.
 * @param model Structure that defines the model in wich the group will be searched.
 * @param name Yhe name of the group to find.
 * @return The group found in the model.
 */
static GRRLIB_Group* GRRLIB_FindGroup(GRRLIB_Model* model, char* name) {
    GRRLIB_Group* group = model->groups;
    while(group) {
        if (!strcmp(name, group->name)) {
            break;
        }
        group = group->next;
    }
    return group;
}

/**
 * Add a group to the model.
 * @param model Structure that defines the model to wich the group will be added.
 * @param name Yhe name of the group to add.
 * @return The group added to the model.
 */
static GRRLIB_Group* GRRLIB_AddGroup(GRRLIB_Model* model, char* name) {
    GRRLIB_Group* group = GRRLIB_FindGroup(model, name);
    if (!group) {
        group = (GRRLIB_Group*)malloc(sizeof(GRRLIB_Group));
        group->name = strdup(name);
        group->material = 0;
        group->numtriangles = 0;
        group->triangles = NULL;
        group->next = model->groups;
        model->groups = group;
        model->numgroups++;
    }
    return group;
}

/**
 * Find a material in the model.
 * @param model Structure that defines the model in wich to find the material.
 * @param name The name of the material.
 * @return The position of the material, zero if not found.
 */
u32 GRRLIB_FindMaterial(GRRLIB_Model* model, char* name) {
    if (model == NULL || name == NULL) {
        return 0;
    }

    for (u32 i = 0; i < model->nummaterials; i++) {
        if (strcmp(model->materials[i].name, name) == 0) {
            return i;
        }
    }

    return 0;
}

/**
 * Return the directory given a path.
 * @param path Filesystem path.
 * @return The directory given a path. The return value should be free'd.
 */
static char* GRRLIB_DirName(char* path) {
    char* dir = strdup(path);
    char* s = strrchr(dir, '/');
    if (s != NULL) {
        s[1] = '\0';
    }
    else {
        dir[0] = '\0';
    }
    return dir;
}

/**
 * Read a wavefront material library file.
 * @param model Properly initialized GRRLIB_Model structure.
 * @param name The name of the material library.
 */
static void GRRLIB_ReadMTL(GRRLIB_Model* model, char* name) {
    char buf[128];
    f32 Red, Blue, Green;

    char* dir = NULL;

    // open the file
    FILE* file = NULL;
    const GRRLIB_EmbeddedFile* embedded_file = FindEmbedded(name);
    if (embedded_file) {
        file = fmemopen((void*)embedded_file->data, embedded_file->len, "r");
    }
    else {
        dir = GRRLIB_DirName(model->pathname);
        char* filename = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(name) + 1));
        strcpy(filename, dir);
        strcat(filename, name);
        file = fopen(filename, "r");
        free(filename);
    }

    if (file == NULL) {
        return;
    }

    // count the number of materials in the file
    u32 nummaterials = 1;
    while (fscanf(file, "%s", buf) != EOF) {
        switch (buf[0]) {
            case '#': // comment
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
            case 'n': // newmtl
                fgets(buf, sizeof(buf), file);
                nummaterials++;
                sscanf(buf, "%s %s", buf, buf);
                break;
            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
        }
    }

    rewind(file);

    // allocate memory for the materials
    model->materials = (GRRLIB_Material*)malloc(sizeof(GRRLIB_Material) * nummaterials);
    model->nummaterials = nummaterials;

    // set the default material
    for (u32 i = 0; i < nummaterials; i++) {
        model->materials[i].name = NULL;
        model->materials[i].shininess = 0;
        model->materials[i].diffuse[0] = 204;
        model->materials[i].diffuse[1] = 204;
        model->materials[i].diffuse[2] = 204;
        model->materials[i].ambient[0] = 51;
        model->materials[i].ambient[1] = 51;
        model->materials[i].ambient[2] = 51;
        model->materials[i].specular[0] = 0;
        model->materials[i].specular[1] = 0;
        model->materials[i].specular[2] = 0;
        model->materials[i].alpha = 255;
        model->materials[i].diffusetex = NULL;
        model->materials[i].ambienttex = NULL;
        model->materials[i].speculartex = NULL;
    }
    model->materials[0].name = strdup("default");

    // now, read in the data
    nummaterials = 0;

    while (fscanf(file, "%s", buf) != EOF) {
        switch (buf[0]) {
        case '#': // comment
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        case 'n': // newmtl
            fgets(buf, sizeof(buf), file);
            sscanf(buf, "%s %s", buf, buf);
            nummaterials++;
            model->materials[nummaterials].name = strdup(buf);
            break;
        case 'N':
            fscanf(file, "%f", &model->materials[nummaterials].shininess);
            // wavefront shininess is from [0, 1000], so scale for OpenGL
            model->materials[nummaterials].shininess /= 1000.0;
            model->materials[nummaterials].shininess *= 128.0;
            break;
        case 'm': // texture map
            switch (buf[5]) {
            case 'd': // the diffuse texture map
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf); // Get file name
                if(buf[0] != ' ') {
                    model->materials[nummaterials].diffusetex = GRRLIB_LoadTextureFromX(dir, buf);
                }
                break;
            case 's': // the specular texture map
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf); // Get file name
                if(buf[0] != ' ') {
                    model->materials[nummaterials].speculartex = GRRLIB_LoadTextureFromX(dir, buf);
                }
                break;
            case 'a': // the ambient texture map
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf); // Get file name
                if(buf[0] != ' ') {
                    model->materials[nummaterials].ambienttex = GRRLIB_LoadTextureFromX(dir, buf);
                }
                break;
            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
            }
            break;
        case 'K':
            switch (buf[1]) {
            case 'd': // diffuse color of the material
                fscanf(file, "%f %f %f", &Red, &Green, &Blue);
                model->materials[nummaterials].diffuse[0] = (u8)(255.0f * Red + 0.5f);
                model->materials[nummaterials].diffuse[1] = (u8)(255.0f * Green + 0.5f);
                model->materials[nummaterials].diffuse[2] = (u8)(255.0f * Blue + 0.5f);
                break;
            case 's': // specular color of the material
                fscanf(file, "%f %f %f", &Red, &Green, &Blue);
                model->materials[nummaterials].specular[0] = (u8)(255.0f * Red + 0.5f);
                model->materials[nummaterials].specular[1] = (u8)(255.0f * Green + 0.5f);
                model->materials[nummaterials].specular[2] = (u8)(255.0f * Blue + 0.5f);
                break;
            case 'a': // ambient color of the material
                fscanf(file, "%f %f %f", &Red, &Green, &Blue);
                model->materials[nummaterials].ambient[0] = (u8)(255.0f * Red + 0.5f);
                model->materials[nummaterials].ambient[1] = (u8)(255.0f * Green + 0.5f);
                model->materials[nummaterials].ambient[2] = (u8)(255.0f * Blue + 0.5f);
                break;
            default:
                // eat up rest of line
                fgets(buf, sizeof(buf), file);
                break;
            }
            break;
        default:
            // eat up rest of line
            fgets(buf, sizeof(buf), file);
            break;
        }
    }
    free(dir);
}

/**
 * Compute the dot product of two vectors.
 * @param u An array of 3 f32 (f32 u[3]).
 * @param v An array of 3 f32 (f32 v[3]).
 */
static f32 GRRLIB_Dot(f32* u, f32* v) {
    if(u == NULL || v == NULL) {
        return 0.0;
    }
    return u[X] * v[X] + u[Y] * v[Y] + u[Z] * v[Z];
}

/**
 * Compute the cross product of two vectors.
 * @param u An array of 3 f32 (f32 u[3]).
 * @param v An array of 3 f32 (f32 v[3]).
 * @param n An array of 3 f32 (f32 n[3]) to return the cross product in.
 */
static void GRRLIB_Cross(f32* u, f32* v, f32* n) {
    if(u == NULL || v == NULL || n == NULL) {
        return;
    }

    // compute the cross product (u x v for right-handed [ccw])
    n[X] = u[Y] * v[Z] - u[Z] * v[Y];
    n[Y] = u[Z] * v[X] - u[X] * v[Z];
    n[Z] = u[X] * v[Y] - u[Y] * v[X];
}

/**
 * Normalize a vector.
 * @param n An array of 3 f32 (f32 n[3]) to be normalized.
 */
static void GRRLIB_Normalize(f32* n) {
    if(n == NULL) {
        return;
    }

    f32 l = (f32)sqrt(n[X] * n[X] + n[Y] * n[Y] + n[Z] * n[Z]);
    n[0] /= l;
    n[1] /= l;
    n[2] /= l;
}

/**
 * Calculates the dimensions (width, height, depth) of a model.
 *
 * @param model Initialized GRRLIB_Model structure.
 * @param dimensions Array of 3 f32 (f32 dimensions[3])
 */
static void GRRLIB_Dimensions(GRRLIB_Model* model, f32* dimensions) {
    f32 maxx, minx, maxy, miny, maxz, minz;

    if(model == NULL || model->vertices == NULL || dimensions == NULL) {
        dimensions[X] = 0;
        dimensions[Y] = 0;
        dimensions[Z] = 0;
        return;
    }

    // get the max/mins
    maxx = minx = model->vertices[3 + X];
    maxy = miny = model->vertices[3 + Y];
    maxz = minz = model->vertices[3 + Z];
    for (u32 i = 1; i <= model->numvertices; i++) {
        if (maxx < model->vertices[3 * i + X]) {
            maxx = model->vertices[3 * i + X];
        }
        if (minx > model->vertices[3 * i + X]) {
            minx = model->vertices[3 * i + X];
        }

        if (maxy < model->vertices[3 * i + Y]) {
            maxy = model->vertices[3 * i + Y];
        }
        if (miny > model->vertices[3 * i + Y]) {
            miny = model->vertices[3 * i + Y];
        }

        if (maxz < model->vertices[3 * i + Z]) {
            maxz = model->vertices[3 * i + Z];
        }
        if (minz > model->vertices[3 * i + Z]) {
            minz = model->vertices[3 * i + Z];
        }
    }

    // calculate model width, height, and depth
    dimensions[X] = fabsf(maxx) + fabsf(minx);
    dimensions[Y] = fabsf(maxy) + fabsf(miny);
    dimensions[Z] = fabsf(maxz) + fabsf(minz);
}

/**
 * Second pass at a Wavefront OBJ file that gets all the data.
 *
 * @param model Properly initialized GRRLIB_Model structure.
 * @param file File descriptor.
 */
static void GRRLIB_SecondPass(GRRLIB_Model* model, FILE* file) {
    u32    v, n, t;
    char   buf[128];

    // set the pointer shortcuts
    f32* vertices       = model->vertices;  /* array of vertices */
    f32* normals        = model->normals;   /* array of normals */
    f32* texcoords      = model->texcoords; /* array of texture coordinates */
    GRRLIB_Group* group = model->groups;    /* current group pointer */

    /* on the second pass through the file, read all the data into the
     allocated arrays */
    u32 numvertices = 1;     /* number of vertices in model */
    u32 numnormals = 1;      /* number of normals in model */
    u32 numtexcoords = 1;    /* number of texcoords in model */
    u32 numtriangles = 0;    /* number of triangles in model */
    u32 material = 0;        /* current material */
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
            case '#':               /* comment */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
            break;
            case 'v':               /* v, vn, vt */
                switch(buf[1]) {
                    case '\0':          /* vertex */
                        fscanf(file, "%f %f %f",
                           &vertices[3 * numvertices + X],
                           &vertices[3 * numvertices + Y],
                           &vertices[3 * numvertices + Z]);
                        numvertices++;
                    break;
                    case 'n':               /* normal */
                        fscanf(file, "%f %f %f",
                           &normals[3 * numnormals + X],
                           &normals[3 * numnormals + Y],
                           &normals[3 * numnormals + Z]);
                        numnormals++;
                    break;
                    case 't':               /* texcoord */
                        fscanf(file, "%f %f",
                           &texcoords[2 * numtexcoords + X],
                           &texcoords[2 * numtexcoords + Y]);
                        numtexcoords++;
                    break;
                }
            break;
            case 'u':               /* usemtl */
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                group->material = material = GRRLIB_FindMaterial(model, buf);
            break;
            case 'g':               /* group */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s", buf);
                group = GRRLIB_AddGroup(model, buf);
                group->material = material;
            break;
            case 'f':               /* face */
                v = n = t = 0;
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
                if (strstr(buf, "//")) {
                    // v//n
                    sscanf(buf, "%d//%d", &v, &n);
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    // v/t/n
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    // v/t
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else {
                    // v
                    sscanf(buf, "%d", &v);
                    T(numtriangles).vindices[0] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[1] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[2] = v;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).vindices[2] = v;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                }
            break;
            default:
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
            break;
        }
    }
}

/**
 * First pass at a Wavefront OBJ file that gets all the
 * statistics of the model (such as #vertices, #normals, etc).
 *
 * @param model Properly initialized GRRLIB_Model structure.
 * @param file File descriptor.
 */
static void GRRLIB_FirstPass(GRRLIB_Model* model, FILE* file) {
    u32    numvertices;     /* number of vertices in model */
    u32    numnormals;      /* number of normals in model */
    u32    numtexcoords;        /* number of texcoords in model */
    u32    numtriangles;        /* number of triangles in model */
    GRRLIB_Group* group;            /* current group */
    unsigned  v, n, t;
    char      buf[128];

    // make a default group
    group = GRRLIB_AddGroup(model, "default");

    numvertices = numnormals = numtexcoords = numtriangles = 0;

    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
            case '#':               /* comment */
                fgets(buf, sizeof(buf), file);
                break;
            case 'v':               /* v, vn, vt */
                switch(buf[1]) {
                    case '\0':          /* vertex */
                        fgets(buf, sizeof(buf), file);
                        numvertices++;
                        break;
                    case 'n':               /* normal */
                        fgets(buf, sizeof(buf), file);
                        numnormals++;
                        break;
                    case 't':               /* texcoord */
                        fgets(buf, sizeof(buf), file);
                        numtexcoords++;
                        break;
                    default:
                        exit(1);
                }
                break;
            case 'm':
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                model->mtllibname = strdup(buf);
                GRRLIB_ReadMTL(model, buf);
                break;
            case 'u':               /* usemtl */
                fgets(buf, sizeof(buf), file);
                break;
            case 'g':               /* group */
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s", buf);
                group = GRRLIB_AddGroup(model, buf);
                break;
            case 'f':               /* face */
                v = n = t = 0;
                fscanf(file, "%s", buf);
                /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    /* v/t/n */
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    /* v/t */
                    fscanf(file, "%d/%d", &v, &t);
                    fscanf(file, "%d/%d", &v, &t);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else {
                    /* v */
                    fscanf(file, "%d", &v);
                    fscanf(file, "%d", &v);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                break;
            default:
                fgets(buf, sizeof(buf), file);
                break;
        }
    }

    model->numvertices  = numvertices;
    model->numnormals   = numnormals;
    model->numtexcoords = numtexcoords;
    model->numtriangles = numtriangles;

    // allocate memory for the triangles in each group
    group = model->groups;
    while(group) {
        group->triangles = (u32*)malloc(sizeof(u32) * group->numtriangles);
        group->numtriangles = 0;
        group = group->next;
    }
}

/**
 * Reads a model description from a Wavefront .OBJ file.
 * @param filename Name of the file containing the Wavefront .OBJ format data.
 * @return Returns a pointer to the created object which should be free'd with GRRLIB_DeleteObj.
 */
GRRLIB_Model* GRRLIB_ReadOBJ(char* filename) {
    FILE* file = NULL;
    const GRRLIB_EmbeddedFile* embedded_file = FindEmbedded(filename);
    if (embedded_file) {
        file = fmemopen((void*)embedded_file->data, embedded_file->len, "r");
    }
    else {
        file = fopen(filename, "r");
    }

    if (file == NULL) {
        return NULL;
    }

    GRRLIB_Model* model = (GRRLIB_Model*)malloc(sizeof(GRRLIB_Model));
    model->pathname      = strdup(filename);
    model->mtllibname    = NULL;
    model->numvertices   = 0;
    model->vertices      = NULL;
    model->numnormals    = 0;
    model->normals       = NULL;
    model->numtexcoords  = 0;
    model->texcoords     = NULL;
    model->numfacetnorms = 0;
    model->facetnorms    = NULL;
    model->numtriangles  = 0;
    model->triangles     = NULL;
    model->nummaterials  = 0;
    model->materials     = NULL;
    model->numgroups     = 0;
    model->groups        = NULL;
    model->position.x    = 0.0;
    model->position.y    = 0.0;
    model->position.z    = 0.0;

    // Get a count of the number of stuff
    GRRLIB_FirstPass(model, file);

    // allocate memory
    model->vertices = (f32*)malloc(sizeof(f32) * 3 * (model->numvertices + 1));
    model->triangles = (GRRLIB_Triangle*)malloc(sizeof(GRRLIB_Triangle) * model->numtriangles);
    if (model->numnormals) {
        model->normals = (f32*)malloc(sizeof(f32) * 3 * (model->numnormals + 1));
    }
    if (model->numtexcoords) {
        model->texcoords = (f32*)malloc(sizeof(f32) * 2 * (model->numtexcoords + 1));
    }

    rewind(file);

    GRRLIB_SecondPass(model, file);

    fclose(file);

    return model;
}

/**
 * Deletes a GRRLIB_Material structure.
 * @param material Initialized GRRLIB_Material structure.
 */
static void GRRLIB_DeleteMaterial(GRRLIB_Material* material) {
    free(material->name);
    GRRLIB_FreeTexture(material->diffusetex);
    GRRLIB_FreeTexture(material->ambienttex);
    GRRLIB_FreeTexture(material->speculartex);
}

/**
 * Deletes a GRRLIB_Model structure.
 * @param model Initialized GRRLIB_Model structure.
 */
void GRRLIB_DeleteObj(GRRLIB_Model* model) {
    if (model == NULL) {
        return;
    }

    if (model->pathname)
        free(model->pathname);
    if (model->mtllibname)
        free(model->mtllibname);
    if (model->vertices)
        free(model->vertices);
    if (model->normals)
        free(model->normals);
    if (model->texcoords)
        free(model->texcoords);
    if (model->facetnorms)
        free(model->facetnorms);
    if (model->triangles)
        free(model->triangles);
    if (model->materials) {
        for (u32 i = 0; i < model->nummaterials; i++) {
            GRRLIB_DeleteMaterial(&model->materials[i]);
        }
    }
    free(model->materials);
    while(model->groups) {
        GRRLIB_Group* group = model->groups;
        model->groups = model->groups->next;
        free(group->name);
        free(group->triangles);
        free(group);
    }

    free(model);
}

/**
 * Draw a 3D object.
 * @param model Structure that defines the model to draw.
 */
void GRRLIB_Draw3dObj(GRRLIB_Model* model) {
    if(model == NULL) {
        return;
    }

    GRRLIB_Group* group = model->groups;
    while (group) {
        const u32 Color = RGBA(model->materials[group->material].diffuse[0],
            model->materials[group->material].diffuse[1],
            model->materials[group->material].diffuse[2],
            0xFF);

        GRRLIB_texImg* tex = model->materials[group->material].diffusetex;

        GX_ClearVtxDesc();

        GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
        if(model->numnormals) {
            GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
        }
        GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        if(model->numtexcoords && tex) {
            GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        }

        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        if(model->numnormals) {
            GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
        }
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        if(model->numtexcoords && tex) {
            GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        }

        if(model->numtexcoords && tex) {
            GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
            GRRLIB_SetTexture(tex, 0);
        }
        else {
            GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        }

        GX_Begin(GX_TRIANGLES, GX_VTXFMT0, group->numtriangles*3);
        for (int i = 0; i < group->numtriangles; i++) {
            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[0] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[0] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[0] + Z]);
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[0] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[0] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[0] + Z]);
            }
            GX_Color1u32(Color);
            if(model->numtexcoords && tex) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[0] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[0] + Y]);
            }


            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[1] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[1] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[1] + Z]);
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[1] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[1] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[1] + Z]);
            }
            GX_Color1u32(Color);
            if(model->numtexcoords && tex) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[1] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[1] + Y]);
            }


            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[2] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[2] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[2] + Z]);
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[2] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[2] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[2] + Z]);
            }
            GX_Color1u32(Color);
            if(model->numtexcoords && tex) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[2] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[2] + Y]);
            }
        }
        GX_End();
        group = group->next;
    }
}

/**
 * Generates smooth vertex normals for a model.
 * First builds a list of all the triangles each vertex is in.  Then
 * loops through each vertex in the the list averaging all the facet
 * normals of the triangles each vertex is in.  Finally, sets the
 * normal index in the triangle for the vertex to the generated smooth
 * normal.  If the dot product of a facet normal and the facet normal
 * associated with the first triangle in the list of triangles the
 * current vertex is in is greater than the cosine of the angle
 * parameter to the function, that facet normal is not added into the
 * average normal calculation and the corresponding vertex is given
 * the facet normal.  This tends to preserve hard edges.  The angle to
 * use depends on the model, but 90 degrees is usually a good start.
 *
 * @param model Initialized GRRLIB_Model structure.
 * @param angle Maximum angle (in degrees) to smooth across.
 */
void GRRLIB_VertexNormals(GRRLIB_Model* model, f32 angle) {
    GRRLIB_Node*  node;

    if(model == NULL || model->facetnorms == NULL) {
        return;
    }

    // calculate the cosine of the angle (in degrees)
    const f32 cos_angle = cos(angle * M_PI / 180.0);

    // nuke any previous normals
    if (model->normals) {
        free(model->normals);
    }

    // allocate space for new normals
    model->numnormals = model->numtriangles * 3; /* 3 normals per triangle */
    model->normals = (f32*)malloc(sizeof(f32)* 3* (model->numnormals+1));

    // allocate a structure that will hold a linked list of triangle indices for each vertex
    GRRLIB_Node** members = (GRRLIB_Node**)malloc(sizeof(GRRLIB_Node*) * (model->numvertices + 1));
    for (u32 i = 1; i <= model->numvertices; i++) {
        members[i] = NULL;
    }

    // for every triangle, create a node for each vertex in it
    for (u32 i = 0; i < model->numtriangles; i++) {
        node = (GRRLIB_Node*)malloc(sizeof(GRRLIB_Node));
        node->index = i;
        node->next  = members[T(i).vindices[0]];
        members[T(i).vindices[0]] = node;

        node = (GRRLIB_Node*)malloc(sizeof(GRRLIB_Node));
        node->index = i;
        node->next  = members[T(i).vindices[1]];
        members[T(i).vindices[1]] = node;

        node = (GRRLIB_Node*)malloc(sizeof(GRRLIB_Node));
        node->index = i;
        node->next  = members[T(i).vindices[2]];
        members[T(i).vindices[2]] = node;
    }

    // calculate the average normal for each vertex
    u32 numnormals = 1;
    for (u32 i = 1; i <= model->numvertices; i++) {
        // calculate an average normal for this vertex by averaging the
        // facet normal of every triangle this vertex is in
        node = members[i];

        f32 average[3] = {0.0f, 0.0f, 0.0f};
        u32 avg = 0;
        while (node) {
            /* only average if the dot product of the angle between the two
            facet normals is greater than the cosine of the threshold
            angle -- or, said another way, the angle between the two
            facet normals is less than (or equal to) the threshold angle */
            const f32 dot = GRRLIB_Dot(&model->facetnorms[3 * T(node->index).findex],
                &model->facetnorms[3 * T(members[i]->index).findex]);
            if (dot > cos_angle) {
                node->averaged = true;
                average[0] += model->facetnorms[3 * T(node->index).findex + 0];
                average[1] += model->facetnorms[3 * T(node->index).findex + 1];
                average[2] += model->facetnorms[3 * T(node->index).findex + 2];
                avg = 1;            // we averaged at least one normal!
            }
            else {
                node->averaged = false;
            }
            node = node->next;
        }

        if (avg) {
            // normalize the averaged normal
            GRRLIB_Normalize(average);

            // add the normal to the vertex normals list
            model->normals[3 * numnormals + 0] = average[0];
            model->normals[3 * numnormals + 1] = average[1];
            model->normals[3 * numnormals + 2] = average[2];
            avg = numnormals;
            numnormals++;
        }

        // set the normal of this vertex in each triangle it is in
        node = members[i];
        while (node) {
            if (node->averaged) {
                // if this node was averaged, use the average normal
                if (T(node->index).vindices[0] == i) {
                    T(node->index).nindices[0] = avg;
                }
                else if (T(node->index).vindices[1] == i) {
                    T(node->index).nindices[1] = avg;
                }
                else if (T(node->index).vindices[2] == i) {
                    T(node->index).nindices[2] = avg;
                }
            }
            else {
                // if this node wasn't averaged, use the facet normal
                model->normals[3 * numnormals + 0] =
                model->facetnorms[3 * T(node->index).findex + 0];
                model->normals[3 * numnormals + 1] =
                model->facetnorms[3 * T(node->index).findex + 1];
                model->normals[3 * numnormals + 2] =
                model->facetnorms[3 * T(node->index).findex + 2];
                if (T(node->index).vindices[0] == i) {
                    T(node->index).nindices[0] = numnormals;
                }
                else if (T(node->index).vindices[1] == i) {
                    T(node->index).nindices[1] = numnormals;
                }
                else if (T(node->index).vindices[2] == i) {
                    T(node->index).nindices[2] = numnormals;
                }
                numnormals++;
            }
            node = node->next;
        }
    }

    model->numnormals = numnormals - 1;

    // free the member information
    for (u32 i = 1; i <= model->numvertices; i++) {
        node = members[i];
        while (node) {
            GRRLIB_Node* tail = node;
            node = node->next;
            free(tail);
        }
    }
    free(members);

    /* pack the normals array (we previously allocated the maximum
    number of normals that could possibly be created (numtriangles *
    3), so get rid of some of them (usually alot unless none of the
    facet normals were averaged)) */
    f32* normals = model->normals;
    model->normals = (f32*)malloc(sizeof(f32)* 3* (model->numnormals+1));
    for (u32 i = 1; i <= model->numnormals; i++) {
        model->normals[3 * i + 0] = normals[3 * i + 0];
        model->normals[3 * i + 1] = normals[3 * i + 1];
        model->normals[3 * i + 2] = normals[3 * i + 2];
    }
    free(normals);
}

/**
 * Generates facet normals for a model (by taking the
 * cross product of the two vectors derived from the sides of each
 * triangle).  Assumes a counter-clockwise winding.
 *
 * @param model Initialized GRRLIB_Model structure.
 */
void GRRLIB_FacetNormals(GRRLIB_Model* model) {
    f32 u[3];
    f32 v[3];

    if(model == NULL || model->vertices == NULL) {
        return;
    }

    // clobber any old facetnormals
    if (model->facetnorms) {
        free(model->facetnorms);
    }

    // allocate memory for the new facet normals
    model->numfacetnorms = model->numtriangles;
    model->facetnorms = (f32*)malloc(sizeof(f32) * 3 * (model->numfacetnorms + 1));

    for (u32 i = 0; i < model->numtriangles; i++) {
        model->triangles[i].findex = i + 1;

        u[X] = model->vertices[3 * T(i).vindices[1] + X] -
            model->vertices[3 * T(i).vindices[0] + X];
        u[Y] = model->vertices[3 * T(i).vindices[1] + Y] -
            model->vertices[3 * T(i).vindices[0] + Y];
        u[Z] = model->vertices[3 * T(i).vindices[1] + Z] -
            model->vertices[3 * T(i).vindices[0] + Z];

        v[X] = model->vertices[3 * T(i).vindices[2] + X] -
            model->vertices[3 * T(i).vindices[0] + X];
        v[Y] = model->vertices[3 * T(i).vindices[2] + Y] -
            model->vertices[3 * T(i).vindices[0] + Y];
        v[Z] = model->vertices[3 * T(i).vindices[2] + Z] -
            model->vertices[3 * T(i).vindices[0] + Z];

        GRRLIB_Cross(u, v, &model->facetnorms[3 * (i+1)]);
        GRRLIB_Normalize(&model->facetnorms[3 * (i+1)]);
    }
}

/**
 * Generates texture coordinates according to a
 * linear projection of the texture map.  It generates these by
 * linearly mapping the vertices onto a square.
 *
 * @param model Pointer to initialized GRRLIB_Model structure.
 */
void GRRLIB_LinearTexture(GRRLIB_Model* model) {
    f32 dimensions[3];

    if(model == NULL) {
        return;
    }

    if(model->texcoords) {
        free(model->texcoords);
    }
    model->numtexcoords = model->numvertices;
    model->texcoords=(f32*)malloc(sizeof(f32)*2*(model->numtexcoords+1));

    GRRLIB_Dimensions(model, dimensions);
    const f32 scalefactor =
        2.0 / fabsf(fmaxf(fmaxf(dimensions[0], dimensions[1]), dimensions[2]));

    // do the calculations
    for(u32 i = 1; i <= model->numvertices; i++) {
        const f32 x = model->vertices[3 * i + 0] * scalefactor;
        const f32 y = model->vertices[3 * i + 2] * scalefactor;
        model->texcoords[2 * i + 0] = (x + 1.0) / 2.0;
        model->texcoords[2 * i + 1] = (y + 1.0) / 2.0;
    }

    // go through and put texture coordinate indices in all the triangles
    GRRLIB_Group *group = model->groups;
    while(group) {
        for(u32 i = 0; i < group->numtriangles; i++) {
            T(group->triangles[i]).tindices[0] = T(group->triangles[i]).vindices[0];
            T(group->triangles[i]).tindices[1] = T(group->triangles[i]).vindices[1];
            T(group->triangles[i]).tindices[2] = T(group->triangles[i]).vindices[2];
        }
        group = group->next;
    }
}

/**
 * Generates texture coordinates according to a
 * spherical projection of the texture map.  Sometimes referred to as
 * spheremap, or reflection map texture coordinates.  It generates
 * these by using the normal to calculate where that vertex would map
 * onto a sphere.  Since it is impossible to map something flat
 * perfectly onto something spherical, there is distortion at the
 * poles.  This particular implementation causes the poles along the X
 * axis to be distorted.
 *
 * @param model Pointer to initialized GRRLIB_Model structure.
 */
void GRRLIB_SpheremapTexture(GRRLIB_Model* model) {
    if(model == NULL || model->normals == NULL) {
        return;
    }

    if (model->texcoords) {
        free(model->texcoords);
    }
    model->numtexcoords = model->numnormals;
    model->texcoords=(f32*)malloc(sizeof(f32)*2*(model->numtexcoords+1));

    // do the calculations
    for (u32 i = 1; i <= model->numnormals; i++) {
        f32 theta;
        f32 phi;
        const f32 z = model->normals[3 * i + 0];    /* re-arrange for pole distortion */
        const f32 y = model->normals[3 * i + 1];
        const f32 x = model->normals[3 * i + 2];
        const f32 r = sqrt((x * x) + (y * y));
        const f32 rho = sqrt((r * r) + (z * z));

        if(r == 0.0) {
            theta = 0.0;
            phi = 0.0;
        }
        else {
            if(z == 0.0) {
                phi = M_PI / 2.0;
            }
            else {
                phi = acos(z / rho);
            }

            if(y == 0.0) {
                theta = M_PI / 2.0;    /* acos(x / r); */
            }
            else {
                theta = asin(y / r) + (M_PI / 2.0);
            }
        }

        model->texcoords[2 * i + 0] = theta / M_PI;
        model->texcoords[2 * i + 1] = phi / M_PI;
    }

    // go through and put texcoord indices in all the triangles
    GRRLIB_Group* group = model->groups;
    while(group) {
        for (u32 i = 0; i < group->numtriangles; i++) {
            T(group->triangles[i]).tindices[0] = T(group->triangles[i]).nindices[0];
            T(group->triangles[i]).tindices[1] = T(group->triangles[i]).nindices[1];
            T(group->triangles[i]).tindices[2] = T(group->triangles[i]).nindices[2];
        }
        group = group->next;
    }
}

/**
 * Set the embedded file for the model.
 * @param file Pointer to the GRRLIB_EmbeddedFile structure containing the embedded file data.
 */
void GRRLIB_SetEmbeddedFile(GRRLIB_EmbeddedFile* file) {
    EmbeddedFile = file;
}
