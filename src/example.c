#define CLAY_IMPLEMENTATION
#include "clay.h"

#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

#define PNTR_CLAY_IMPLEMENTATION
#include "pntr_clay.h"

// array of fonts you want to use
static pntr_font* fonts[1] = {};

void HandleClayErrors(Clay_ErrorData errorData) {
    pntr_app_log(PNTR_APP_LOG_ERROR, errorData.errorText.chars);
}

bool Init(pntr_app* app) {
    pntr_clay_initialize(app->screen, HandleClayErrors);
    fonts[0] = pntr_load_font_default();

    return true;
}

// Here I implement my UI as a stand-alone function
Clay_RenderCommandArray DrawUI() {
    Clay_BeginLayout();
    
    // Main container with padding and background
    CLAY({
        .id = CLAY_ID("MainContainer"),
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .padding = CLAY_PADDING_ALL(16)
        },
        .backgroundColor = { 250, 250, 255, 255 }
    }) {
        // Header bar
        CLAY({
            .id = CLAY_ID("Header"),
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(60) },
                .padding = CLAY_PADDING_ALL(12),
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = { 100, 150, 200, 255 },
            .cornerRadius = CLAY_CORNER_RADIUS(8)
        }) {
            CLAY_TEXT(CLAY_STRING("Header Title"), CLAY_TEXT_CONFIG({
                .fontSize = 24,
                .textColor = { 255, 255, 255, 255 }
            }));
        }
        
        // Content area with sidebar and main panel
        CLAY({
            .id = CLAY_ID("ContentArea"),
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .childGap = 16,
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            }
        }) {
            // Sidebar
            CLAY({
                .id = CLAY_ID("Sidebar"),
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(200), CLAY_SIZING_GROW(0) },
                    .padding = CLAY_PADDING_ALL(16),
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 8
                },
                .backgroundColor = { 220, 220, 220, 255 },
                .cornerRadius = CLAY_CORNER_RADIUS(6)
            }) {
                CLAY_TEXT(CLAY_STRING("Sidebar"), CLAY_TEXT_CONFIG({
                    .fontSize = 18,
                    .textColor = { 50, 50, 50, 255 }
                }));
                
                // Sidebar items
                for (int i = 0; i < 3; i++) {
                    CLAY({
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(40) },
                            .padding = CLAY_PADDING_ALL(8)
                        },
                        .backgroundColor = { 180, 180, 180, 255 },
                        .cornerRadius = CLAY_CORNER_RADIUS(4)
                    }) {
                        CLAY_TEXT(CLAY_STRING("Item"), CLAY_TEXT_CONFIG({
                            .fontSize = 14,
                            .textColor = { 50, 50, 50, 255 }
                        }));
                    }
                }
            }
            
            // Main content panel
            CLAY({
                .id = CLAY_ID("MainContent"),
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                    .padding = CLAY_PADDING_ALL(24)
                },
                .backgroundColor = { 240, 240, 240, 255 },
                .cornerRadius = CLAY_CORNER_RADIUS(6)
            }) {
                CLAY_TEXT(CLAY_STRING("Main Content Area"), CLAY_TEXT_CONFIG({
                    .fontSize = 20,
                    .textColor = { 50, 50, 50, 255 }
                }));
            }
        }
    }
    
    return Clay_EndLayout();
}



bool Update(pntr_app* app, pntr_image* screen) {
    // pntr_clear_background(screen, PNTR_WHITE);
    // pntr_draw_circle_fill(screen, screen->width / 2, screen->height / 2, 100, PNTR_BLUE);

    pntr_clay_render(
        screen,
        DrawUI(),
        fonts,
        pntr_app_delta_time(app),
        pntr_app_mouse_x(app), // maybe pntr_app_mouse_delta_x?
        pntr_app_mouse_y(app),  // maybe pntr_app_mouse_delta_y?
        pntr_app_mouse_button_down(app, PNTR_APP_MOUSE_BUTTON_FIRST),
        
        // not sure how to get scroll x/y
        0.0f,
        0.0f
    );

    return true;
}

void Close(pntr_app* app) {
    // Uninitialize the application.
}

pntr_app Main(int argc, char* argv[]) {
    return (pntr_app) {
        .width = 800,
        .height = 450,
        .title = "pntr_clay: example",
        .init = Init,
        .update = Update,
        .close = Close,
        .fps = 60
    };
}