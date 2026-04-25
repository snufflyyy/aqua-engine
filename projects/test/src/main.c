#include <stdio.h>

#include <glad/glad.h>
#include <string.h>

#include "aqua.h"
#include "gfx/window/window.h"
#include "gfx/shader/shader.h"
#include "gfx/mesh/mesh.h"
#include "gfx/vertex.h"

static const AquaVertex QUAD_VERTICES[4] = {
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // bottom-left
    { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // bottom-right
    { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // top-right
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }  // top-left
};

static const GLuint QUAD_INDICES[6] = {
    0, 1, 2,
    2, 3, 0
};

int main(void) {
	AquaContextInitializeProperties aqua_initialize_properties = AQUA_CONTEXT_DEFAULT_INITIALIZE_PROPERTIES;
	aqua_initialize_properties.window_properties.title = "Aqua Engine - Test Project";
	aqua_initialize_properties.window_properties.vsync = true;

	AquaContext* aqua_context = aqua_initialize(aqua_initialize_properties);
	if (!aqua_context) {
		fprintf(stderr, "[ERROR] [Aqua] Failed to initialize aqua!\n");
		return -1;
	}

	mat4 projection;
	glm_perspective(glm_rad(60.0f), 1280.0f / 720.0f, 0.01f, 100.0f, projection);
	mat4 view = GLM_MAT4_IDENTITY_INIT;
	glm_translate(view, (vec3) { 0.0f, 0.0f, -1.0f });
	mat4 model = GLM_MAT4_IDENTITY_INIT;

	AquaShader shader = aqua_shader_create("../shaders/color/color.vert", "../shaders/color/color.frag");
	aqua_shader_bind(shader);
	AquaShaderUniformLocation shader_uniform_projection = aqua_shader_get_uniform_location(shader, "projection");
	AquaShaderUniformLocation shader_uniform_view = aqua_shader_get_uniform_location(shader, "view");
	AquaShaderUniformLocation shader_uniform_model = aqua_shader_get_uniform_location(shader, "model");
	AquaShaderUniformLocation shader_uniform_color = aqua_shader_get_uniform_location(shader, "color");
	aqua_shader_unbind();

	AquaVertex* vertices = (AquaVertex*) malloc(sizeof(QUAD_VERTICES));
	if (!vertices) {
		fprintf(stderr, "[ERROR] [Test Project] Failed to allocate memory for mesh vertices!\n");
		aqua_destroy(aqua_context);
		return -2;
	}
	memcpy(vertices, QUAD_VERTICES, sizeof(QUAD_VERTICES));

	GLuint* indices = (GLuint*) malloc(sizeof(QUAD_INDICES));
	if (!indices) {
		fprintf(stderr, "[ERROR] [Test Project] Failed to allocate memory for mesh indices!\n");
		aqua_destroy(aqua_context);
		return -3;
	}
	memcpy(indices, QUAD_INDICES, sizeof(QUAD_INDICES));

	AquaMesh mesh = aqua_mesh_create(vertices, (sizeof(QUAD_VERTICES) / sizeof(AquaVertex)), indices, (sizeof(QUAD_INDICES) / sizeof(GLuint)), (AquaColor) { 0.1f, 0.2f, 0.5f });

	while (aqua_context->running) {
		aqua_event(aqua_context);
		aqua_update(aqua_context);

		// all imgui code must be in between these two function calls
		aqua_window_imgui_begin(aqua_context->window);
			aqua_window_imgui_update(aqua_context->window);
			aqua_imgui_update(aqua_context);
		aqua_window_imgui_end(aqua_context->window);

		aqua_window_clear();

		aqua_shader_bind(shader);
		aqua_shader_set_mat4_uniform(shader_uniform_projection, projection);
		aqua_shader_set_mat4_uniform(shader_uniform_view, view);
		aqua_shader_set_mat4_uniform(shader_uniform_model, model);
		aqua_shader_set_vec3_uniform(shader_uniform_color, mesh.color);
		aqua_mesh_draw(&mesh);

		aqua_draw(aqua_context);
	}

	aqua_mesh_destroy(&mesh);
	aqua_shader_destroy(shader);
	aqua_destroy(aqua_context);

	return 0;
}
