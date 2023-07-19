#include <iostream>
#include "Matrix.h"

int main()
{
	Matrix m1 = Matrix(2, 3);


	m1.RandomizeFloats(0, 10);
	std::cout << "M1:" << std::endl;
	m1.TextDraw();
	std::cout << std::endl;

	Matrix m2 = Matrix::Transpose(m1);
	std::cout << "M2:" << std::endl;
	m2.TextDraw();
	std::cout << std::endl;

	//Matrix m2 = Matrix(3, 2);
	//m2.RandomizeFloats(0, 10);
	//std::cout << "M2:" << std::endl;
	//m2.TextDraw();
	//std::cout << std::endl;

	//Matrix m3 = Matrix::MatrixMultiply(m1, m2);
	//std::cout << "After multiply:" << std::endl;
	//m3.TextDraw();
	//std::cout << std::endl;

	std::cout << "breakline";
}
