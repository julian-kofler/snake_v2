#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>


enum content
{
    Border = '#',
    Snake_head = '@',
    Snake_tail = '8',
    Food = '*',
    Empty = ' ',
};
struct field
{
    int **arr;  // speicher für Feld
    int width;  // Breite
    int height; // Höhe
    int dx;     // Schlange geschwindigkeit in x-Richtung
    int dy;     // Schlange geschwindigkeit in y-Richtung
    bool lost;  //=1 wenn verloren
};
struct field newField(int x, int y)
{
    struct field ret = {
        .arr = (int **) calloc(x, sizeof(int *)),
        .width = x,
        .height = y,
        .dx = 1,
        .dy = 0,
        .lost = false,
    };
    for (int i = 0; i < ret.width; i++)
    {
        ret.arr[i] = (int *)calloc(ret.height, sizeof(int));
    }
    return ret;
}

enum move
{
    Up,
    Down,
    Left,
    Right,
};
enum move getMove(void) // Read Input
{
    char input = 0; //getInput();

    switch (input)
    {
    case 'w':
        return Up;
        break;
    case 's':
        return Down;
        break;
    case 'a':
        return Left;
        break;
    case 'd':
        return Right;
        break;
    default:
        printf("\a");//Piepst
        return getMove();
        break;
    }
}

void moveSnake(struct field *field) // bewegt die Schlange
{

}
void printField(struct field *field) // Zeichnet das spielfeld
{
    system("clear");
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            printf("%c", field->arr[x][y]);
        }
        printf("\n");
    }
}
void clearField(struct field *field) // clears Field without Borders
{
    for (int y = 1; y < field->height - 1; y++)
    {
        for (int x = 1; x < field->width - 1; x++)
        {
            field->arr[x][y] = Empty;
        }
    }
}
void createBorder(struct field *field) // Malt den Rand
{
    for (int x = 0; x < field->width; x++)
    {
        field->arr[x][0] = Border;
        field->arr[x][field->height - 1] = Border;
    }
    for (int y = 0; y < field->height; y++)
    {
        field->arr[0][y] = Border;
        field->arr[field->width - 1][y] = Border;
    }
}
void createFood(struct field *field) // erschafft essen
{

}

int main(int argc, char const *argv[])
{
    struct field spielfeld = newField(80, 30);
    createBorder(&spielfeld);
    createFood(&spielfeld);
    printField(&spielfeld);

    while (!spielfeld.lost)
    {

        printField(&spielfeld);
        usleep(100*1000);      // 500 ms delay
    }

    return 0;
}
