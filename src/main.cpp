#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <map>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

using namespace std;

// Constantes
const int MAX_WIDTH = 20;
const int MAX_HEIGHT = 20;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Structs
struct Vector
{
    int X;
    int Y;

    Vector move(const Vector &moveVector)
    {
        return {X + moveVector.X, Y + moveVector.Y};
    };

    bool isOutOfLimit()
    {
        if (X < 0 || Y < 0 || X > (MAX_WIDTH + 1) || Y > (MAX_HEIGHT + 1))
        {
            return true;
        }
        return false;
    }

    bool collision(Vector vector){
        if (X==vector.X && Y==vector.Y)
        {
            return true;
        }
        return false;
    }
};

struct Snake
{
    Vector head;
    //Vector body[MAX_WIDTH * MAX_HEIGHT];
    Vector body[100];

    void addBodyPart(int bodyParts, Vector lastPos)
    {
        if (bodyParts == 0)
        {
            body[bodyParts].X = lastPos.X;
            body[bodyParts].Y = lastPos.Y;
        }

        body[bodyParts].X = body[bodyParts - 1].X;
        body[bodyParts].Y = body[bodyParts - 1].Y;
    }
};

// Mapa con los vectores de movimiento
map<Direction, Vector> movement = {
    {Direction::UP, {0, -1}},
    {Direction::DOWN, {0, 1}},
    {Direction::LEFT, {-1, 0}},
    {Direction::RIGHT, {1, 0}},
};

struct Node
{
    Vector direction;
    struct Node *next;
    struct Node *prev;

    Node(Vector dir)
    {
        direction = dir;
        next = nullptr;
        prev = nullptr;
    }
};
struct termios orig_termios;

// funciones
void printMatrix(string matrix[MAX_HEIGHT][MAX_WIDTH])
{
    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            cout << matrix[j][i];
        }
        cout << endl;
    }
}
// llenado de la matriz
void fillMatrix(string matrix[MAX_WIDTH][MAX_HEIGHT])
{
    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            matrix[j][i] = "  ";
        }
    }
}

Vector createRandomFood()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(1, MAX_WIDTH - 1);
    std::uniform_int_distribution<int> distribY(1, MAX_HEIGHT - 1);
    Vector food = {distribX(gen), distribY(gen)};
    return food;
}

void cleanConsole()
{
    // cout << "\033[2J\033[H" << flush;
    cout << "\x1b[2J\033[H" << flush;
}

// Captura caracteres
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); // Restaura la terminal automáticamente al salir

    struct termios raw = orig_termios;
    // Apagar modo canónico (ICANON) y eco (ECHO)
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    // Configuración clave para no bloquear el bucle:
    raw.c_cc[VMIN] = 0;  // Número mínimo de bytes para leer = 0
    raw.c_cc[VTIME] = 0; // Tiempo de espera en décimas de segundo (0 = retorna inmediato)

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// inicio programa
int main()
{
    enableRawMode();
    cout << "\033[?1049h\033[?25l";
    cleanConsole();

    // inicializacion dial direcciones
    
    Node *upDir = new Node(movement[Direction::UP]);
    Node *rightDir = new Node(movement[Direction::RIGHT]);
    Node *downDir = new Node(movement[Direction::DOWN]);
    Node *leftDir = new Node(movement[Direction::LEFT]);

    upDir->next = rightDir;
    rightDir->next = downDir;
    downDir->next = leftDir;
    leftDir->next = upDir;

    upDir->prev = leftDir;
    rightDir->prev = upDir;
    downDir->prev = rightDir;
    leftDir->prev = downDir;

    Node *actualDirection = upDir;

    string matrix[MAX_HEIGHT][MAX_WIDTH];
    Vector food = createRandomFood();
    //Vector food = {0, 0};

    Snake snake = {{MAX_HEIGHT / 2, MAX_WIDTH / 2}, {}};
    int bodyParts = 0;
    Vector headLastPos;
    while (true)
    {
        // lectura de caracteres
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            switch (c)
            {
            case 'q':
                break; // Salir con 'q'
            case 'j':
                actualDirection = actualDirection->prev;
                break;
            case 'k':
                actualDirection = actualDirection->next;
                break;
            }
        }

        //limpieza
        cleanConsole();
        fillMatrix(matrix);
        
        // control de dirección de la serpiente
        headLastPos = snake.head;
        snake.head = snake.head.move(actualDirection->direction);
        if (snake.head.isOutOfLimit()){
            cout << "GAME OVER" << endl;
            break;
        }
        
        // renderizado de la matriz
        matrix[food.Y][food.X] = "@ ";
        matrix[snake.head.X][snake.head.Y] = "O ";
        for (int i = 0; i < bodyParts + 1; i++)
        {
            matrix[headLastPos.X][headLastPos.Y]="o ";
        }
        
        if (snake.head.collision(food))
        {
            snake.addBodyPart(bodyParts,headLastPos);
            food= createRandomFood();
        }
        
        
        printMatrix(matrix);

        cout << snake.head.X << " " << snake.head.Y << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}