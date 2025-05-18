#include "std.h" 
#include "str.h"
// #include "math.h"

#define SCREEN_WIDTH 800    
#define SCREEN_HEIGHT 800
#define THICKNESS 20  
#define MAX_LENGTH 1024

color COLOR_BACKGROUND = {255, 255, 255}; 
color COLOR_WALL = {0, 0, 0};      
color COLOR_SNACK = {255, 105, 180}; 
color COLOR_BODY = {255, 182, 193};
color COLOR_FOOD = {0, 0, 255};
color COLOR_LABEL = {255, 0, 0};
color COLOR_HINT = {0, 255, 0};

static int counter = 0;
int over = 0;
int restart = 0;

void sleep_ms(int ms){
    volatile long long count = (long long)ms * 200;
    while(count--);
}

void draw_rectangle(int x, int y, int width, int height, color c) {
    point p1 = {x, y};
    point p2 = {x + width, y};
    point p3 = {x, y + height};
    point p4 = {x + width, y + height};
    draw_triangle(&p1, &p2, &p3, &c);
    draw_triangle(&p2, &p4, &p3, &c);
}

typedef struct{
    int x[MAX_LENGTH];
    int y[MAX_LENGTH];
    int length;
    char direction;
}Snake;

typedef struct{
    int x;
    int y;
}Food;

Snake snake;
Food food;

int hash_snake() {
    int h = 0;
    for (int i = 0; i < snake.length; i++) {
        h = h * 31 + snake.x[i] * 17 + snake.y[i] * 23;
        h = h ^ (h >> 5);
    }
    return h;
}

void grow_food(){
    int x1 = 1;
    int x2 = (SCREEN_WIDTH/ THICKNESS) - 2;
    int y1 = 1;
    int y2 = (SCREEN_HEIGHT/ THICKNESS) - 2;

    for (int attempts = 0; attempts < 1000; attempts++) {
        int h = hash_snake() + counter++;
        if (h < 0) h = -h;
        int new_x = x1 + (h % (x2 - x1 + 1));
        h = (h * 5 + 3) ^ (h << 2);
        if (h < 0) h = -h;
        int new_y = y1 + (h % (y2 - y1 + 1));

        int conflict = 0;
        for (int i = 0; i < snake.length; i++) {
            if (snake.x[i] == new_x && snake.y[i] == new_y) {
                conflict = 1;
                break;
            }
        }

        if (!conflict) {
            food.x = new_x;
            food.y = new_y;
            return;
        }
    }
    food.x = 1;
    food.y = 1;
}

void draw_food(){
    int x1 = (food.x* THICKNESS);
    int y1 = (food.y* THICKNESS);
    draw_rectangle(x1, y1, THICKNESS, THICKNESS, COLOR_FOOD);
}

void update_snake(int grow){
    if(over)return;
    int new_x = snake.x[0];
    int new_y = snake.y[0];
    switch(snake.direction){
        case 'U': new_y -= 1; break;
        case 'D': new_y += 1; break;
        case 'L': new_x -= 1; break;
        case 'R': new_x += 1; break;
    }
    if(new_x <= 0 || new_x >= (SCREEN_WIDTH / THICKNESS) - 1 || new_y <= 0 || new_y >= (SCREEN_HEIGHT / THICKNESS) - 1) {
        over = 1;
        return;
    }
    for(int i = 1; i < snake.length; i++) {
        if(new_x == snake.x[i] && new_y == snake.y[i]) {
            over = 1;
            return;
        }
    }
    if(grow){
        snake.length++;
    }
    for(int i = snake.length - 1; i > 0; i--){
        snake.x[i] = snake.x[i-1];
        snake.y[i] = snake.y[i-1];
    }
    snake.x[0] = new_x;
    snake.y[0] = new_y;
}

void move(char ch){
    if(over)return;
    if(ch >= 'A' && ch <= 'Z'){
        ch = ch + ('a' - 'A');
    }
    char new_dir = ch;
    switch(ch){
        case 'w':new_dir = 'U';break;
        case 's':new_dir = 'D';break;
        case 'a':new_dir = 'L';break;
        case 'd':new_dir = 'R';break;
        default: return;
    }
    if((snake.direction == 'U' && new_dir == 'D') || (snake.direction == 'D' && new_dir == 'U') || (snake.direction == 'L' && new_dir == 'R') || (snake.direction == 'R' && new_dir == 'L'))
        return;

    snake.direction = new_dir;
}

void init_snake(){
    snake.x[0] = (SCREEN_WIDTH/ THICKNESS) / 2;
    snake.y[0] = (SCREEN_HEIGHT/ THICKNESS) / 2;
    snake.length = 1;
    snake.direction = 'U';
}

void draw_snake(){
    for(int i = 0; i < snake.length; i++){
        int x1 = (snake.x[i]* THICKNESS);
        int y1 = (snake.y[i]* THICKNESS);
        if(i == 0)
            draw_rectangle(x1, y1, THICKNESS, THICKNESS, COLOR_SNACK);
        else
            draw_rectangle(x1, y1, THICKNESS, THICKNESS, COLOR_BODY);
    }
}

void draw_map(){
    draw_rectangle(0, 0, SCREEN_WIDTH, THICKNESS, COLOR_WALL);
    draw_rectangle(0, SCREEN_HEIGHT - THICKNESS, SCREEN_WIDTH, THICKNESS, COLOR_WALL);
    draw_rectangle(0, THICKNESS, THICKNESS, SCREEN_HEIGHT - (2 * THICKNESS), COLOR_WALL);
    draw_rectangle(SCREEN_WIDTH - THICKNESS,  THICKNESS, THICKNESS, SCREEN_HEIGHT - (2 * THICKNESS), COLOR_WALL);
}

int eat(){
    return snake.x[0] == food.x && snake.y[0] == food.y;
}

void draw_gaming(){
    init_snake();
    grow_food();
    while(over != 1){
        draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BACKGROUND);
        draw_map();
        draw_food();
        draw_snake();
        flush();

        if(kbhit()){
            char ch = vgetch();       
            move(ch);                
        }

        int grow = eat();
        update_snake(grow);

        if(grow){
            grow_food();
        }

        sleep_ms(10);
    }
}

void draw_end(){
    while(restart == 0){
        point title = {SCREEN_WIDTH / 2- 110, SCREEN_HEIGHT / 2 - 60};
        char* msg1 = "GAME OVER!        ";
        int font1 = 40; 
        draw_label(&title, msg1, &COLOR_LABEL, font1);
        point hint = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 20};
        char* msg2 = "Press m to restart";
        int font2 = 20; 
        draw_label(&hint, msg2, &COLOR_HINT, font2);
        flush();
        sleep_ms(10);
        if(kbhit()){
            char ch = vgetch();       
            if(ch == 'm' || ch == 'M'){ 
                restart = 1; 
                draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BACKGROUND);
                flush();
                break;
            }     
        }
    }
}

int main(int argc, char* argv[]) {
    while(1){
        over = 0;
        restart = 0;
        draw_gaming();
        draw_end();
    }
    return 0;
}