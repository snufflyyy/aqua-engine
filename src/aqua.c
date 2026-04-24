#include "aqua.h"
#include "SDL3/SDL_events.h"

#include <stdio.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

#include "gfx/window/window.h"

AquaContext* aqua_initialize(AquaContextInitializeProperties initialize_properties) {
	AquaContext* aqua_context = (AquaContext*) malloc(sizeof(AquaContext));
	if (!aqua_context) {
		fprintf(stderr, "[ERROR] [Aqua] Failed to allocate memory for Aqua context!\n");
		return NULL;
	}

	aqua_context->properties.show_aqua_properties_window = initialize_properties.show_properties_window;

	aqua_context->window = aqua_window_create(initialize_properties.window_properties);
	if (!aqua_context) {
		fprintf(stderr, "[ERROR] [Aqua] Failed to create window!\n");
		free(aqua_context);
		return NULL;
	}

	aqua_context->running = true;

	#ifndef NDEBUG
		printf("[INFO] [Aqua] Created Aqua context!\n");
	#endif

	printf("[INFO] [Aqua] Press P to toggle Aqua properties window\n");

	return aqua_context;
}

void aqua_event(AquaContext* aqua_context) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_KEY_DOWN: {
				switch (event.key.scancode) {
					case SDL_SCANCODE_P: {
						aqua_context->properties.show_aqua_properties_window = !aqua_context->properties.show_aqua_properties_window;
					} break;
					default: break;
				}
			} break;
			default: break;
		}

		aqua_window_event(aqua_context->window, &event);
	}
}

void aqua_update(AquaContext* aqua_context) {
	aqua_window_update(aqua_context->window);
	aqua_context->running = aqua_context->window->running;
}

void aqua_imgui_update(AquaContext* aqua_context) {
	if (aqua_context->properties.show_aqua_properties_window) {
		aqua_imgui_properties_window(aqua_context);
	}
}

void aqua_imgui_properties_window(AquaContext* aqua_context) {
	igBegin("Aqua Properties", &aqua_context->properties.show_aqua_properties_window, ImGuiWindowFlags_None);
	igCheckbox("Show Window Properties", &aqua_context->window->properties.show_properties_window);
	igEnd();
}

void aqua_draw(AquaContext* aqua_context) {
	aqua_window_clear();
	aqua_window_imgui_draw(aqua_context->window);
}

void aqua_destroy(AquaContext* aqua_context) {
	aqua_window_destroy(aqua_context->window);
	free(aqua_context);

	#ifndef NDEBUG
		printf("[INFO] [Aqua] Destroyed Aqua context!\n");
	#endif
}
