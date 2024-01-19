#include <iostream>
#include <random>

struct Matrix
{
	int dim;
	double* data;

	Matrix()
	{
		dim = 0;
		data = nullptr;
	}

	Matrix(int n)
	{
		dim = n;
		data = new double[n * n];
		for (int i = 0; i < dim * dim; ++i)
		{
			data[i] = 0;
		}
	}

	~Matrix()
	{
		if (data != nullptr)
		{
			delete[] data;
		}
	}

	void Resize(int n)
	{
		if (n != dim)
		{
			dim = n;
			delete[] data;
			data = new double[n * n];
		}
	}

	void GenerateElements(bool random = false)
	{
		std::uniform_int_distribution<int> uniform(-10, 10);
		std::default_random_engine dre(std::random_device{}());
		for (int i = 0; i < dim * dim; ++i)
		{
			if (random)
			{
				data[i] = uniform(dre);
				continue;
			}
			data[i] = i;
		}
	}

	void DisplayMatrix()
	{
		for (int i = 0; i < dim; ++i)
		{
			for (int j = 0; j < dim; ++j)
			{
				std::cout << data[i * dim + j] << " ";
			}
			std::cout << std::endl;
		}
	}

	double operator() (int x, int y)
	{
		return data[x * dim + y];
	}
};

void MatMult (const Matrix& a, const Matrix& b, Matrix& c)
{
	for (int i = 0; i < a.dim; ++i)
	{
		for (int k = 0; k < a.dim; ++k)
		{
			for (int j = 0; j < a.dim; ++j)
			{
				c.data[i * a.dim + j] += a.data[i * a.dim + k] * b.data[k * a.dim + j];
			}
		}
	}

}
