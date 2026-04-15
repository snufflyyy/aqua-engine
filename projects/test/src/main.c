#include <stdio.h>

#include "aqua.h"
#include "gfx/window/window.h"

int main(void) {
	AquaContextInitializeProperties aqua_initialize_properties = {
		.window_properties = {
			.width = 1280,
			.height = 720,
			.title = "Aqua Engine - Test Project",
			.fullscreen = false,
			.resizeable = false,
			.vsync = true,
		},
		.show_properties_window = true,
	};

	AquaContext* aqua_context = aqua_initialize(aqua_initialize_properties);
	if (!aqua_context) {
		fprintf(stderr, "[ERROR] [Aqua] Failed to initialize aqua!\n");
		return -1;
	}

	while (aqua_context->running) {
		aqua_event(aqua_context);
		aqua_update(aqua_context);

		// all imgui code must be in between these two function calls
		aqua_window_imgui_begin(aqua_context->window);
		aqua_imgui_update(aqua_context);
		aqua_window_imgui_end(aqua_context->window);

		aqua_draw(aqua_context);
	}

	aqua_destroy(aqua_context);

	return 0;
}
