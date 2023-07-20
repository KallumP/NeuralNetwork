#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <functional>

#include "Helper.h"

class Matrix {

public:

	//default constructor
	Matrix() {
		rows = 0;
		cols = 0;
	}

	//empty matrix constructor
	Matrix(int _rows, int _cols) {
		rows = _rows;
		cols = _cols;

		//loops through each row to be initialised
		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			//loops through each column to be initialised
			for (int c = 0; c < cols; c++)

				//creates the column for this row
				rowBuffer.push_back(0.0f);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//makes a matrix out of the inputted vector
	Matrix(std::vector<float> input) {

		rows = input.size();
		cols = 1;

		//loops through each row to be initialised
		for (int r = 0; r < rows; r++) {

			//creates a vector of columns
			std::vector<float> rowBuffer;

			//adds the vector value for this row
			rowBuffer.push_back(input[r]);

			//adds this row to the matrix
			matrix.push_back(rowBuffer);
		}
	}

	//draws the matrix to the console
	void TextDraw() {
		for (int r = 0; r < rows; r++) {
			std::string rowString = "";
			for (int c = 0; c < cols; c++) {
				rowString += std::to_string(matrix[r][c]);

				if (c == cols - 1)
					continue;
				rowString += ", ";
			}
			std::cout << rowString << std::endl;
		}
	}

	//returns a flattened vector (the values from each row will be next to each other)
	std::vector<float> AsVector() {


		std::vector<float> toReturn;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				toReturn.push_back(matrix[r][c]);

		return toReturn;
	}

	//fills the matrix in with random floats between a bound
	void RandomizeFloats(int lowerBound, int upperBound) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] = Helper::RandomFloat(lowerBound, upperBound);
	}

	//fills the matrix in with random ints between a bound
	void RandomizeInts(int lowerBound, int upperBound) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] = Helper::RandomFloat(lowerBound, upperBound);
	}



	//adds the input value to all cells in the matrix
	void ScalarAdd(float n) {

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] += n;
	}

	//elementwise adds the two matrices together
	void ElementAdd(Matrix m) {

		if (!ElementWiseCompatible(*this, m))
			return;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] += m.matrix[r][c];
	}

	//elementwise subtract
	static Matrix ElementSubtract(Matrix m1, Matrix m2) {

		if (!ElementWiseCompatible(m1, m2))
			return Matrix();

		Matrix toReturn = Matrix(m1.rows, m1.cols);

		for (int r = 0; r < m1.rows; r++)
			for (int c = 0; c < m1.cols; c++)
				toReturn.matrix[r][c] = m1.matrix[r][c] - m2.matrix[r][c];

		return toReturn;
	}

	//multiplies the input value to all the cells in the matrix
	void ScalarMultiply(float n) {
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] *= n;
	}

	//elementwise multiplies the two matrices together
	void ElementMultiply(Matrix m) {

		if (!ElementWiseCompatible(*this, m))
			return;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				matrix[r][c] *= m.matrix[r][c];
	}

	//returns a new matrix whcich has the result of a matrix multiplcation on the two input matrices
	static Matrix Multiply(Matrix m1, Matrix m2) {

		if (!MatrixMultiplyCompatible(m1, m2))
			return Matrix(0, 0);

		Matrix toReturn = Matrix(m1.rows, m2.cols);

		//loops through each cell in the resulting matrix
		for (int r = 0; r < toReturn.rows; r++)
			for (int c = 0; c < toReturn.cols; c++)

				//loops through each value to add
				for (int i = 0; i < m1.cols; i++)

					toReturn.matrix[r][c] += m1.matrix[r][i] * m2.matrix[i][c];

		return toReturn;
	}



	//returns a transposed matrix of the input matrix
	static Matrix Transpose(Matrix m1) {

		Matrix m2 = Matrix(m1.cols, m1.rows);

		for (int r = 0; r < m1.rows; r++)
			for (int c = 0; c < m1.cols; c++)
				m2.matrix[c][r] = m1.matrix[r][c];

		return m2;
	}

	//applies a function to all cells in the matrix
	void ScalarMap(const std::function<float(float x)>& func) {
		for (int r = 0; r < rows; r++) {

			for (int c = 0; c < cols; c++) {

				float val = matrix[r][c];
				float newVal = func(val);
				matrix[r][c] = newVal;
			}
		}
	}

	static Matrix ScalarMap(Matrix m1, const std::function<float(float x)>& func) {

		Matrix toReturn = Matrix(m1.rows, m1.cols);

		for (int r = 0; r < toReturn.rows; r++) {

			for (int c = 0; c < toReturn.cols; c++) {

				float val = m1.matrix[r][c];
				float newVal = func(val);
				toReturn.matrix[r][c] = newVal;
			}
		}

		return toReturn;
	}

	//returns if the two matrices can be elementwise operated
	static bool ElementWiseCompatible(Matrix m1, Matrix m2) {
		return m1.rows == m2.rows && m1.cols == m2.cols;
	}

	//returns if two matrices can be matrix multiplied
	static bool MatrixMultiplyCompatible(Matrix m1, Matrix m2) {
		return m1.cols == m2.rows;
	}

	std::vector<std::vector<float>> matrix; //rows x cols
	int rows;
	int cols;
};