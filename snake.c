#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

enum content
{
    Border = '#',
    Snake_head = '@',
    Snake_tail = '8',
    Food = '*',
    Empty = ' ',
};
struct entity
{
    enum content content;
    int dx; // geschwindigkeit in x-Richtung
    int dy; // geschwindigkeit in y-Richtung
    bool updated;
};
struct field
{
    struct entity **arr; // speicher für Feld
    int width;           // Breite
    int height;          // Höhe
    bool lost;           //=1 wenn verloren
};
struct field newField(int x, int y)
{
    struct field ret = {
        .arr = (struct entity **)calloc(x, sizeof(struct entity *)),
        .width = x,
        .height = y,
        .lost = false,
    };
    for (int i = 0; i < ret.width; i++)
    {
        ret.arr[i] = (struct entity *)calloc(ret.height, sizeof(struct entity));
    }
    return ret;
}
bool isInField(struct field *field, int x, int y)
{
    if (!(0 < x && x < field->height - 1))
    {
        return 0;
    }
    if (!(0 < y && y < field->width - 1))
    {
        return 0;
    }
    return 1;
}
void setDxDy(struct field *field, int dx, int dy)
{
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            switch (field->arr[x][y].content)
            {
            case Snake_tail:
            case Snake_head:
                field->arr[x][y].dx = dx;
                field->arr[x][y].dy = dy;
                break;
            default:
                break;
            }
        }
    }
}
void getMove(struct field *field) // Read Input
{
    char input;
    input = getchar();
    switch (input)
    {
    case 'w':
        setDxDy(field, 0, -1);
        break;
    case 's':
        setDxDy(field, 0, 1);
        break;
    case 'a':
        setDxDy(field, -1, 0);
        break;
    case 'd':
        setDxDy(field, 1, 0);
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
            printf("%c", field->arr[x][y].content);
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
            field->arr[x][y].content = Empty;
            field->arr[x][y].dx = 0;
            field->arr[x][y].dy = 0;
        }
    }
}
void createBorder(struct field *field) // Malt den Rand
{
    for (int x = 0; x < field->width; x++)
    {
        field->arr[x][0].content = Border;
        field->arr[x][field->height - 1].content = Border;
    }
    for (int y = 0; y < field->height; y++)
    {
        field->arr[0][y].content = Border;
        field->arr[field->width - 1][y].content = Border;
    }
}
void createFood(struct field *field) // erschafft essen
{
    field->arr[7][7].content = Food;
}
void moveAll(struct field *field)
{
    for (int y = 1; y < field->height - 1; y++)
    {
        for (int x = 1; x < field->width - 1; x++)
        {
            int
                x_next = x + field->arr[x][y].dx,
                y_next = y + field->arr[x][y].dy;
            if (x == x_next && y == y_next) // Wenn die Position sich nicht ändert.
            {
                continue;
            }
            if (field->arr[x_next][y_next].updated) // Wenn das Feld schon geupdated wurde
            {
                continue;
            }
            if (!isInField(field, x_next, y_next)) // Falls außer Spielfeld bricht ab
            {
                field->lost = 1;
                break;
            }
            switch (field->arr[x_next][y_next].content)
            {
            case Food:
            case Empty:
                field->arr[x_next][y_next].content = field->arr[x][y].content;
                field->arr[x][y].content = Empty;
                field->arr[x_next][y_next].updated = true;
                break;
            default:
                field->lost = 1;
                break;
            }
        }
    }
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            field->arr[x][y].updated = false;
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

    spielfeld.arr[5][5].content = Snake_head;
    spielfeld.arr[5][5].dx = 1;

    while (!spielfeld.lost)
    {
        if (kbhit())
        {
            getMove(&spielfeld);
        }

        moveAll(&spielfeld);

        printField(&spielfeld);
        usleep(500 * 1000); // 50 ms delay
    }

    return 0;
}
