#include "gfx/mesh/mesh.h"
#include "gfx/vertex.h"

#include <glad/glad.h>

#include "utils/base-types.h"

AquaMesh aqua_mesh_create(AquaVertex* vertices, usize vertices_count, GLuint* indices, usize indices_count, AquaColor color) {
	AquaMesh mesh = {
		.vertices = vertices,
		.vertices_count = vertices_count,

		.indices = indices,
		.indices_count = indices_count,
	};

	glm_vec3_copy(color, mesh.color);

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glBufferData(GL_ARRAY_BUFFER, (u32) mesh.vertices_count * sizeof(AquaVertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (u32) mesh.indices_count * sizeof(GLuint), &mesh.indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AquaVertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AquaVertex), (void*) offsetof(AquaVertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AquaVertex), (void*) offsetof(AquaVertex, texture_coords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return mesh;
}

void aqua_mesh_draw(AquaMesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh->indices_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void aqua_mesh_destroy(AquaMesh* mesh) {
	if (!mesh) { return; }

	if (mesh->vertices) { free(mesh->vertices); }
	mesh->vertices_count = 0;
	if (mesh->indices) { free(mesh->indices); }
	mesh->indices_count = 0;

	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);
}
