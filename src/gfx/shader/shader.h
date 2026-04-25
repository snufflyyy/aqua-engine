#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef GLuint AquaShader;
typedef GLint AquaShaderUniformLocation;

AquaShader aqua_shader_create(const char* vertex_shader_source_path, const char* fragment_shader_source_path);
AquaShaderUniformLocation aqua_shader_get_uniform_location(AquaShader shader, const char* uniform_name);
void aqua_shader_bind(AquaShader shader);
void aqua_shader_set_mat4_uniform(AquaShaderUniformLocation uniform_location, mat4 value);
void aqua_shader_set_vec3_uniform(AquaShaderUniformLocation uniform_location, vec3 value);
void aqua_shader_set_float_uniform(AquaShaderUniformLocation uniform_location, GLfloat value);
void aqua_shader_set_int_uniform(AquaShaderUniformLocation uniform_location, GLint value);
void aqua_shader_unbind(void);
void aqua_shader_destroy(AquaShader shader);
