// pntr_clay.h - Clay renderer for pntr
#ifndef PNTR_CLAY_H
#define PNTR_CLAY_H

#include "clay.h"
#include "pntr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pntr_clay_error_callback)(Clay_ErrorData);

void pntr_clay_initialize(pntr_image* destinationImage, pntr_clay_error_callback errorHandler);
void pntr_clay_render(pntr_image* destinationImage, Clay_RenderCommandArray renderCommands, pntr_font** fonts, float deltaTime, float mouse_x, float mouse_y, bool isMouseDown, float mouse_wheel_x, float mouse_wheel_y);


#ifdef PNTR_CLAY_IMPLEMENTATION

static inline Clay_Dimensions _pntr_clay_measure_text(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
  pntr_font** fonts = userData;
  pntr_vector dim = pntr_measure_text_ex(fonts[config->fontId], text.chars, text.length);
  return (Clay_Dimensions) { dim.x, dim.y };
}
 
void pntr_clay_initialize(pntr_image* destinationImage, pntr_clay_error_callback errorHandler) {
  uint64_t totalMemorySize = Clay_MinMemorySize();
  Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
  Clay_Initialize(arena, (Clay_Dimensions) { destinationImage->width, destinationImage->height }, (Clay_ErrorHandler) { errorHandler });
}

void pntr_clay_render(pntr_image* destinationImage, Clay_RenderCommandArray renderCommands, pntr_font** fonts, float deltaTime, float mouse_x, float mouse_y, bool isMouseDown, float mouse_wheel_x, float mouse_wheel_y ) {
    Clay_SetLayoutDimensions((Clay_Dimensions) { destinationImage->width, destinationImage->height });
    Clay_SetPointerState((Clay_Vector2) { mouse_x, mouse_y }, isMouseDown);
    Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouse_wheel_x, mouse_wheel_y }, deltaTime);
    Clay_SetMeasureTextFunction(_pntr_clay_measure_text, fonts);

    // Simple scissor stack using static array to keep state in params
    static pntr_rectangle scissor_stack[32];
    static int scissor_depth = 0;
    
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
                pntr_color backgroundColor = pntr_new_color(config->backgroundColor.r, config->backgroundColor.g, config->backgroundColor.b, config->backgroundColor.a);
                pntr_draw_rectangle_rounded_fill(destinationImage, boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height, config->cornerRadius.topLeft, backgroundColor);
                break;
            }
            
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData *config = &renderCommand->renderData.text;
                pntr_color textColor = pntr_new_color(config->textColor.r, config->textColor.g, config->textColor.b, config->textColor.a);
                
                if (config->fontId < 0 || !fonts[config->fontId]) break;
                
                // Use stringContents instead of text
                pntr_draw_text_len(destinationImage, fonts[config->fontId], config->stringContents.chars, 
                                 config->stringContents.length, boundingBox.x, boundingBox.y, textColor);
                break;
            }

            
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                // Save current scissor state
                if (scissor_depth < 31) {
                    scissor_stack[scissor_depth] = pntr_image_get_clip(destinationImage);
                    scissor_depth++;
                }
                
                // Set new scissor rectangle - pntr_image_set_clip takes individual parameters
                pntr_image_set_clip(destinationImage, (int)boundingBox.x, (int)boundingBox.y, 
                                  (int)boundingBox.width, (int)boundingBox.height);
                break;
            }
            
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                // Restore previous scissor state
                if (scissor_depth > 0) {
                    scissor_depth--;
                    pntr_rectangle prev = scissor_stack[scissor_depth];
                    pntr_image_set_clip(destinationImage, prev.x, prev.y, prev.width, prev.height);
                }
                break;
            }
            
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                Clay_ImageRenderData *config = &renderCommand->renderData.image;
                
                // Use imageData instead of sourceImageData
                pntr_image* sourceImage = (pntr_image*)config->imageData;
                if (sourceImage) {
                    // Calculate scale factors and use proper pntr_draw_image_scaled signature
                    float scaleX = boundingBox.width / sourceImage->width;
                    float scaleY = boundingBox.height / sourceImage->height;
                    pntr_draw_image_scaled(destinationImage, sourceImage, 
                                         (int)boundingBox.x, (int)boundingBox.y, 
                                         scaleX, scaleY, 0.0f, 0.0f, PNTR_FILTER_BILINEAR);
                }
                break;
            }
            
            default: {
                // Unknown command type - skip silently
                break;
            }
        }
    }
    
    // Reset scissor depth for next frame
    scissor_depth = 0;
}



#endif // PNTR_CLAY_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // PNTR_CLAY_H
