#include "gfx/window/window.h"
#include "SDL3/SDL_video.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

AquaWindow* aqua_window_create(AquaWindowProperties properties) {
	AquaWindow* window = (AquaWindow*) malloc(sizeof(AquaWindow));
	if (!window) {
		fprintf(stderr, "[ERROR] [Aqua] [Window] Failed to allocate memory for window!\n");
		return NULL;
	}

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "[ERROR] [Aqua] [Window] Failed to initialize SDL!\n");
		free(window);
		return NULL;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    #ifdef __APPLE__
   		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window->properties = properties;

    SDL_WindowFlags sdl_window_flags = SDL_WINDOW_OPENGL;
    if (properties.resizeable) { sdl_window_flags |= SDL_WINDOW_RESIZABLE; }
    if (window->properties.fullscreen) { sdl_window_flags |= SDL_WINDOW_FULLSCREEN; }

   	window->sdl_window = SDL_CreateWindow(properties.title, (i32) properties.width, (i32) properties.height, sdl_window_flags);
    if (!window->sdl_window) {
   		fprintf(stderr, "[ERROR] [Aqua] [Window] Failed to create SDL window!\n");
    	SDL_Quit();
       	free(window);
       	return NULL;
    }

    window->gl_context = SDL_GL_CreateContext(window->sdl_window);
    if (!window->gl_context) {
   		fprintf(stderr, "[ERROR] [Aqua] [Window] Failed to create OpenGL Context!\n");
    	SDL_DestroyWindow(window->sdl_window);
    	SDL_Quit();
       	free(window);
       	return NULL;
    }

    SDL_GL_MakeCurrent(window->sdl_window, window->gl_context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
  		fprintf(stderr, "[ERROR] [Aqua] [Window] Failed load GLAD!\n");
    	SDL_GL_DestroyContext(window->gl_context);
    	SDL_DestroyWindow(window->sdl_window);
    	SDL_Quit();
       	free(window);
       	return NULL;
    }


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    aqua_window_set_mouse_captured(window, window->properties.mouse_captured);
    aqua_window_set_vsync(window, window->properties.vsync);

    window->last_performance_counter = SDL_GetPerformanceCounter();
    window->performance_frequency = (double) SDL_GetPerformanceFrequency();

    window->fps = 0.0f;
    window->delta_time = 0.0;

    igCreateContext(NULL);
    ImGuiIO* ioptr = igGetIO_Nil();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForOpenGL(window->sdl_window, window->gl_context);
    ImGui_ImplOpenGL3_Init(NULL);

    window->running = true;

    #ifndef NDEBUG
	    printf("[INFO] [Aqua] [Window] Created Window:\n");
	    printf("\tVideo Driver: %s\n", SDL_GetCurrentVideoDriver());
	    printf("\tVender: %s\n", glGetString(GL_VENDOR));
	    printf("\tRenderer: %s\n", glGetString(GL_RENDERER));
	    printf("\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	    printf("\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	#endif

	return window;
}

void aqua_window_event(AquaWindow* window, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_QUIT: { window->running = false; } break;
        case SDL_EVENT_KEY_DOWN: {
            switch (event->key.scancode) {
                case SDL_SCANCODE_F11: { aqua_window_set_fullscreen(window, !window->properties.fullscreen); } break;
                case SDL_SCANCODE_ESCAPE: {
                    window->properties.mouse_captured = !window->properties.mouse_captured;
                    aqua_window_set_mouse_captured(window, window->properties.mouse_captured);
                } break;
                default: break;
            }
        } break;
		case SDL_EVENT_WINDOW_RESIZED: {
			aqua_window_resize(window, (u32) event->window.data1, (u32) event->window.data2);
		} break;
    }

    if (!window->properties.mouse_captured) { ImGui_ImplSDL3_ProcessEvent(event); }
}

void aqua_window_update(AquaWindow* window) {
	SDL_GL_SwapWindow(window->sdl_window);

	u64 performance_counter = SDL_GetPerformanceCounter();
	window->delta_time = (double) (performance_counter - window->last_performance_counter) / window->performance_frequency;
	window->last_performance_counter = performance_counter;

	if (window->delta_time > 0.0) {
	    window->fps = 1.0f / ((float) window->delta_time);
	}
}

void aqua_window_imgui_begin(AquaWindow* window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    igNewFrame();

    window->started_imgui_frame = true;
}

void aqua_window_imgui_performance_window(AquaWindow* window) {
	if (!window->started_imgui_frame) { return; }

	igBegin("Performance", &window->properties.show_performance_window, ImGuiWindowFlags_AlwaysAutoResize);
	igText("Vsync: %s", (window->properties.vsync) ? "Yes" : "No");
	igText("FPS: %0.2f", window->fps);
	igText("Deltatime: %0.2f", window->delta_time * 1000.0);
	igEnd();
}

void aqua_window_imgui_properties_window(AquaWindow* window) {
	if (!window->started_imgui_frame) { return; }

	igBegin("Window Properties", &window->properties.show_properties_window, ImGuiWindowFlags_AlwaysAutoResize);
	igText("Size: %ux%u", window->properties.width, window->properties.height);
	if (igCheckbox("Resizeable", &window->properties.resizeable)) { aqua_window_set_resizeable(window, window->properties.resizeable); }
	if (igCheckbox("Fullscreen", &window->properties.fullscreen)) { aqua_window_set_fullscreen(window, window->properties.fullscreen); }
	if (igCheckbox("Vsync", &window->properties.vsync)) { aqua_window_set_vsync(window, window->properties.vsync); }
	if (igCheckbox("Capture Mouse", &window->properties.mouse_captured)) { aqua_window_set_mouse_captured(window, window->properties.mouse_captured); }
	igCheckbox("Performance Window", &window->properties.show_performance_window);
	igEnd();
}

void aqua_window_imgui_end(AquaWindow* window) {
	if (!window->started_imgui_frame) { return; }

	if (window->properties.show_properties_window) {
		aqua_window_imgui_properties_window(window);
	}
	if (window->properties.show_performance_window) {
		aqua_window_imgui_performance_window(window);
	}

    igEndFrame();

    window->started_imgui_frame = false;
}

void aqua_window_imgui_draw(AquaWindow* window) {
	if (window->started_imgui_frame) { return; }

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void aqua_window_set_resizeable(AquaWindow* window, bool value) {
	SDL_SetWindowResizable(window->sdl_window, value);
}

void aqua_window_set_fullscreen(AquaWindow* window, bool value) {
	SDL_SetWindowFullscreen(window->sdl_window, value);
	window->properties.fullscreen = value;
}

void aqua_window_set_vsync(AquaWindow* window, bool value) {
	SDL_GL_SetSwapInterval((value) ? 1 : 0);
	window->properties.vsync = value;
}

void aqua_window_set_mouse_captured(AquaWindow *window, bool value) {
	SDL_SetWindowRelativeMouseMode(window->sdl_window, value);
    SDL_SetWindowMouseGrab(window->sdl_window, value);
}

void aqua_window_set_clear_color(vec3 color) {
    glClearColor(color[0], color[1], color[2], 1.0f);
}

void aqua_window_clear(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void aqua_window_resize(AquaWindow *window, u32 new_width, u32 new_height) {
	window->properties.width = new_width;
	window->properties.height = new_height;
	glViewport(0, 0, (GLsizei) new_width, (GLsizei) new_height);
}

void aqua_window_get_size(AquaWindow* window, u32* width, u32* height) {
	SDL_GetWindowSize(window->sdl_window, (int*) width, (int*) height);
}

void aqua_window_destroy(AquaWindow* window) {
	window->running = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);
 	SDL_GL_DestroyContext(window->gl_context);
   	SDL_DestroyWindow(window->sdl_window);
   	SDL_Quit();
    free(window);

    #ifndef NDEBUG
    	printf("[INFO] [Aqua] [Window] Window Destroyed\n");
    #endif
}
