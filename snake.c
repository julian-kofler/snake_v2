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
    int n; // nummer des Schwanzstückes
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
    if (!(0 < x && x < field->width - 1))
    {
        return 0;
    }
    if (!(0 < y && y < field->height - 1))
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
                break;
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
    char *ausgabe = (char *)calloc(
        field->height * field->width + field->height + 1,
        sizeof(char));

    int i = 0;
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            ausgabe[i++] = field->arr[x][y].content;
        }
        ausgabe[i++] = '\n';
    }
    system("clear");
    printf("%s", ausgabe);

    free(ausgabe);
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
            field->arr[x][y].n = 0;
        }
    }
}
void createBorder(struct field *field) // Malt den Rand
{
    for (int x = 0; x < field->width; x++)
    {
        field->arr[x][0].content = Border;
        field->arr[x][0].dx = 0;
        field->arr[x][0].dy = 0;

        field->arr[x][field->height - 1].content = Border;
        field->arr[x][field->height - 1].dx = 0;
        field->arr[x][field->height - 1].dy = 0;
    }
    for (int y = 0; y < field->height; y++)
    {
        field->arr[0][y].content = Border;
        field->arr[0][y].dx = 0;
        field->arr[0][y].dy = 0;

        field->arr[field->width - 1][y].content = Border;
        field->arr[field->width - 1][y].dx = 0;
        field->arr[field->width - 1][y].dy = 0;
    }
}
void createRandomEntity(struct field *field, int n_max, enum content content)
{
    srand(time(0));
    int n = n_max;
    if (n_max > 1)
    {
        n = rand() % n_max;
    }
    for (int i = 0; i < n; i++)
    {
        int
            r_x = (rand() % (field->width - 2)) + 1,
            r_y = (rand() % (field->height - 2)) + 1;

        field->arr[r_x][r_y].content = content;
        field->arr[r_x][r_y].dx = 0;
        field->arr[r_x][r_y].dy = 0;
    }
}
struct position
{
    int x;
    int y;
};
struct position findFirst(struct field *field, enum content content)
{
    for (int y = 0; y < field->height; y++)
    {
        for (int x = 0; x < field->width; x++)
        {
            if (field->arr[x][y].content == content)
            {
                struct position pos = {.x = x, .y = y};
                return pos;
            }
        }
    }
}
void moveSnake(struct field *field)
{
    struct position head_act = findFirst(field, Snake_head);
    struct position head_next = {
        .x = head_act.x + field->arr[head_act.x][head_act.y].dx,
        .y = head_act.y + field->arr[head_act.x][head_act.y].dy,
    };

    if (!isInField(field, head_next.x, head_next.y)) // Falls außer Spielfeld bricht ab
    {
        field->lost = 1;
    }
    if (field->arr[head_next.x][head_next.y].content != Food && field->arr[head_next.x][head_next.y].content != Empty) // falls nächstes Feld weder Essen noch Frei -> spiel verloren
    {
        field->lost = 1;
    }

    bool eatenFood = field->arr[head_next.x][head_next.y].content == Food ? true : false; //hat schlange gerade etwas gegessen

    field->arr[head_next.x][head_next.y] = field->arr[head_act.x][head_act.y]; //Kopf auf nächstes Feld setzen

    bool tail_exist = false;
    struct position tail_last = {0};
    for (int y = 0, n = 0; y < field->height; y++) // Findet letztes Stück Tail
    {
        for (int x = 0; x < field->width; x++)
        {
            if (field->arr[x][y].content != Snake_tail)
            {
                continue;
            }
            if (field->arr[x][y].n < n)
            {
                continue;
            }
            tail_exist = true;

            n = field->arr[x][y].n;
            tail_last.x = x;
            tail_last.y = y;
        }
    }
    if (tail_exist == false)
    {
        tail_last = head_act; //wenn es noch kein Tail gibt ist die letzte Position des Kopfes leste Position des Tail
    }

    //Tail wird auf letzte Position des Kopf gesetzt
    field->arr[head_act.x][head_act.y].content = Snake_tail;
    field->arr[head_act.x][head_act.y].n = 1;
    field->arr[head_act.x][head_act.y].updated = true;

    for (int y = 0; y < field->height; y++) // index des Tail anpassen (alles ist jetzt eins index höher)
    {
        for (int x = 0; x < field->width; x++)
        {
            if (field->arr[x][y].content != Snake_tail)
            {
                continue;
            }
            if (field->arr[x][y].updated == true)
            {
                continue;
            }
            field->arr[x][y].n += 1;
            field->arr[x][y].updated = true;
        }
    }

    if (eatenFood == false)
    {
        field->arr[tail_last.x][tail_last.y].content = Empty; //Falls die Schlange nicht gegessen hat wird das letzte Stück tail gelöscht
    }

    for (int y = 0; y < field->height; y++) // alle updated zurücksetzen
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
    createRandomEntity(&spielfeld, 3, Food); // erschafft essen

    spielfeld.arr[5][5].content = Snake_head;
    spielfeld.arr[5][5].dx = 1;

    printField(&spielfeld);
    while (!spielfeld.lost)
    {
        if (kbhit())
        {
            getMove(&spielfeld);
        }
        createRandomEntity(&spielfeld, 3, Food); // erschafft essen
        moveSnake(&spielfeld);

        printField(&spielfeld);
        usleep(150 * 1000); // ms delay
    }

    return 0;
}
