#pragma once
#include <vector>
#include <iostream>
#include <string>

#include "Helper.h"

class Matrix {

public:

	//constructor
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
	static Matrix MatrixMultiply(Matrix m1, Matrix m2) {

		if (!MatrixMultiplyCompatible(m1, m2))
			return Matrix(0,0);

		Matrix toReturn = Matrix(m1.rows, m2.cols);

		//loops through each cell in the resulting matrix
		for (int r = 0; r < toReturn.rows; r++) {
			for (int c = 0; c < toReturn.cols; c++) {

				float sum = 0;

				//loops through each column in matrix 1
				for (int i = 0; i < m1.cols; i++)
						
					sum += m1.matrix[r][i] * m2.matrix[i][c];
					
				toReturn.matrix[r][c] = sum;
			}
		}

		return toReturn;
	}


	//returns a transposed matrix of the input matrix
	static Matrix Transpose(Matrix m1) {

		Matrix m2 = Matrix(m1.cols, m1.rows);

		for (int r = 0; r < m2.rows; r++)
			for (int c = 0; c < m2.cols; c++)
				m2.matrix[r][c] = m1.matrix[c][r];

		return m2;
	}


	//returns if the two matrices can be elementwise operated
	static bool ElementWiseCompatible(Matrix m1, Matrix m2) {
		return m1.rows == m2.rows && m1.cols == m2.cols;
	}

	//returns if two matrices can be matrix multiplied
	static bool MatrixMultiplyCompatible(Matrix m1, Matrix m2) {
		return m1.cols == m2.rows;
	}

private:
	std::vector<std::vector<float>> matrix; //rows x cols
	int rows;
	int cols;
};