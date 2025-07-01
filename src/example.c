#define CLAY_IMPLEMENTATION
#include "clay.h"

#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_APP_IMPLEMENTATION
#include "pntr_app.h"

#define PNTR_CLAY_IMPLEMENTATION
#include "pntr_clay.h"

// array of fonts you want to use
static pntr_font* fonts[1] = {};

// demo image
pntr_image* profilePicture;

// enable debug by pressing D
static bool debugMode = false;


// Re-useable components are just normal functions
void SidebarItemComponent() {
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_FIXED(50)
            },
            .padding = CLAY_PADDING_ALL(16),
        },
        .backgroundColor = PNTR_CLAY_COLOR(PNTR_ORANGE)
    }) {
        CLAY_TEXT(CLAY_STRING("ITEM"), CLAY_TEXT_CONFIG({
            .fontSize = 20,
            .textColor = PNTR_CLAY_COLOR(PNTR_WHITE)
        }));
    }
}

// Here I implement my UI as a stand-alone function
Clay_RenderCommandArray DrawUI() {
    Clay_BeginLayout();
    
    // An example of laying out a UI with a fixed width sidebar and flexible width main content
    CLAY({
        .id = CLAY_ID("OuterContainer"),
        .layout = {
            .sizing = {
                CLAY_SIZING_GROW(0),
                CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16
        },
        .backgroundColor = PNTR_CLAY_COLOR(PNTR_BLACK)
    }) {
        CLAY({
            .id = CLAY_ID("SideBar"),
            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {
                .width = CLAY_SIZING_FIXED(300),
                .height = CLAY_SIZING_GROW(0) },
                .padding = CLAY_PADDING_ALL(16),
                .childGap = 16
            },
            .backgroundColor = PNTR_CLAY_COLOR(PNTR_RAYWHITE)
        }) {
            CLAY({
                .id = CLAY_ID("ProfilePictureOuter"),
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0)
                    },
                    .padding = CLAY_PADDING_ALL(16),
                    .childGap = 16,
                    .childAlignment = {
                        .y = CLAY_ALIGN_Y_CENTER
                    }
                },
                .backgroundColor = PNTR_CLAY_COLOR(PNTR_RED)
            }) {
                CLAY({
                    .id = CLAY_ID("ProfilePicture"),
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(60),
                            .height = CLAY_SIZING_FIXED(60)
                        }
                    },
                    .image = {
                        .imageData = profilePicture
                    }
                }) {}
                CLAY_TEXT(CLAY_STRING("Clay - UI Library"), CLAY_TEXT_CONFIG({
                    .fontSize = 24,
                    .textColor = PNTR_CLAY_COLOR(PNTR_WHITE)
                }));
            }

            // Standard C code like loops etc work inside components
            for (int i = 0; i < 5; i++) {
                SidebarItemComponent();
            }
        }

        CLAY({
            .id = CLAY_ID("MainContent"),
            .layout = { 
                .sizing = {
                    .width = CLAY_SIZING_GROW(0), 
                    .height = CLAY_SIZING_GROW(0)
                }
            },
            .backgroundColor = PNTR_CLAY_COLOR(PNTR_SKYBLUE)
        }) {}
    }
    
    return Clay_EndLayout();
}

void HandleClayErrors(Clay_ErrorData errorData) {
    pntr_app_log(PNTR_APP_LOG_ERROR, errorData.errorText.chars);
}

bool Init(pntr_app* app) {
    pntr_clay_initialize(app->screen, HandleClayErrors);
    fonts[0] = pntr_load_font_default();
    profilePicture = pntr_load_image("assets/logo.png");
    return true;
}

bool Update(pntr_app* app, pntr_image* screen){
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

    // pntr_app_key_pressed is not workign right on web
    if (pntr_app_key_pressed(app, PNTR_APP_KEY_D)) {
        debugMode = !debugMode;
    }
    Clay_SetDebugModeEnabled(debugMode);

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