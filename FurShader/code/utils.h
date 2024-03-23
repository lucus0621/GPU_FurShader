#define internal static
#define global static

#define UNUSED(x) ((void)(x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, l, h) (MIN(MAX((x), (l)), (h)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SIGN(x) ((x) < 0 ? -1 : 1)

const char *load_entire_file(const char *filename);
void move_towards(float *val, float target, float dt, float rate_up, float rate_down = -1.0f);

