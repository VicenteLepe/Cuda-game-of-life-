#include <iostream>
#include <random>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

// width -> ANCHO -> M
// height -> ALTO -> N
using namespace std;

#define ITERATIONS 10
#define m_worldWidth 10
#define m_worldHeight 10

typedef bool grid_t;
grid_t grid[m_worldWidth * m_worldHeight];
grid_t tempGrid[m_worldWidth * m_worldHeight];

int aliveCells;
int generation;

clock_t before, after;
double diff;

void initGameVariables(){
    aliveCells = 0;
    generation = 0;
}

void linearInitGrid(grid_t grid[m_worldWidth * m_worldHeight]){
    int i;
    for(i=0; i<(m_worldWidth * m_worldHeight); i++){
        grid[i] = 0;
    }
}

void linearRandomFillGrid(grid_t grid[m_worldWidth * m_worldHeight]){
    int i;

    // https://stackoverflow.com/questions/22923551/generating-number-0-1-using-mersenne-twister-c
    std::mt19937 generator (123);
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for(i=0; i < m_worldWidth * m_worldHeight; i++){

        double randomRealBetweenZeroAndOne = dis(generator);

        if(randomRealBetweenZeroAndOne > 0.5){
            grid[i] = 1;
            aliveCells++;
        }

    }
}

int countAliveCells(int x0, int x1, int x2,
                    int y0, int y1, int y2) {

    return grid[x0 + y0] + grid[x1 + y0] + grid[x2 + y0]
           + grid[x0 + y1] + grid[x2 + y1]
           + grid[x0 + y2] + grid[x1 + y2] + grid[x2 + y2];
}

void linearUpdateGrid(grid_t grid[m_worldWidth * m_worldHeight]){
    linearInitGrid(tempGrid);

    before = clock();
    for (int y = 0; y < m_worldHeight; ++y) {
        int y0 = ((y + m_worldHeight - 1) % m_worldHeight) * m_worldWidth;
        int y1 = y * m_worldWidth;
        int y2 = ((y + 1) % m_worldHeight) * m_worldWidth;

        for (int x = 0; x < m_worldWidth; ++x) {
            int x0 = (x + m_worldWidth - 1) % m_worldWidth;
            int x2 = (x + 1) % m_worldWidth;

            int aliveCells = countAliveCells(x0, x, x2, y0, y1, y2);
            tempGrid[y1 + x] = aliveCells == 3 || (aliveCells == 2 && grid[x + y1]) ? 1 : 0;
        }
    }

    //std::swap(grid; tempGrid);
    memcpy(grid, tempGrid, m_worldHeight * m_worldWidth);
    after = clock();

    diff = (after - before) * 1000.0 ;// CLOCKS_PER_SEC;

    generation++;

}


int main(int argc, char** argv){
    initGameVariables();
    linearInitGrid(grid);
    linearRandomFillGrid(grid);

    int i = ITERATIONS;
    while(i>0){
        linearUpdateGrid(grid);
        printf("------------\n");
        for(int j = 0 ; j < m_worldHeight; j++){
            for(int z = j * m_worldHeight; z < (j+1)*m_worldWidth; z++){
                printf("%i",grid[z]);
            }
            printf("\n");
        }

        i--;
    }


    return 0;
}
