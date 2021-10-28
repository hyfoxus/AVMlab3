#include <stdio.h>
#include "pbPlots.h"
#include "supportLib.h"


#define COUNT_POINTS 11
#define MAIN_POINTS_L 3


struct nomerator {
    double firstX;
    double secondX;
};

void fillInitialArray(double points[COUNT_POINTS][2]) {
    FILE *file = fopen("../input.txt", "r");
    if (file == NULL) {
        printf("no such file.");
    } else {
        for (int i = 0; i < COUNT_POINTS; ++i) {
            fscanf(file, "%lf ", &points[i][0]);
            fscanf(file, "%lf\n", &points[i][1]);
        }
        fclose(file);
    }
}

void lagransh(double arr[COUNT_POINTS][2], struct nomerator *nom, double *denom, int pos[MAIN_POINTS_L]) {

    /*for (int i = 0; i < MAIN_POINTS_L; ++i) {
        int n = 0;
        for (int j = 0; j < MAIN_POINTS_L; ++j) {
            if (i != j) {
                if (n % 2 != 0) nom[i].secondX = arr[pos[j]][0];
                else {
                    nom[i].firstX = arr[pos[j]][0];
                    n++;
                }
            }
        }
    }*/

    for (int i = 0; i < MAIN_POINTS_L; ++i) {
        denom[i] = 1;
        int n = 0;
        for (int j = 0; j < MAIN_POINTS_L; ++j) {
            if (i != j) {
                denom[i] *= (arr[pos[i] - 1][0] - arr[pos[j] - 1][0]);

                if (n % 2 != 0) nom[i].secondX = arr[pos[j] - 1][0];
                else {
                    nom[i].firstX = arr[pos[j] - 1][0];
                    n++;
                }
            }
        }
        denom[i] /= arr[pos[i] - 1][1];
    }
}


int main() {
    double points[COUNT_POINTS][2];
    fillInitialArray(points);
    struct nomerator nom[MAIN_POINTS_L];
    double denom[MAIN_POINTS_L];
    int pos[MAIN_POINTS_L];
    FILE *file = fopen("../input_points.txt", "r");
    for (int i = 0; i < MAIN_POINTS_L; ++i) {
        fscanf(file, "%d\n", &pos[i]);
    }
    fclose(file);
    lagransh(points, nom, denom, pos);
    double resultLagransh[2][COUNT_POINTS];
    for (int i = 0; i < COUNT_POINTS; ++i) {
        double x = points[i][0];
        resultLagransh[0][i] = x;
        double y = 0;
        for (int j = 0; j < MAIN_POINTS_L; ++j) {
            y += (x - nom[j].firstX) * (x - nom[j].secondX) / denom[j];
        }
        resultLagransh[1][i] = y;
    }
    FILE *file1 = fopen("../result.txt", "w");
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < COUNT_POINTS; ++j) {
            fprintf(file1, "%f\t", resultLagransh[i][j]);
        }
        fprintf(file1, "\n");
    }

    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    DrawScatterPlot(canvasReference, 600, 400, resultLagransh[0], COUNT_POINTS, resultLagransh[1], COUNT_POINTS);

    size_t length;
    double *pngdata = ConvertToPNG(&length, canvasReference->image);
    WriteToFile(pngdata, length, "result.png");

    return 0;
}
