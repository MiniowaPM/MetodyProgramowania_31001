#include "MatrixEquations.h"
#include <vector>
#include <string>
#include <iostream>
#include "Globals.h"

Matrix MatrixEquations::addMatrix(Matrix& matrix1, Matrix& matrix2) {
    if (matrix1.rows == matrix2.rows and matrix1.cols == matrix2.cols) {
        Matrix resultMatrix(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                resultMatrix.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
            }
        }
        return resultMatrix;
    }
};

Matrix MatrixEquations::subtractMatrix(Matrix& matrix1, Matrix& matrix2) {
    if (matrix1.rows == matrix2.rows and matrix1.cols == matrix2.cols) {
        Matrix resultMatrix(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                resultMatrix.data[i][j] = matrix1.data[i][j] - matrix2.data[i][j];
            }
        }
        return resultMatrix;
    }
};

Matrix MatrixEquations::multiplyMatrix(Matrix& matrix1, Matrix& matrix2) {
    if (matrix1.cols == matrix2.rows) {
        Matrix resultMatrix(matrix1.rows, matrix2.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix2.cols; j++) {
                for (int k = 0; k < matrix1.cols; k++) {
                    resultMatrix.data[i][j] = resultMatrix.data[i][j] + matrix1.data[i][k] * matrix2.data[k][j];
                }
            }
        }
        return resultMatrix;
    }
};

Matrix MatrixEquations::multiplyMatrix(Matrix& matrix, float value) {
    Matrix resultMatrix(matrix.rows, matrix.cols);
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            resultMatrix.data[i][j] = matrix.data[i][j] * value;
        }
    }
    return resultMatrix;
};

Matrix MatrixEquations::powerOfMatrix(Matrix& matrix, int value) {
    Matrix resultMatrix(matrix.rows, matrix.cols);
    resultMatrix = matrix;
    for (int k = 1; k < value; k++) {
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                resultMatrix.data[i][j] = resultMatrix.data[i][j] * matrix.data[i][j];
            }
        }
    }
    return resultMatrix;
};

Matrix MatrixEquations::transposeMatrix(Matrix& matrix) {
    Matrix resultMatrix(matrix.cols, matrix.rows);
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            resultMatrix.data[i][j] = matrix.data[j][i];
        }
    }
    return resultMatrix;
};

float MatrixEquations::detMatrix(Matrix& matrix) {
    if (matrix.cols != matrix.rows) {
        return 0;
    };
    if (matrix.cols == 2) {
        return matrix.data[0][0] * matrix.data[1][1] - matrix.data[0][1] * matrix.data[1][0];
    }
    else {
        float cumulativeSum = 0, sign = 1, determinant;
        for (int colNum = 0; colNum < matrix.cols; colNum++) {
            for (int rowNum = 0; rowNum < matrix.rows; rowNum++) {
                // Funkcja pomocnicza tworz¹ca pod macierz o stopieñ ni¿sz¹
                Matrix subMatrix = findSubMatrix(matrix, rowNum, colNum);
                determinant = detMatrix(subMatrix);
                cumulativeSum += matrix.data[0][colNum] * determinant * sign;
                sign = -sign;
            }
        }
        return cumulativeSum;
    }
};

Matrix MatrixEquations::findSubMatrix(Matrix& matrix, int skipRow, int skipCol) {
    Matrix subMatrix(matrix.rows - 1, matrix.cols - 1);
    int countRows = 0;
    for (int i = 0; i < matrix.rows; i++) {
        if (i == skipRow) continue;
        int countCols = 0;
        for (int j = 0; j < matrix.cols; j++) {
            if (j == skipCol) continue;
            subMatrix.data[countRows][countCols] = matrix.data[i][j];
            countCols++;
        }
        countRows++;
    }
    return subMatrix;
}

Matrix MatrixEquations::inverseMatrix(Matrix& matrix) {
    Matrix resultMatrix(matrix.rows, matrix.cols);
    Matrix transposedMatrix = transposeMatrix(matrix);
    float det = detMatrix(matrix);
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++)
        {
            resultMatrix.data[i][j] = 1 / det * transposedMatrix.data[i][j];
        }
    }
    return resultMatrix;
};

int MatrixEquations::rankMatrix(Matrix& matrix) {
    float determinant;
    for (int colNum = 0; colNum < matrix.cols; colNum++) {
        for (int rowNum = 0; rowNum < matrix.rows; rowNum++) {
            // Funkcja pomocnicza tworz¹ca pod macierz o stopieñ ni¿sz¹
            Matrix subMatrix = findSubMatrix(matrix, rowNum, colNum);
            determinant = detMatrix(subMatrix);
            if (determinant != 0) {
                return subMatrix.cols;
            }
        }
    }
};

Matrix MatrixEquations::createMatrix(std::string name, int rows, int cols) {
    Matrix matrix(name, rows, cols);
    matrix.data.assign(rows, std::vector<float>(cols, 0.0));
    Matrices.push_back(matrix);
    return matrix;
};

void MatrixEquations::quickSaveMatrix(Matrix& curMatrix, std::string name) {
    Matrix matrix(name, curMatrix.rows, curMatrix.cols);
    matrix.data = curMatrix.data;
    Matrices.push_back(matrix);
};

void MatrixEquations::copyMatrix(Matrix& matrixA, std::string matrixB) {
    quickSaveMatrix(matrixA, matrixB);
};

void MatrixEquations::insertIntoMatrix(Matrix& matrix, std::vector<std::vector<float>> data) {
    matrix.data = data;
};

void MatrixEquations::deleteMatrix(std::string name) {
    for (auto i = Matrices.begin(); i != Matrices.end(); ++i) {
        if (i->name == name) {
            Matrices.erase(i);
            return;
        }
    }
};

Matrix* MatrixEquations::getMatrixByName(std::string matrixName) {
    for (Matrix& matrix : Matrices) {
        if (matrix.name == matrixName) {
            return &matrix;
        }
    }
    return nullptr;
}