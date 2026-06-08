#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 200
#define EMPTY_CHAR '_'
#define DRAW_CHAR '*'

typedef enum {OBJ_LINE, OBJ_RECT, OBJ_CIRCLE, OBJ_TRIANGLE} ObjType;

typedef struct {
    ObjType type;
    int params[6];
} Object;

static Object objects[MAX_OBJECTS];
static int obj_count = 0;
static char canvas[HEIGHT][WIDTH];

void clear_canvas() {
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            canvas[y][x] = EMPTY_CHAR;
}

int in_bounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

void plot(int x, int y) {
    if (in_bounds(x, y))
        canvas[y][x] = DRAW_CHAR;
}

void print_canvas() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x)
            putchar(canvas[y][x]);
        putchar('\n');
    }
}

void draw_line_int(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        plot(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_rect_int(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    for (int x = left; x <= right; x++) {
        plot(x, top);
        plot(x, bottom);
    }

    for (int y = top; y <= bottom; y++) {
        plot(left, y);
        plot(right, y);
    }
}

void draw_circle_int(int cx, int cy, int r) {
    int x = r;
    int y = 0;
    int err = 1 - r;

    while (x >= y) {
        plot(cx + x, cy + y);
        plot(cx + y, cy + x);
        plot(cx - y, cy + x);
        plot(cx - x, cy + y);
        plot(cx - x, cy - y);
        plot(cx - y, cy - x);
        plot(cx + y, cy - x);
        plot(cx + x, cy - y);

        y++;

        if (err < 0)
            err += 2 * y + 1;
        else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void draw_triangle_int(int x1,int y1,int x2,int y2,int x3,int y3) {
    draw_line_int(x1,y1,x2,y2);
    draw_line_int(x2,y2,x3,y3);
    draw_line_int(x3,y3,x1,y1);
}

void redraw_all() {
    clear_canvas();

    for (int i = 0; i < obj_count; i++) {
        Object *o = &objects[i];

        switch (o->type) {
            case OBJ_LINE:
                draw_line_int(o->params[0], o->params[1],
                              o->params[2], o->params[3]);
                break;

            case OBJ_RECT:
                draw_rect_int(o->params[0], o->params[1],
                              o->params[2], o->params[3]);
                break;

            case OBJ_CIRCLE:
                draw_circle_int(o->params[0], o->params[1],
                                o->params[2]);
                break;

            case OBJ_TRIANGLE:
                draw_triangle_int(o->params[0], o->params[1],
                                  o->params[2], o->params[3],
                                  o->params[4], o->params[5]);
                break;
        }
    }
}

void list_objects() {
    if (obj_count == 0) {
        printf("No objects.\n");
        return;
    }

    for (int i = 0; i < obj_count; i++) {
        printf("[%d] ", i);

        switch (objects[i].type) {
            case OBJ_LINE:
                printf("Line (%d,%d)-(%d,%d)\n",
                       objects[i].params[0], objects[i].params[1],
                       objects[i].params[2], objects[i].params[3]);
                break;

            case OBJ_RECT:
                printf("Rectangle (%d,%d)-(%d,%d)\n",
                       objects[i].params[0], objects[i].params[1],
                       objects[i].params[2], objects[i].params[3]);
                break;

            case OBJ_CIRCLE:
                printf("Circle Center(%d,%d) Radius=%d\n",
                       objects[i].params[0], objects[i].params[1],
                       objects[i].params[2]);
                break;

            case OBJ_TRIANGLE:
                printf("Triangle (%d,%d)-(%d,%d)-(%d,%d)\n",
                       objects[i].params[0], objects[i].params[1],
                       objects[i].params[2], objects[i].params[3],
                       objects[i].params[4], objects[i].params[5]);
                break;
        }
    }
}

void add_line() {
    int x1,y1,x2,y2;
    printf("Enter x1 y1 x2 y2: ");
    scanf("%d%d%d%d",&x1,&y1,&x2,&y2);

    objects[obj_count].type = OBJ_LINE;
    objects[obj_count].params[0]=x1;
    objects[obj_count].params[1]=y1;
    objects[obj_count].params[2]=x2;
    objects[obj_count].params[3]=y2;
    obj_count++;
}

void add_rect() {
    int x1,y1,x2,y2;
    printf("Enter x1 y1 x2 y2: ");
    scanf("%d%d%d%d",&x1,&y1,&x2,&y2);

    objects[obj_count].type = OBJ_RECT;
    objects[obj_count].params[0]=x1;
    objects[obj_count].params[1]=y1;
    objects[obj_count].params[2]=x2;
    objects[obj_count].params[3]=y2;
    obj_count++;
}

void add_circle() {
    int cx,cy,r;
    printf("Enter center x y and radius: ");
    scanf("%d%d%d",&cx,&cy,&r);

    objects[obj_count].type = OBJ_CIRCLE;
    objects[obj_count].params[0]=cx;
    objects[obj_count].params[1]=cy;
    objects[obj_count].params[2]=r;
    obj_count++;
}

void add_triangle() {
    int x1,y1,x2,y2,x3,y3;
    printf("Enter x1 y1 x2 y2 x3 y3: ");
    scanf("%d%d%d%d%d%d",&x1,&y1,&x2,&y2,&x3,&y3);

    objects[obj_count].type = OBJ_TRIANGLE;
    objects[obj_count].params[0]=x1;
    objects[obj_count].params[1]=y1;
    objects[obj_count].params[2]=x2;
    objects[obj_count].params[3]=y2;
    objects[obj_count].params[4]=x3;
    objects[obj_count].params[5]=y3;
    obj_count++;
}

void delete_object() {
    int idx;

    if (obj_count == 0) {
        printf("No objects to delete.\n");
        return;
    }

    list_objects();

    printf("Enter object index to delete: ");
    scanf("%d",&idx);

    if (idx < 0 || idx >= obj_count) {
        printf("Invalid index.\n");
        return;
    }

    for (int i = idx; i < obj_count - 1; i++)
        objects[i] = objects[i + 1];

    obj_count--;
    redraw_all();
}

void modify_object() {
    int idx;

    if (obj_count == 0) {
        printf("No objects to modify.\n");
        return;
    }

    list_objects();

    printf("Enter object index to modify: ");
    scanf("%d",&idx);

    if (idx < 0 || idx >= obj_count) {
        printf("Invalid index.\n");
        return;
    }

    Object *o = &objects[idx];

    switch (o->type) {
        case OBJ_LINE:
        case OBJ_RECT:
            printf("Enter new x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &o->params[0], &o->params[1],
                  &o->params[2], &o->params[3]);
            break;

        case OBJ_CIRCLE:
            printf("Enter new center x y radius: ");
            scanf("%d%d%d",
                  &o->params[0], &o->params[1],
                  &o->params[2]);
            break;

        case OBJ_TRIANGLE:
            printf("Enter new x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &o->params[0], &o->params[1],
                  &o->params[2], &o->params[3],
                  &o->params[4], &o->params[5]);
            break;
    }

    redraw_all();
}

void save_canvas() {
    char filename[100];

    printf("Enter filename: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Cannot open file.\n");
        return;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            fputc(canvas[y][x], fp);

        fputc('\n', fp);
    }

    fclose(fp);
    printf("Picture saved successfully.\n");
}

int main() {
    int choice;

    clear_canvas();

    while (1) {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Line\n");
        printf("2. Add Rectangle\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. List Objects\n");
        printf("6. Delete Object\n");
        printf("7. Modify Object\n");
        printf("8. Display Picture\n");
        printf("9. Save Picture\n");
        printf("10. Clear All Objects\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice) {
            case 1:
                add_line();
                redraw_all();
                break;

            case 2:
                add_rect();
                redraw_all();
                break;

            case 3:
                add_circle();
                redraw_all();
                break;

            case 4:
                add_triangle();
                redraw_all();
                break;

            case 5:
                list_objects();
                break;

            case 6:
                delete_object();
                break;

            case 7:
                modify_object();
                break;

            case 8:
                print_canvas();
                break;

            case 9:
                save_canvas();
                break;

            case 10:
                obj_count = 0;
                redraw_all();
                printf("All objects cleared.\n");
                break;

            case 0:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}


