#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

#include "utils/base-types.h"
#include "gfx/color.h"

typedef struct AquaWindowProperties {
	bool show_properties_window;

	u32 width, height;
	char* title;
	bool resizeable;
	bool fullscreen;
	bool vsync;
	bool mouse_captured;

	bool rainbow_clear_color; // for fun
	float rainbow_clear_color_speed;
	AquaColor clear_color;

	bool show_performance_window;
	bool use_imgui_dockspace;
} AquaWindowProperties;

typedef struct AquaWindow {
	SDL_Window* sdl_window;
	SDL_GLContext gl_context;
	ImGuiContext* imgui_context;

	AquaWindowProperties properties;
	bool running;

	bool started_imgui_frame;
	u64 last_performance_counter;
	double performance_frequency;

	float fps;
	double delta_time;
} AquaWindow;

AquaWindow* aqua_window_create(AquaWindowProperties properties);
void aqua_window_event(AquaWindow* window, SDL_Event* event);
void aqua_window_update(AquaWindow* window);
void aqua_window_imgui_begin(AquaWindow* window);
void aqua_window_imgui_update(AquaWindow* window);
void aqua_window_imgui_properties_window(AquaWindow* window);
void aqua_window_imgui_performance_window(AquaWindow* window);
void aqua_window_imgui_flags(AquaWindow* window);
void aqua_window_imgui_end(AquaWindow* window);
void aqua_window_imgui_draw(AquaWindow* window);
void aqua_window_set_resizeable(AquaWindow* window, bool value);
void aqua_window_set_fullscreen(AquaWindow* window, bool value);
void aqua_window_set_vsync(AquaWindow* window, bool value);
void aqua_window_set_mouse_captured(AquaWindow* window, bool value);
void aqua_window_set_clear_color(AquaColor color);
void aqua_window_clear(void);
void aqua_window_resize(AquaWindow* window, u32 new_width, u32 new_height);
void aqua_window_get_size(AquaWindow* window, u32* width, u32* height);
void aqua_window_destroy(AquaWindow* window);
