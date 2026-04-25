#pragma once

#include <glad/glad.h>

#include "utils/base-types.h"
#include "gfx/vertex.h"
#include "gfx/color.h"

typedef struct AquaMesh {
    AquaVertex* vertices;
    usize vertices_count;

    GLuint* indices;
    usize indices_count;

    AquaColor color;

    GLuint vao, vbo, ebo;
} AquaMesh;

AquaMesh aqua_mesh_create(AquaVertex* vertices, usize vertices_count, GLuint* indices, usize indices_count, AquaColor color);
void aqua_mesh_draw(AquaMesh* mesh);
void aqua_mesh_destroy(AquaMesh* mesh);
