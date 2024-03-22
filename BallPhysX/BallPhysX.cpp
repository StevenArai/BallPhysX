#include <iostream>
#include <graphics.h>
#include <time.h>

#define CANVAS_WIDTH 1280
#define CANVAS_HEIGHT 720

typedef struct M2d{
    int x;
    int y;
};
typedef struct Ball{
    int r=10;
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

void* createNewBall(EnityChain **node, Ball ball) {
    if ((*node) == NULL) {
        EnityChain* newNode = (EnityChain*)malloc(sizeof(EnityChain));
        Ball* newBall = (Ball*)malloc(sizeof(Ball));
        *node = newNode;
        *newBall = ball;
        (*newNode).ball = newBall;
        (*newNode).next = NULL;
        return newNode;

    }
    else {
        return createNewBall(&((**node).next), ball);
    }
}
void boxBounce(Ball* ball, M2d boxSize) {
    int* x = &(*ball).position.x, * y = &(*ball).position.y,
        * accx = &(*ball).acceloration.x, * accy = &(*ball).acceloration.y;
    if (*x < 0)*accx += -*x / (*ball).springConstant;
    else if (*x > boxSize.x)*accx += (boxSize.x - *x) / (*ball).springConstant;
    if (*y < 0)*accy += -*y / (*ball).springConstant;
    else if (*y > boxSize.y)*accy += (boxSize.y - *y)/(*ball).springConstant;
}
void refreshVelocity(Ball* ball) {
    M2d *acc = &(*ball).acceloration,
        *vel = &(*ball).velocity;
    (*vel).x += (*acc).x;
    (*vel).y += (*acc).y;
    (*acc).x = 0;
    (*acc).y = 0;
    return ;
}
void refreshPosition(Ball* ball) {
    M2d* vel = &(*ball).velocity,
       * pos = &(*ball).position;
    (*pos).x += (*vel).x;
    (*pos).y += (*vel).y;
}
void PhysX(World *world) {
    EnityChain *node = (*world).enityChain;
    for (int i = 0; node != NULL; i++) {
        Ball* ball = (*node).ball;
        //Phyx start
        boxBounce(ball,(*world).size);
        refreshVelocity(ball);
        refreshPosition(ball);
        (*ball).acceloration.y += (*world).gravity;
        //Phyx end
        node = (*node).next;
    }
    return;
}
void Draw(World *world) {
    cleardevice();
    EnityChain* node = (*world).enityChain;
    for (int i = 0; node != NULL; i++)
    {
        Ball* ball = (*node).ball;
        fillcircle((*ball).position.x, (*ball).position.y, (*ball).r);
        node = (*node).next;
    }
    return;
}

int main()
{
    initgraph(CANVAS_WIDTH, CANVAS_HEIGHT);
    std::cout << "Hello World!\n";
    int ballCount = 5;
    World mainWorld{
        2,
        {1000,400},
        NULL
    };
    EnityChain** node = &(mainWorld.enityChain);
    for (int i = 0; i < ballCount; i++) {
        Ball ball = {
            10,
            1,
            { 200 + rand() % (i + 10),200 + rand() % (i + 10) },
            { rand() % (i + 10), rand() % (i + 10)},
            { 0, 0 },
        };
        createNewBall(node, ball);
    }
    for (long frameCount=0;true;frameCount++)
    {
        Draw(&mainWorld);
        PhysX(&mainWorld);
        Sleep(16);
    }
    return 0;
}