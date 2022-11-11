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
        .arr = (int **)calloc(x, sizeof(int *)),
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

void getMove(struct field *field) // Read Input
{
    char input;
    scanf(" %c", &input);
    switch (input)
    {
    case 'w':
        field->dx = 0;
        field->dy = -1;
        break;
    case 's':
        field->dx = 0;
        field->dy = 1;
        break;
    case 'a':
        field->dx = -1;
        field->dy = 0;
        break;
    case 'd':
        field->dx = 1;
        field->dy = 0;
        break;
    default:
        printf("\a"); // Piepst
        return getMove(field);
        break;
    }
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
    field->arr[7][7] = Food;
}
void moveSnake(struct field *field)
{
    int Xnext, Ynext;
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            if (field->arr[x][y] == Snake_head)
            {
                Xnext = x + field->dx;
                Ynext = y + field->dy;

                switch (field->arr[Xnext][Ynext])
                {
                case Empty:
                    field->arr[Xnext][Ynext] = Snake_head;
                    field->arr[x][y] = Empty;
                    break;
                case Food:
                    field->arr[Xnext][Ynext] = Snake_head;
                    field->arr[x][y] = Snake_tail;
                    break;
                default:
                    field->lost = true;
                    break;
                }
            }
        }
    }
}
int main(int argc, char const *argv[])
{
    struct field spielfeld = newField(80, 15);
    clearField(&spielfeld);
    createBorder(&spielfeld);
    createFood(&spielfeld);
    printField(&spielfeld);

    spielfeld.arr[10][10] = Snake_head;

    while (!spielfeld.lost)
    {
        getMove(&spielfeld);
        moveSnake(&spielfeld);

        printField(&spielfeld);
        usleep(500 * 1000); // 50 ms delay
    }

    return 0;
}
