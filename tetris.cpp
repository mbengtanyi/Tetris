#include <SFML/Graphics.hpp>
// FYI location of SFML is /usr/local/Cellar/sfml/2.5.1_1
#include <time.h>
using namespace sf;


const int M = 20;
const int N = 20;

int field[M][N] = {0};

struct Point
{int x,y;} a[4], b[4];

//States of the tiles
// __________________
// |  0     |    1  |
// |  2     |    3  |
// |  4     |    5  |
// |  6     |    7  |
// ------------------

int figures[7][4] = {
    1,3,5,7, //I
    2,4,5,7, //Z
    3,5,4,6, //S
    3,5,4,7, //T
    2,3,5,7, //L
    3,5,7,6, //J
    2,3,4,5, //O
};

bool check(){
    for (int i=0; i<4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {return 0;}
        if (field[a[i].y][a[i].x]) {return 0;}
    }
    return 1;
};

//main code begins here
int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "Tetris!");

    Texture t1, t2;
    //loading the tiles and background from the images folder
    t1.loadFromFile("Tetris Images/tiles.png");
    t2.loadFromFile("Tetris Images/background.png");

    Sprite s(t1), background(t2);
    s.setTextureRect(IntRect(0, 0, 18, 18));

    int dx = 0; //x direction of tiles
    bool rotate = 0; 
    int colorNum = 1;

    float timer = 0;
    float delay = 0.3;

    Clock clock;

    {
        const int firstFigure = rand() % 7;
        for (int i = 0; i < 4; i++)
        {

            a[i].x = figures[firstFigure][i] % 2;
            a[i].y = figures[firstFigure][i] / 2;
        }
    }

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up) rotate=true;  //change shape of tile if up key is pressed
                else if (e.key.code == Keyboard::Left) dx = -1;  //move tile 1px to the left if left arrow key is pressed
                else if (e.key.code == Keyboard::Right) dx = 1; //move tile 1px to the right if right arrow key is pressed
        }
    

    if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

    // Implementing movement
    for (int i=0; i<4; i++) {
        b[i] = a[i];
        a[i].x += dx;
        }
    if (!check()) for (int i=0; i<4; i++) a[i] = b[i];

    // Rotate

    if (rotate){
        Point p = a[1]; //center of rotation
        for (int i=0; i<4; i++){
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }

        if (!check()) 
        for (int i=0; i<4; i++) a[i] = b[i];
    }


    //Implementing ticking of the timer

    if (timer > delay){
        for (int i=0; i<4; i++) {
            b[i] = a[i];
            a[i].y += 1;
            }

        if (!check()){
            for (int i=0; i<4; i++){
                field[b[i].y][b[i].x] = colorNum;
            }

            colorNum = 1 + rand()%7;
            int n = rand()%7;
            for (int i=0; i<4; i++){
                a[i].x = figures[n][i] % 2;
                a[i].y = figures[n][i] / 2;
            }
            
            if(!check()){
                window.close();
                break;
            }
        }

        timer=0;
    }
    

    int n=3;

    if (a[0].x==0)
    for (int i=0; i<4; i++){
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }


    //Check lines?
    int k = M - 1;
    for (int i=M-1; i>0; i--){
        int count = 0;
        for (int j=0; j<N; j++){
            if (field[i][j]){
                count++;
                }
            field[k][j] = field[i][j];
        }
        if(count <N ){ 
            k--;
            }
        else{
            //increase score
            // ++score;
        }
    }
    if (k != 0)
    {
        std::fill(field[0], field[0] + (N * (k + 1)), 0);
    }



    dx = 0;
    rotate = 0;
    delay = 0.3;

    //Implementing the actual drawing

    window.clear(Color::Red);  //background color
    window.draw(background);

    for (int i=0; i<M; i++)
        for(int j=0; j<N; j++){
            if (field[i][j] == 0) continue;
            s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
            s.setPosition(j*18, i*18);
            s.move(33,31); 
            //offset

            window.draw(s);
        }


    for (int i = 0; i < 4; i++)
    {
        s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
        s.setPosition(a[i].x*18, a[i].y*18);
        s.move(33,31); //offset

        window.draw(s);
    }
    

    window.draw(s); //draws tiles
    window.display();
    }
    
    return 0;
}