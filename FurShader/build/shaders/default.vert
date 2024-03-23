#version 330 core

attribute vec3 ver_pos;
attribute vec3 ver_norm;
attribute vec2 ver_uv;
attribute float current_layer;

uniform mat4 projection;
uniform mat4 view;
uniform int layers;
uniform float fur_length;

varying vec3 normal;
varying vec2 uv;
varying float layer;

void main()
{
    const float curvature = 10;
    
    float shell_height = current_layer / layers;
    shell_height = pow(shell_height, fur_length);

    //reference: float3 P = IN.position.xyz + (IN.normal * FurLength);
    //based on P to move, I used pos which is an existint position to increase
    vec3 pos = ver_pos;
    pos += ver_norm * fur_length * shell_height;
    
    //follow y to simulate gravity, shell by shell, and then add to pos
    float k = pow(shell_height, curvature);
    pos += vec3(0.0, -0.1, 0.0) * k;

    gl_Position = projection * view * vec4(pos, 1.0);
    
    normal = ver_norm;
    uv = ver_uv;
    layer = current_layer;
}
