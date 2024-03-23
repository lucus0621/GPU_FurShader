#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "./linmath.h"
#include "./utils.h"
#include "./render.h"

#include "./imgui/imgui_impl_sdl2.h"

#define FPS 60
#define FRAME_MS (1000/FPS)

#define ZOOM_RATE 2.8f
#define ZOOM_UP 0.95f
#define ZOOM_DOWN 1.09f

#define ZOOM_NEAR 1.2f
#define ZOOM_FAR 3.0f

#define ROTATION_BUMP 1.1f
#define ROTATION_DAMP 9.0f

struct Camera {
    vec3 eye;
    vec3 center;
    vec3 up;
};

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    render_initialize_context();
    
    Camera camera = {
        { 0.0f, 0.0f, 1.5f }, // eye
        { 0.0f, 0.0f, 0.0f }, // center
        { 0.0f, 1.0f, 0.0f }, // up
    };
    
    float zoom_target = camera.eye[2];
    vec2 view_angle = {0};
    vec2 rotation_speed = {0};
    mat4x4 view = {0};
    
    unsigned int current_time = 0;
    unsigned int previous_time = SDL_GetTicks();
    
    bool should_quit = false;
    while (!should_quit) {
        current_time = SDL_GetTicks();
        unsigned int elapsed_time = current_time - previous_time;
        previous_time = current_time;

        float dt = elapsed_time / 1000.0f;
        
        SDL_Event e = {0};
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (ImGui::GetIO().WantCaptureMouse) break;
            
            switch (e.type) {
                case SDL_QUIT: {
                    should_quit = true;
                } break;

                case SDL_MOUSEWHEEL: {
                    if (e.wheel.y > 0.0f) zoom_target *= ZOOM_UP;
                    else if (e.wheel.y < 0.0f) zoom_target *= ZOOM_DOWN;
                    
                    if (zoom_target < ZOOM_NEAR) zoom_target = ZOOM_NEAR;
                    else if (zoom_target > ZOOM_FAR) zoom_target = ZOOM_FAR;
                } break;

                case SDL_MOUSEMOTION: {
                    unsigned int mouse_state = SDL_GetMouseState(0, 0);
                    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) 
                    {
                        vec2 v = { (float) e.motion.xrel, (float) e.motion.yrel };
                        float mag = CLAMP(vec2_len(v) * ROTATION_BUMP, 12.0f, 20.0f);
                        //save the rotate speed
                        vec2 nv = {0};
                        //clear direaction and unit = 1
                        vec2_norm(nv, v);
                        //only realated to mouse speed
                        vec2_scale(nv, nv, mag * dt);
                        vec2_add(rotation_speed, rotation_speed, nv);
                    }
                } break;
            }
        }

        render_update_camera(view);
        render_update_time(SDL_GetTicks() / 1000.0f);//transfer to sec

        render_begin();
        
        if (!global_ctx.reload_fail) {
            render_sphere_controls();
            render_immediate_sphere();
        } else {
            render_error_screen();
        }
        
        render_end();

        //update camera values
        vec2 speed_over_time = { rotation_speed[0] * dt, rotation_speed[1] * dt };
        vec2_add(view_angle, view_angle, speed_over_time);
        view_angle[1] = CLAMP(view_angle[1], -PI32/2.0f, PI32/2.0f);

        move_towards(&camera.eye[2], zoom_target, dt, ZOOM_RATE);
        move_towards(&rotation_speed[0], 0.0f, dt, ROTATION_DAMP);
        move_towards(&rotation_speed[1], 0.0f, dt, ROTATION_DAMP);

        //update camera matrix
        mat4x4_identity(view); 
        mat4x4_look_at(view, camera.eye, camera.center, camera.up);
        mat4x4_rotate_X(view, view, view_angle[1]);
        mat4x4_rotate_Y(view, view, view_angle[0]);
        
        int wait_time = FRAME_MS - elapsed_time;
        if (wait_time > 0 && wait_time < FRAME_MS) SDL_Delay(wait_time);
    }

    render_destroy_context();
    
    return 0;
}
