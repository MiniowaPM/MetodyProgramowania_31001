#pragma once
#include <vector>
#include <string>
#include "Globals.h"

class MatrixEquations{
public:	
	// Matrix Equations
	Matrix addMatrix(Matrix& matrix1, Matrix& matrix2);
	Matrix subtractMatrix(Matrix& matrix1, Matrix& matrix2);
	Matrix multiplyMatrix(Matrix& matrix1, Matrix& matrix2);
	Matrix multiplyMatrix(Matrix& matrix, float value);
	Matrix powerOfMatrix(Matrix& matrix, int value);
	Matrix transposeMatrix(Matrix& matrix);
	float detMatrix(Matrix& matrix);
	Matrix findSubMatrix(Matrix& matrix, int skipRow, int skipCol);
	Matrix inverseMatrix(Matrix& matrix);
	int rankMatrix(Matrix& matrix);
	// Matrix Operations
	Matrix createMatrix(std::string name, int rows, int cols);
	void quickSaveMatrix(Matrix& curMatrix, std::string name);
	void copyMatrix(Matrix& matrixA, std::string matrixBName);
	void insertIntoMatrix(Matrix& matrix, std::vector<std::vector<float>> data);
	void deleteMatrix(std::string name);
	Matrix* getMatrixByName(std::string matrixName);
};