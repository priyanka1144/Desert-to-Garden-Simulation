#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// --- Global Variables (No change) ---
int trees = 0;
int maxTrees = 9;  // Only 9 plantable trees
bool showRain = false;
int rainTimer = 0;

int birdX1 = 50, birdX2 = 150, birdX3 = 250;
int birdY1 = 100, birdY2 = 120, birdY3 = 80;
int baseBirdY1 = 100, baseBirdY2 = 120, baseBirdY3 = 80;

// Fish variables updated for 5 small fish and 1 large fish
int fishX1 = -40, fishX2 = -20, fishX3 = 0, fishX4 = 20, fishX5 = 40;
int fishY1 = -10, fishY2 = 5, fishY3 = 0, fishY4 = -5, fishY5 = 8;
int baseFishY1 = -10, baseFishY2 = 5, baseFishY3 = 0, baseFishY4 = -5, baseFishY5 = 8;
int fishDirection1 = 1, fishDirection2 = -1, fishDirection3 = 1, fishDirection4 = -1, fishDirection5 = 1;

// Added variables for the new large fish (Fish 6)
int fishX6 = 60; 
int fishY6 = -20;
int baseFishY6 = -20;
int fishDirection6 = -1; 
int fishSpeed6 = 2; // Slightly faster

int sunRayAngle = 0;
int cloudX1 = 100, cloudX2 = 300, cloudX3 = 500;
int frameCount = 0;

int cloudSpeed = 1;
int birdSpeed1 = 2, birdSpeed2 = 1, birdSpeed3 = 3;
int fishSpeed = 1;

// --- Function Prototypes ---
void drawSky();
void drawClouds();
void drawSun();
void drawDesert();
void drawGrass();
void drawTree(int x, int y, int scale = 100);
void drawBigPond();
void drawColorfulFish();
void drawBirds();
void drawRain();
void drawApples(int x, int y, int scale = 100, int count = 4);
void drawMountains();
void drawStones();
void handleInput();
void updateAnimation();
void drawGradualTransition();

int main()
{
    int gd = DETECT, gm;
    char bgi_path[] = "C:\\TURBOC3\\BGI"; 
    initgraph(&gd, &gm, bgi_path);

    srand(time(NULL));

    int maxX = getmaxx(), maxY = getmaxy();
    void *buffer = malloc(imagesize(0, 0, maxX, maxY));

    while (1)
    {
        setactivepage(1);
        cleardevice();

        drawSky();
        drawClouds();
        drawSun();
        drawMountains(); // Color transition still depends on `trees`

        drawGradualTransition();

        int pond_cx = getmaxx()/2;
        int pond_cy = getmaxy()-80;

        // --- Tree Positions ---

        // 9 plantable trees (medium scale: 70, 4 apples each) - Lower/Middle Ground
        int plantableTreePositions[][2] = {
            {50, getmaxy() - 80}, {150, getmaxy() - 90}, {250, getmaxy() - 85}, 
            {390, getmaxy() - 75}, {490, getmaxy() - 80}, {590, getmaxy() - 90},
            {100, getmaxy() - 150}, {200, getmaxy() - 140}, 
            {540, getmaxy() - 150}
        };

        // 3 Permanent Trees near Mountains (medium scale: 70, 4 apples each) - ONLY APPEAR IF trees >= 5
        if (trees >= 5) {
            drawTree(20, getmaxy() - 200, 70);
            drawApples(20, getmaxy() - 200, 70, 4); 
            
            drawTree(600, getmaxy() - 210, 70);
            drawApples(600, getmaxy() - 210, 70, 4);

            drawTree(getmaxx()/2 - 10, getmaxy() - 220, 70);
            drawApples(getmaxx()/2 - 10, getmaxy() - 220, 70, 4);
        }

        if (trees >= 3)
        {
            drawBigPond();
            drawStones();
            drawColorfulFish();

            // 3 Baby Trees NEAR pond (Scale 40, 2 apples each) - ONLY APPEAR IF trees >= 3
            drawTree(pond_cx - 180, getmaxy() - 130, 40);
            drawApples(pond_cx - 180, getmaxy() - 130, 40, 2); 
            
            drawTree(pond_cx + 180, getmaxy() - 125, 40);
            drawApples(pond_cx + 180, getmaxy() - 125, 40, 2);

            drawTree(pond_cx, getmaxy() - 150, 40);
            drawApples(pond_cx, getmaxy() - 150, 40, 2);
        }

        // 9 plantable trees - Draw only the number planted up to maxTrees
        for (int i = 0; i < trees && i < maxTrees; i++)
        {
            int tx = plantableTreePositions[i][0];
            int ty = plantableTreePositions[i][1];
            drawTree(tx, ty, 70); // Medium size
            drawApples(tx, ty, 70, 4); // 4 apples
        }

        if (trees >= 5) drawBirds();

        if (trees >= maxTrees)
        {
            showRain = true;
            rainTimer++;
            if (rainTimer > 500) { showRain = false; rainTimer = 0; }
        }
        if (showRain) drawRain();

        // --- Info Text (No change) ---
        setcolor(WHITE);
        settextstyle(SMALL_FONT, HORIZ_DIR, 1);
        outtextxy(10,10,(char*)"Press 'P' to plant trees");
        outtextxy(10,25,(char*)"Press ESC to exit");
        outtextxy(10,40,(char*)"Pond & Baby Trees appear at 3 trees!");
        outtextxy(10,55,(char*)"Mountain Trees & Birds appear at 5 trees!");
        outtextxy(10,70,(char*)"Rain celebration at 9 trees!");

        char treeCount[50];
        sprintf(treeCount,"Trees: %d/%d",trees,maxTrees);
        outtextxy(10,85,treeCount);

        // --- Double Buffering (No change) ---
        getimage(0,0,maxX,maxY,buffer);
        setactivepage(0);
        putimage(0,0,buffer,COPY_PUT);

        handleInput();
        updateAnimation();

        delay(50);
        frameCount++;
    }

    free(buffer);
    closegraph();
    return 0;
}

/* ----------------------- DRAW FUNCTIONS ----------------------- */

// Custom COLOR function for deep colors
#define DEEP_YELLOW COLOR(255, 191, 0)
#define DEEP_RED    COLOR(179, 0, 0)
#define DEEP_GREEN  COLOR(0, 102, 0)
#define DEEP_MAGENTA COLOR(153, 0, 153)
#define DEEP_CYAN   COLOR(0, 128, 128)
#define DEEP_BLUE   COLOR(0, 0, 153)

void drawSky()
{
    for (int y = 0; y < 200; y++)
        setcolor(COLOR(135,206-y/2,235-y/4)), line(0,y,getmaxx(),y);
}

void drawClouds()
{
    setcolor(WHITE); setfillstyle(SOLID_FILL,WHITE);
    fillellipse(cloudX1,60,40,25);  fillellipse(cloudX1+30,55,45,30);
    fillellipse(cloudX1+65,60,40,25); fillellipse(cloudX1+35,40,30,20);
    fillellipse(cloudX1+15,75,25,15); fillellipse(cloudX1+50,75,25,15);

    fillellipse(cloudX2,100,35,20);  fillellipse(cloudX2+25,95,40,25);
    fillellipse(cloudX2+55,100,35,20);fillellipse(cloudX2+30,80,25,15);
    fillellipse(cloudX2+10,115,20,12);

    fillellipse(cloudX3,40,50,30);   fillellipse(cloudX3+40,35,55,35);
    fillellipse(cloudX3+85,40,50,30);fillellipse(cloudX3+45,15,35,20);
    fillellipse(cloudX3+20,60,30,18);fillellipse(cloudX3+65,60,30,18);

    setcolor(LIGHTGRAY); setfillstyle(SOLID_FILL,LIGHTGRAY);
    fillellipse(cloudX1+5,65,40,25);
    fillellipse(cloudX2+5,105,35,20);
    fillellipse(cloudX3+5,45,50,30);
}

void drawSun()
{
    int cx=550,cy=80;
    for (int r=40;r>=0;r--)
        setcolor(COLOR(255,255-r*2,0)), setfillstyle(SOLID_FILL,COLOR(255,255-r*2,0)),
        fillellipse(cx,cy,r,r);

    setcolor(YELLOW);
    for (int i=0;i<16;i++)
    {
        float a = sunRayAngle + i*22.5;
        int rx = cx + (int)(60*cos(a*3.14159/180));
        int ry = cy + (int)(60*sin(a*3.14159/180));
        line(cx,cy,rx,ry);
    }
}

/* Mountain: Color transition remains */
void drawMountains()
{
    int startR = 100, startG = 100, startB = 100;
    int endR   = 144, endG   = 238, endB   = 144; 

    float prog = (float)trees / (maxTrees + 2);

    int fillR = startR + (int)((endR - startR) * prog);
    int fillG = startG + (int)((endG - startG) * prog);
    int fillB = startB + (int)((endB - startB) * prog);

    int lineR = fillR + 50; if (lineR > 255) lineR = 255;
    int lineG = fillG + 50; if (lineG > 255) lineG = 255;
    int lineB = fillB + 50; if (lineB > 255) lineB = 255;

    setcolor(COLOR(fillR, fillG, fillB));
    setfillstyle(SOLID_FILL, COLOR(fillR, fillG, fillB));

    int m1[]={0,200,100,120,200,200,0,200}; fillpoly(4,m1);
    int m2[]={150,200,280,100,400,200,150,200}; fillpoly(4,m2);
    int m3[]={350,200,500,80,640,200,350,200}; fillpoly(4,m3);

    setcolor(COLOR(lineR, lineG, lineB));
    line(100,120,150,160);
    line(280,100,320,150);
    line(500,80,540,140);
}

void drawGradualTransition()
{
    drawDesert();
    if (trees < 3) return;
    if (trees < 7)
    {
        int h = (trees-2)*40;
        if (h>getmaxy()-200) h=getmaxy()-200;
        setcolor(GREEN); setfillstyle(SOLID_FILL,GREEN);
        bar(0,getmaxy()-h,getmaxx(),getmaxy());

        setcolor(LIGHTGREEN);
        for (int i=0;i<trees*6;i++)
        {
            int x=(i*47)%getmaxx();
            int y=getmaxy()-h + (i*23)%h;
            if (y>=200) { line(x,y,x+2,y+5); line(x,y,x-2,y+5); }
        }
    }
    else drawGrass();
}

void drawDesert()
{
    for (int y=200;y<getmaxy();y++)
        setcolor(COLOR(255,255-(y-200)/4,100)), line(0,y,getmaxx(),y);

    setfillstyle(SOLID_FILL,YELLOW);
    fillellipse(80,330,70,25); fillellipse(220,345,80,30);
    fillellipse(380,335,75,28); fillellipse(520,350,85,32);
    setcolor(LIGHTGRAY); ellipse(80,330,0,180,70,25);
}

void drawGrass()
{
    setcolor(GREEN); setfillstyle(SOLID_FILL,GREEN);
    bar(0,200,getmaxx(),getmaxy());

    setcolor(LIGHTGREEN);
    for (int i=0;i<60;i++)
    {
        int x=(i*47)%getmaxx();
        int y=210 + (i*31)%(getmaxy()-210);
        line(x,y,x+3,y+6); line(x,y,x-3,y+6);
    }
}

void drawTree(int x, int y, int scale)
{
    float s = scale/100.0;
    int trunkW = 7*s, trunkH = 45*s;

    setcolor(BROWN); setfillstyle(SOLID_FILL,BROWN);
    bar(x-trunkW,y,x+trunkW,y+trunkH);

    setcolor(LIGHTGREEN); setfillstyle(SOLID_FILL,LIGHTGREEN);
    fillellipse(x,y-10*s,32*s,28*s);
    fillellipse(x-14*s,y+3*s,21*s,18*s);
    fillellipse(x+14*s,y+3*s,21*s,18*s);
    fillellipse(x,y+10*s,18*s,14*s);

    setcolor(WHITE);
    for (int i=0;i<8;i++)
        putpixel(x-21*s+i*5*s, y-25*s+(i%3)*8*s, WHITE);

    setcolor(DARKGRAY); setfillstyle(SOLID_FILL,DARKGRAY);
    fillellipse(x+7*s,y+trunkH,21*s,7*s);
}

void drawApples(int x, int y, int scale, int count)
{
    float s = scale/100.0;
    setcolor(RED); setfillstyle(SOLID_FILL,RED);
    
    // Apple 1 (Top-Left)
    if (count >= 1) {
        fillellipse(x-18*s,y-14*s,4*s,4*s);
        setcolor(WHITE); putpixel(x-18*s+1,y-14*s-1,WHITE);
        setcolor(BROWN); line(x-18*s,y-18*s,x-18*s,y-14*s);
    }
    // Apple 2 (Top-Right)
    if (count >= 2) {
        fillellipse(x+15*s,y-8*s,4*s,4*s);
        setcolor(WHITE); putpixel(x+15*s+1,y-8*s-1,WHITE);
        setcolor(BROWN); line(x+15*s,y-12*s,x+15*s,y-8*s);
    }
    // Apple 3 (Highest)
    if (count >= 3) {
        fillellipse(x-7*s ,y-25*s,4*s,4*s);
        setcolor(WHITE); putpixel(x-7*s +1,y-25*s-1,WHITE);
        setcolor(BROWN); line(x-7*s ,y-29*s,x-7*s ,y-25*s);
    }
    // Apple 4 (Lower-Right)
    if (count >= 4) {
        fillellipse(x+8*s ,y+3*s ,4*s,4*s);
        setcolor(WHITE); putpixel(x+8*s +1,y+3*s -1,WHITE);
        setcolor(BROWN); line(x+8*s ,y-1*s ,x+8*s ,y+3*s);
    }
}

void drawBigPond()
{
    int cx = getmaxx()/2, cy = getmaxy()-80;
    for (int r=120; r>=80; r-=10)
    {
        setcolor(COLOR(0,100+(120-r)/2,200+(120-r)/4));
        setfillstyle(SOLID_FILL,COLOR(0,100+(120-r)/2,200+(120-r)/4));
        fillellipse(cx,cy,r,r/2);
    }
    setcolor(WHITE);
    for (int i=0;i<3;i++)
        ellipse(cx,cy,0,360,95-i*10,38-i*4);
}

void drawStones()
{
    int cx = getmaxx()/2, cy = getmaxy()-80;
    setcolor(DARKGRAY); setfillstyle(SOLID_FILL,LIGHTGRAY);
    fillellipse(cx-80,cy-20,12,8); fillellipse(cx+75,cy-15,10,7);
    fillellipse(cx-85,cy+25,14,9); fillellipse(cx+80,cy+30,11,8);
    fillellipse(cx   ,cy-45,9,6);  fillellipse(cx-30,cy+45,13,8);
    fillellipse(cx+35,cy+42,12,7);
}

void drawColorfulFish()
{
    int cx = getmaxx()/2, cy = getmaxy()-80;

    // Fish 1 - Deep Yellow
    setcolor(DEEP_YELLOW); setfillstyle(SOLID_FILL,DEEP_YELLOW);
    fillellipse(cx+fishX1,cy+fishY1,12,7);
    if (fishDirection1>0){
        int t[]={cx+fishX1-12,cy+fishY1,cx+fishX1-18,cy+fishY1-5,cx+fishX1-18,cy+fishY1+5};
        fillpoly(3,t); line(cx+fishX1+8,cy+fishY1,cx+fishX1+12,cy+fishY1-3);
    }else{
        int t[]={cx+fishX1+12,cy+fishY1,cx+fishX1+18,cy+fishY1-5,cx+fishX1+18,cy+fishY1+5};
        fillpoly(3,t); line(cx+fishX1-8,cy+fishY1,cx+fishX1-12,cy+fishY1-3);
    }
    setcolor(BLACK); putpixel(cx+fishX1+(fishDirection1>0?3:-3),cy+fishY1-2,BLACK);

    // Fish 2 - Deep Red
    setcolor(DEEP_RED); setfillstyle(SOLID_FILL,DEEP_RED);
    fillellipse(cx+fishX2,cy+fishY2,10,6);
    if (fishDirection2>0){
        int t[]={cx+fishX2-10,cy+fishY2,cx+fishX2-15,cy+fishY2-4,cx+fishX2-15,cy+fishY2+4};
        fillpoly(3,t); line(cx+fishX2+6,cy+fishY2,cx+fishX2+10,cy+fishY2-2);
    }else{
        int t[]={cx+fishX2+10,cy+fishY2,cx+fishX2+15,cy+fishY2-4,cx+fishX2+15,cy+fishY2+4};
        fillpoly(3,t); line(cx+fishX2-6,cy+fishY2,cx+fishX2-10,cy+fishY2-2);
    }
    setcolor(BLACK); putpixel(cx+fishX2+(fishDirection2>0?3:-3),cy+fishY2-1,BLACK);

    // Fish 3 - Deep Green
    setcolor(DEEP_GREEN); setfillstyle(SOLID_FILL,DEEP_GREEN);
    fillellipse(cx+fishX3,cy+fishY3,11,6);
    if (fishDirection3>0){
        int t[]={cx+fishX3-11,cy+fishY3,cx+fishX3-16,cy+fishY3-4,cx+fishX3-16,cy+fishY3+4};
        fillpoly(3,t);
    }else{
        int t[]={cx+fishX3+11,cy+fishY3,cx+fishX3+16,cy+fishY3-4,cx+fishX3+16,cy+fishY3+4};
        fillpoly(3,t);
    }

    // Fish 4 - Deep Magenta
    setcolor(DEEP_MAGENTA); setfillstyle(SOLID_FILL,DEEP_MAGENTA);
    fillellipse(cx+fishX4,cy+fishY4,9,5);
    if (fishDirection4>0){
        int t[]={cx+fishX4-9,cy+fishY4,cx+fishX4-14,cy+fishY4-3,cx+fishX4-14,cy+fishY4+3};
        fillpoly(3,t);
    }else{
        int t[]={cx+fishX4+9,cy+fishY4,cx+fishX4+14,cy+fishY4-3,cx+fishX4+14,cy+fishY4+3};
        fillpoly(3,t);
    }

    // Fish 5 - Deep Cyan
    setcolor(DEEP_CYAN); setfillstyle(SOLID_FILL,DEEP_CYAN);
    fillellipse(cx+fishX5,cy+fishY5,10,5);
    if (fishDirection5>0){
        int t[]={cx+fishX5-10,cy+fishY5,cx+fishX5-15,cy+fishY5-3,cx+fishX5-15,cy+fishY5+3};
        fillpoly(3,t);
    }else{
        int t[]={cx+fishX5+10,cy+fishY5,cx+fishX5+15,cy+fishY5-3,cx+fishX5+15,cy+fishY5+3};
        fillpoly(3,t);
    }
    
    // Fish 6 - **Large Fish** - Deep Blue
    setcolor(DEEP_BLUE); setfillstyle(SOLID_FILL,DEEP_BLUE);
    fillellipse(cx+fishX6,cy+fishY6,20,12); // Larger size
    if (fishDirection6>0){
        int t[]={cx+fishX6-20,cy+fishY6,cx+fishX6-30,cy+fishY6-8,cx+fishX6-30,cy+fishY6+8};
        fillpoly(3,t); line(cx+fishX6+14,cy+fishY6,cx+fishX6+20,cy+fishY6-6);
    }else{
        int t[]={cx+fishX6+20,cy+fishY6,cx+fishX6+30,cy+fishY6-8,cx+fishX6+30,cy+fishY6+8};
        fillpoly(3,t); line(cx+fishX6-14,cy+fishY6,cx+fishX6-20,cy+fishY6-6);
    }
    setcolor(WHITE); putpixel(cx+fishX6+(fishDirection6>0?6:-6),cy+fishY6-4,WHITE);
}

void drawBirds()
{
    setcolor(BLACK);
    int w1 = (frameCount%20<10)?-2:2;
    arc(birdX1,birdY1+w1,20,160,14); arc(birdX1+15,birdY1+w1,20,160,14);
    setfillstyle(SOLID_FILL,LIGHTGRAY); fillellipse(birdX1+7,birdY1,4,3);

    int w2 = (frameCount%25<12)?-2:2;
    arc(birdX2,birdY2+w2,25,155,12); arc(birdX2+12,birdY2+w2,25,155,12);
    setfillstyle(SOLID_FILL,LIGHTGRAY); fillellipse(birdX2+6,birdY2,3,2);

    int w3 = (frameCount%18<9)?-2:2;
    arc(birdX3,birdY3+w3,15,165,16); arc(birdX3+18,birdY3+w3,15,165,16);
    setfillstyle(SOLID_FILL,LIGHTGRAY); fillellipse(birdX3+9,birdY3,5,3);
}

void drawRain()
{
    setcolor(LIGHTCYAN);
    for (int i=0;i<40;i++)
    {
        int rx = (i*25 + frameCount*3)%(getmaxx()+40)-20;
        int ry = ((frameCount*4)+i*35)%180 + 30;
        line(rx,ry,rx+2,ry+10);
    }
    setcolor(WHITE); settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
    outtextxy(getmaxx()/2-140,getmaxy()/2-30,(char*)"GARDEN COMPLETE!");
    outtextxy(getmaxx()/2-120,getmaxy()/2+10,(char*)"CELEBRATION!");

    setcolor(YELLOW);
    for (int i=0;i<8;i++)
    {
        int sx = 100+i*80 + (frameCount%10);
        int sy = 150+(i%4)*60;
        fillellipse(sx,sy,3,3);
        line(sx,sy-5,sx,sy+5); line(sx-5,sy,sx+5,sy);
    }
}

void handleInput()
{
    if (kbhit())
    {
        char k = getch();
        // Only the 9 plantable trees are increased by 'P'
        if (k=='p' || k=='P') if (trees<maxTrees) trees++;
        if (k==27) { closegraph(); exit(0); }
    }
}

void updateAnimation()
{
    cloudX1+=cloudSpeed; cloudX2+=cloudSpeed; cloudX3+=cloudSpeed;
    if (cloudX1>getmaxx()+150) cloudX1=-150;
    if (cloudX2>getmaxx()+150) cloudX2=-150;
    if (cloudX3>getmaxx()+150) cloudX3=-150;

    if (trees>=5)
    {
        birdX1+=birdSpeed1; birdX2+=birdSpeed2; birdX3+=birdSpeed3;
        birdY1=baseBirdY1+((frameCount/10)%3)-1;
        birdY2=baseBirdY2+((frameCount/12)%3)-1;
        birdY3=baseBirdY3+((frameCount/8)%3)-1;
        if (birdX1>getmaxx()) birdX1=-50;
        if (birdX2>getmaxx()) birdX2=-50;
        if (birdX3>getmaxx()) birdX3=-50;
    }

    if (trees>=3)
    {
        fishX1+=fishDirection1*fishSpeed;
        fishX2+=fishDirection2*fishSpeed;
        fishX3+=fishDirection3*fishSpeed;
        fishX4+=fishDirection4*fishSpeed;
        fishX5+=fishDirection5*fishSpeed;
        
        fishX6+=fishDirection6*fishSpeed6; // Large fish speed

        if (fishX1>70 || fishX1<-70) fishDirection1*=-1;
        if (fishX2>70 || fishX2<-70) fishDirection2*=-1;
        if (fishX3>70 || fishX3<-70) fishDirection3*=-1;
        if (fishX4>70 || fishX4<-70) fishDirection4*=-1;
        if (fishX5>70 || fishX5<-70) fishDirection5*=-1;
        if (fishX6>70 || fishX6<-70) fishDirection6*=-1; // Large fish boundary

        fishY1=baseFishY1+((frameCount/15)%3)-1;
        fishY2=baseFishY2+((frameCount/18)%3)-1;
        fishY3=baseFishY3+((frameCount/12)%3)-1;
        fishY4=baseFishY4+((frameCount/20)%3)-1;
        fishY5=baseFishY5+((frameCount/16)%3)-1;
        fishY6=baseFishY6+((frameCount/10)%3)-1; // Large fish bobbing
    }

    sunRayAngle = (sunRayAngle+1)%360;
}