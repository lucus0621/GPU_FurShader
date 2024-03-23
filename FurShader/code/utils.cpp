#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./utils.h"

const char *load_entire_file(const char *filename)
{
    char *contents = 0;
    FILE *file = fopen(filename, "rb");
    if (file == 0) {
        fprintf(stderr, "[ERROR] :: Could not open file: %s\n", filename);
        goto error;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    contents = (char *) malloc(sizeof(char) * size + 1);
    if (contents == 0) {
        fprintf(stderr, "[ERROR] :: Could not allocate memory for file: %s\n", filename);
        goto error;
    }
    
    if (fread(contents, sizeof(char), size, file) != size) {
        fprintf(stderr, "[ERROR] :: Could not read entire file: %s\n", filename);
        goto error;
    }

    contents[size] = '\0';
    fclose(file);    
    return(contents);

error:
    if (contents) free(contents);
    if (file) fclose(file);
    
    exit(1);
}

void move_towards(float *value, float target, float dt, float rate_up, float rate_down)
{
    float a = *value;

    if (a > target) {
        if (rate_down == -1.0f) rate_down = rate_up;
        a -= rate_down * dt;
        if (a < target) a = target;
        *value = a;
    } else if (a < target) {
        a += rate_up * dt;
        if (a > target) a = target;
        *value = a;
    }
}
