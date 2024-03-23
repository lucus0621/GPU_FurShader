#include "./linmath.h"

#include <SDL2/SDL.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define MAX_QUADS (8 * 1024)
#define MAX_VERTICES (MAX_QUADS * 4)
#define MAX_INDICES (MAX_QUADS * 6)

#define SPHERE_SECTOR_COUNT 64
#define SPHERE_STACK_COUNT 32
#define SPHERE_RADIUS 0.6f

#define LAYERS_COUNT 50
#define FUR_LENGTH 0.32f

struct Render_Context {
    SDL_Window *window;
    SDL_GLContext gl_context;
    
    float width;
    float height;

    bool reload_fail;
};

extern Render_Context global_ctx;

void render_initialize_context();
void render_destroy_context();
void render_begin();
void render_end();
void render_reload_shaders();
void render_sphere_controls();
void render_immediate_sphere();
void render_error_screen();
void render_update_time(float time);
void render_update_camera(mat4x4 view);
void render_resize_window(int width, int height);
