//
//  Matrix.cpp
//  Text Encryptor
//
//  Created by Akshay Subramaniam on 14/5/15.
//  Copyright (c) 2015 Incultus. All rights reserved.
//

#include "ThreeByThreeMatrix.h"

// Creates the matrix from a 3x3 matrix.
ThreeByThreeMatrix::ThreeByThreeMatrix(int items[3][3]) {
    for (int x = 0; x < 3; x ++)
        for (int y = 0; y < 3; y ++)
            this->items[x][y] = items[x][y];
}

// Multiplies the matrix by a 3x1 matrix. Essentially the matrix math expressed in code form.
ThreeByOneMatrix ThreeByThreeMatrix::multiply(ThreeByOneMatrix threeOne) {
    int newMatrix[3][1];
    for (int x = 0; x < 3; x ++) {
        int sum = 0;
        for (int y = 0; y < 3; y ++)
            // Sums up the item in each row multiplied by each row in the 3x1 matrix.
            sum += items[x][y] * threeOne.getItem(y);
        // Sets the value in the row of the new 3x1 matrix to this sum.
        newMatrix[x][0] = sum;
    }
    // Returns new matrix.
    return ThreeByOneMatrix(newMatrix);
    
}

ThreeByThreeMatrix ThreeByThreeMatrix::invert() {
    // Matrix of Minors.
    int determinantMatrix;
    // Cofactor matrix.
    int cofactorMatrix[3][3];
    // The adjoint or adjunct matrix.
    int adjointMatrix[3][3];
    // Final inverted matrix.
    int inverseMatrix[3][3];
    std::vector<std::vector<std::vector<int>>> minorMatrixes;
    
    // Gets the various 2x2 minor matrices.
    minorMatrixes = getMatrixOfMinors();
    
    // Keeps track of the position of the cofactor Matrix.
    int x = 0;
    int y = 0;
    
    // Creates the cofactor matrix from the determinants of the Matrix of Minors.
    for (std::vector<std::vector<std::vector<int>>>::iterator it = minorMatrixes.begin(); it != minorMatrixes.end(); ++it) {
        std::vector<std::vector<int>> minorMatrix = *it;
        // Getting the determinants of the Matrix of Minors
        cofactorMatrix[x][y] = minorMatrix[0][0] * minorMatrix[1][1] - minorMatrix[0][1] * minorMatrix[1][0];
        ++y;
        // Checks if the column position overflows, and fixes accordingly
        if (y > 2) {
            ++x;
            y = 0;
        }
    }
        
    // Negatizes the proper elements, creating the cofactor matrix.
    cofactorMatrix[0][1] = cofactorMatrix[0][1] * -1;
    cofactorMatrix[1][0] = cofactorMatrix[1][0] * -1;
    cofactorMatrix[1][2] = cofactorMatrix[1][2] * -1;
    cofactorMatrix[2][1] = cofactorMatrix[2][1] * -1;
    
    // Transposes the cofactor to create the adjunct matrix.
    for (int x = 0; x < 3; x ++)
        for (int y = 0; y < 3; y ++)
            adjointMatrix[x][y] = cofactorMatrix[y][x];
    
    
    determinantMatrix = items[0][0] * cofactorMatrix[0][0] + items[0][1] * cofactorMatrix[0][1] +
    items[0][2] * cofactorMatrix[0][2];

    //Multiplies each element of the adjunct matrix by the determinant of the matrix to get the inverted matrix.
    for (int x = 0; x < 3; x ++)
        for (int y = 0; y < 3; y ++)
            inverseMatrix[x][y] = adjointMatrix[x][y] / determinantMatrix;
    
    // Modulos each value of the matrix by 90. If the value is negative, modulos in accordance with the distance it is away from -90.
    for (int x = 0; x < 3; x ++)
        for (int y = 0; y < 3; y ++) {
            if (inverseMatrix[x][y] > 0)
                inverseMatrix[x][y] = inverseMatrix[x][y] % 90;
            else
                inverseMatrix[x][y] = (90 - abs(inverseMatrix[x][y]) % 90) % 90;
        }
    
    // Returns the new matrix.
    return ThreeByThreeMatrix(inverseMatrix);
    
}

std::vector<std::vector<std::vector<int>>> ThreeByThreeMatrix::getMatrixOfMinors() {
    std::vector<std::vector<std::vector<int>>> minorMatrixes;
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y) {
            // Creates one element of the Matrix of Minors in the form of a vector.
            std::vector<int> minorMatrix = getMinorMatrix(x, y);
            // We eventually need to convert the Matrix of Minors into a vector of 2x2 arrays. So we need to create a number of temporary, 2x2 arrays.
            std::vector<std::vector<int>> minorMatrixTwoD;
            minorMatrixTwoD.resize(2);
            for (int i = 0; i < 2; ++i)
                minorMatrixTwoD[i].resize(2);
            // This will help us keep track of the position of the element needed to be inserted into the 2x2 array.
            int count = 0;
            
            //Creating a 2x2 array for each element in the minorMatrixes vector.
            for (std::vector<int>::iterator iterator = minorMatrix.begin(); iterator != minorMatrix.end(); ++iterator) {
                // Sets the matrix element with accordance to the position of minor element relative to the original items matrix.
                switch (count) {
                    case 0:
                        minorMatrixTwoD[0][0] = *iterator;
                        break;
                    case 1:
                        minorMatrixTwoD[0][1] = *iterator;
                        break;
                    case 2:
                        minorMatrixTwoD[1][0] = *iterator;
                        break;
                    case 3:
                        minorMatrixTwoD[1][1] = *iterator;
                        break;
                }
                // Increases count to keep track of the position.
                ++count;
            }
            
            // Adds the newly created 2x2 array to the Matrix of Minors.
            minorMatrixes.push_back(minorMatrixTwoD);
    }
    return minorMatrixes;
}


// Note that this method returns the vector form, rather than the array form, of the minor. This is to avoid creating arrays with dynamically allocated memories.
std::vector<int> ThreeByThreeMatrix::getMinorMatrix(int row, int column) {
    std::vector<int> minorMatrix;
    for (int x = 0; x < 3; x ++)
        for (int y = 0; y < 3; y ++)
            // Neither on the same row nor column. Essentially gets each element of the minor.
            if (x != row && y != column)
                minorMatrix.push_back(items[x][y]);
    
    return minorMatrix;
    
}

// Gets the individual item.
int ThreeByThreeMatrix::getItem(int row, int column) {
    return items[row][column];
}