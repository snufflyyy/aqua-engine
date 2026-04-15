#pragma once

#include <stdbool.h>

#include "gfx/window/window.h"

typedef struct AquaContextInitializeProperties {
	AquaWindowProperties window_properties;
	bool show_properties_window;
} AquaContextInitializeProperties;

typedef struct AquaContextProperties {
	bool show_aqua_properties_window;
} AquaContextProperties;

typedef struct AquaContext {
	bool running;
	AquaContextProperties properties;

	AquaWindow* window;
} AquaContext;

AquaContext* aqua_initialize(AquaContextInitializeProperties properties);
void aqua_event(AquaContext* aqua_context);
void aqua_update(AquaContext* aqua_context);
void aqua_imgui_update(AquaContext* aqua_context);
void aqua_imgui_properties_window(AquaContext* aqua_context);
void aqua_draw(AquaContext* aqua_context);
void aqua_destroy(AquaContext* aqua_context);
