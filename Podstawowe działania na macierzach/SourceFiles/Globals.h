#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>

struct Matrix {
	std::string name;
	int rows;
	int cols;
	std::vector<std::vector<float>> data;
	Matrix() = default;
	Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows, std::vector<float>(cols)) {};
	Matrix(std::string name, int rows, int cols) : name(name), rows(rows), cols(cols), data(rows, std::vector<float>(cols)) {};
};

extern std::vector<Matrix> Matrices;

extern Matrix* selectedMatrix;

#endif