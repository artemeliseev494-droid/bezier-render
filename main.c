#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point p0;
    Point p1;
    Point p2;
    Point p3;
} Curve;

typedef struct {
    unsigned char r, g, b;
} Color;

Color canvas[HEIGHT][WIDTH];

void clear_canvas(Color bg) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = bg;
        }
    }
}

void draw_pixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = color;
    }
}

void draw_bold_point(int cx, int cy, int radius, Color color) {
    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= radius * radius) {
                draw_pixel(x, y, color);
            }
        }
    }
}

Point get_cubic_bezier(Curve c, float t) {
    Point p;
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    p.x = uuu * c.p0.x + 3 * uu * t * c.p1.x + 3 * u * tt * c.p2.x + ttt * c.p3.x;
    p.y = uuu * c.p0.y + 3 * uu * t * c.p1.y + 3 * u * tt * c.p2.y + ttt * c.p3.y;
    return p;
}

int main() {
    Color bg_color = {15, 20, 35};       // Темно-синий фон
    Color curve_color = {190, 0, 255};   // Фиолетовый неон

    clear_canvas(bg_color);

    Curve my_curve = {
        {100, 500},  
        {200, 50},   
        {600, 550},  
        {700, 100}   
    };

    for (float t = 0.0f; t <= 1.0f; t += 0.0005f) {
        Point pt = get_cubic_bezier(my_curve, t);
        draw_bold_point((int)pt.x, (int)pt.y, 3, curve_color);
    }

    // ТЕПЕРЬ ПРОГРАММА САМА СОЗДАЕТ ФАЙЛ НАДИСКЕ
    FILE *f = fopen("render.ppm", "w");
    if (f == NULL) {
        printf("Ошибка\n");
        return 1;
    }


    fprintf(f, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            fprintf(f, "%d %d %d\n", canvas[y][x].r, canvas[y][x].g, canvas[y][x].b);
        }
    }

    fclose(f);
    printf("Успех\n");

    return 0;
}