#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

//constantes
const int MAX_WIDTH=20;
const int MAX_HEIGHT=20;

struct Vector{
    int X;
    int Y;
};

//funciones 
void printMatrix(string matrix[MAX_HEIGHT][MAX_WIDTH]){
      for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            cout << matrix[i][j];  
        }
        cout << endl;
    }
}
 //llenado de la matriz
void fillMatrix(string matrix[MAX_WIDTH][MAX_HEIGHT]){
    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            matrix[i][j]="  ";  
        }        
    }
}

Vector createRandomFood(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(1, MAX_WIDTH-1);
    std::uniform_int_distribution<int> distribY(1, MAX_HEIGHT-1);
    Vector food = {distribX(gen),distribY(gen)};
    return food;
}

void cleanConsole(){
    //cout << "\033[2J\033[H" << flush;
    cout << "\x1b[2J\033[H" << flush;
}

//inicio programa
int main(){
    cout << "\033[?1049h\033[?25l";
    cleanConsole();
    
    string matrix[MAX_HEIGHT][MAX_WIDTH];
    Vector food = createRandomFood(); 
    //cout << food.X << ";" << food.Y << endl ;

   while (true){
        cleanConsole();
        fillMatrix(matrix);
        matrix[food.Y][food.X] = "@"; // Orden correcto: Fila (Y), Columna (X)
        printMatrix(matrix);
        this_thread::sleep_for(chrono::milliseconds(100));
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