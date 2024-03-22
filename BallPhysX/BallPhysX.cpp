#include <iostream>
#include <graphics.h>
#include <time.h>
#define CANVAS_WIDTH 1280
#define CANVAS_HEIGHT 720

int sqrt(int num) {
    int i = 0;
    while (i * i < num)i++;
    return i;
}

typedef struct M2d{
    int x;
    int y;
};

typedef struct Ball{
    int size=10;
    int springConstant = 1;
    M2d position = { 0,0 };
    M2d velocity= { 0,0 };
    M2d acceloration= { 0,0 };
};
typedef struct EnityChain{
    Ball* ball;
    EnityChain* next;
} ;

typedef struct World{
    int gravity;
    M2d size;
    EnityChain* enityChain;
};

void boxBounce(Ball* ball, M2d boxSize) {
    int* x = &ball->position.x, * y = &ball->position.y,
        * accx = &ball->acceloration.x, * accy = &ball->acceloration.y;
    if (*x < 0)*accx += -*x;
    else if (*x > boxSize.x)*accx += boxSize.x - *x;
    if (*y < 0)*accy += -*y;
    else if (*y > boxSize.y)*accy += boxSize.y - *y;
}

void refreshVelocity(Ball* ball) {
    M2d *acc = &ball->acceloration,
        *vel = &ball->velocity;
    vel->x += acc->x;
    vel->y += acc->y;
    acc->x = 0;
    acc->y = 0;
    return ;
}

void refreshPosition(Ball* ball) {
    M2d* vel = &ball->velocity,
       * pos = &ball->position;
    pos->x += vel->x;
    pos->y += vel->y;
}

void PhysX(World *world) {
    EnityChain *node = world->enityChain;
    for (int i = 0; node->next != NULL; i++) {
        Ball* ball = node->ball;
        //Phyx start
        boxBounce(ball,world->size);
        refreshVelocity(ball);
        refreshPosition(ball);
        ball->acceloration.y += 10;
        //Phyx end
        node = node->next;
    }
    return;
}

void Draw(World *world) {
    cleardevice();
    EnityChain* node = world->enityChain;
    for (int i = 0; node->next!=NULL; i++)
    {
        Ball* ball = node->ball;
        fillcircle(ball->position.x, ball->position.y, ball->size);
        node = node->next;
    }
}

int main()
{
    initgraph(CANVAS_WIDTH, CANVAS_HEIGHT);
    std::cout << "Hello World!\n";
    int ballCount = 5;
    World mainWorld{
        10,
        {1000,400},
        NULL
    };                                                           
    mainWorld.enityChain = (EnityChain*)malloc(sizeof(EnityChain));
    EnityChain* node = mainWorld.enityChain;
    for (int i = 0; i < ballCount; i++) {
        node->ball = (Ball*)malloc(sizeof(Ball));
        node->next = (EnityChain*)malloc(sizeof(EnityChain));
        node = node->next;
    }
    node->next = NULL;
    node = mainWorld.enityChain;
    for (int i = 0; i<ballCount; i++)
    {
        Ball* ball = node->ball;
        ball->position.x = 200 + rand() % (i + 10);
        ball->position.y = 200 + rand() % (i + 10);
        ball->velocity.x = rand() % (i + 10);
        ball->velocity.y = rand() % (i + 10);
        ball->acceloration.x = 0;
        ball->acceloration.y = 0;
        ball->size = 10;
        ball->springConstant = 1;
        node = node->next;
    }
    for (long frameCount=0;true;frameCount++)
    {
        Draw(&mainWorld);
        PhysX(&mainWorld);
        Sleep(16);
    }
    return 0;
}