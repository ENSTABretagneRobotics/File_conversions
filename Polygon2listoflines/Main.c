#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

int main(void) {
    FILE *filein = fopen("wplocal.txt", "r");
    FILE *fileout = fopen("lines.txt", "w");
    if ((filein == NULL)||(fileout == NULL)) {
        fprintf(stderr, "Error: Could not open file.\n");
        return 1;
    }

    // Initial capacity to store points
    int capacity = 100;
    int count = 0;
    Point *points = malloc(capacity * sizeof(Point));
    if (points == NULL) {
        fprintf(stderr, "Memory error\n");
        fclose(filein);
        return 1;
    }

    // Read points from file
    while (fscanf(filein, "%lf %lf", &points[count].x, &points[count].y) == 2) {
        count++;
        // If we exceed the capacity, double the array size.
        if (count >= capacity) {
            capacity *= 2;
            Point *temp = realloc(points, capacity * sizeof(Point));
            if (temp == NULL) {
                fprintf(stderr, "Memory reallocation error\n");
                free(points);
                fclose(filein);
                return 1;
            }
            points = temp;
        }
    }
    fclose(filein);

    // Check we have at least two points.
    if (count < 2) {
        fprintf(stderr, "Not enough points in file to form a line.\n");
        free(points);
        return 1;
    }

    // Print out the consecutive lines
    // Each line output as: x1 y1 x2 y2
    for (int i = 0; i < count - 1; i++) {
        fprintf(fileout, "%lf %lf %lf %lf\n", points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }

    // OPTIONAL: If you want the polygon to be closed, uncomment the next line.
    //fprintf(fileout, "%lf %lf %lf %lf\n", points[count - 1].x, points[count - 1].y, points[0].x, points[0].y);

	fclose(fileout);

    free(points);
    return 0;
}
