
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cassert>

// Implementación de http://www.marekfiser.com/Projects/Conways-Game-of-Life-on-GPU-using-CUDA

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

__global__ void LifeKernel(const ubyte* lifeData, uint worldWidth, uint worldHeight, ubyte* resultLifeData) {

    uint worldSize = worldWidth * worldHeight;

    for (uint cellId = (blockIdx.x * blockDim.x) + threadIdx.x; cellId < worldSize; cellId += blockDim.x * gridDim.x) {
        
        uint x = cellId % worldWidth;
        uint yAbs = cellId - x;
        
        uint xLeft = (x + worldWidth - 1) % worldWidth;
        uint xRight = (x + 1) % worldWidth;
        
        uint yAbsUp = (yAbs + worldSize - worldWidth) % worldSize;
        uint yAbsDown = (yAbs + worldWidth) % worldSize;

        uint aliveCells = lifeData[xLeft + yAbsUp] + lifeData[x + yAbsUp]
            + lifeData[xRight + yAbsUp] + lifeData[xLeft + yAbs] + lifeData[xRight + yAbs]
            + lifeData[xLeft + yAbsDown] + lifeData[x + yAbsDown] + lifeData[xRight + yAbsDown];

        resultLifeData[x + yAbs] = aliveCells == 3 || (aliveCells == 2 && lifeData[x + yAbs]) ? 1 : 0;
    }
}

void runLifeKernel(ubyte*& d_lifeData, ubyte*& d_lifeDataBuffer, size_t worldWidth, size_t worldHeight, size_t iterationsCount, ushort threadsCount) {
    
    assert((worldWidth * worldHeight) % threadsCount == 0);
    size_t reqBlocksCount = (worldWidth * worldHeight) / threadsCount;
    ushort blocksCount = (ushort)std::min((size_t)32768, reqBlocksCount);

    for (size_t i = 0; i < iterationsCount; ++i) {
        LifeKernel <<<blocksCount, threadsCount >>> (d_lifeData, worldWidth,
            worldHeight, d_lifeDataBuffer);
        std::swap(d_lifeData, d_lifeDataBuffer);
    }
}
