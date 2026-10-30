#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <map>

using namespace std;

// Constantes
const int MAX_WIDTH = 20;
const int MAX_HEIGHT = 20;

enum class Direction{
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
        if (X < 0 || Y < 0 || X > (MAX_WIDTH+1) || Y > (MAX_HEIGHT+1) )
        {
            return true;
        }
        return false;
    }
};

struct Snake
{
    Vector head;
    Vector body[MAX_WIDTH * MAX_HEIGHT];
};

// Mapa con los vectores de movimiento
map<Direction, Vector> movement = {
    {Direction::UP, {0, -1}},
    {Direction::DOWN, {0, 1}},
    {Direction::LEFT, {-1, 0}},
    {Direction::RIGHT, {1, 0}},
};

struct Node {
   Vector direction;
   struct Node *next;
   struct Node *prev;

   Node(Vector dir) {
        direction = dir;
        next = nullptr;
        prev = nullptr;
    }
};

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

// inicio programa
int main()
{

    cout << "\033[?1049h\033[?25l";
    cleanConsole();
    
    //inicializacion dial direcciones 
    Direction direction;
    Node* upDir = new Node(movement[Direction::UP]);
    Node* rightDir = new Node(movement[Direction::RIGHT]);
    Node* downDir = new Node(movement[Direction::DOWN]);
    Node* leftDir = new Node(movement[Direction::LEFT]);
    
    upDir->next = rightDir;
    rightDir->next = downDir;
    downDir->next = leftDir;
    leftDir->next = upDir;
    
    upDir->prev = leftDir;
    rightDir->prev = upDir;
    downDir->prev = rightDir;
    leftDir->prev = downDir;
    
    Node* actualDirection=upDir;

    string matrix[MAX_HEIGHT][MAX_WIDTH];
    // Vector food = createRandomFood();
    Vector food = {0, 0};

    Snake snake = {{MAX_HEIGHT / 2, MAX_WIDTH / 2}, {}};

    while (true)
    {
        cleanConsole();
        fillMatrix(matrix);
        
        snake.head = snake.head.move(actualDirection->direction);
        if (snake.head.isOutOfLimit()){
            cout << "GAME OVER"<< endl;
            break;
        }

        matrix[snake.head.X][snake.head.Y] = "O ";
        matrix[food.Y][food.X] = "@ "; // Orden correcto: Fila (Y), Columna (X)

        printMatrix(matrix);
        cout << snake.head.X << " " << snake.head.Y << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    /*
    while (true)
    {
        for (size_t i = 0; i < 4; i++)
        {
            cout << "." << flush;
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
        cleanConsole();
    }
    */

    return 0;
}