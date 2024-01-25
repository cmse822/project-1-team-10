#include <iostream>
#include <vector>
#include <random>
#include "get_walltime.c"
// Matrix struct (no need to deal with private data, so class uneeded)
struct Matrix
{
	// Matrix has a dimension and a 1D array (simulates 2D)
	int dim;
	double* data;

	// Default constructor, matrix size 0
	Matrix()
	{
		dim = 0;
		data = nullptr;
	}

	// Constructor for given dimension, creates 0 matrix
	Matrix(int n)
	{
		dim = n;
		data = new double[n * n];
		for (int i = 0; i < dim * dim; ++i)
		{
			data[i] = 0;
		}
	}

	// Destructor
	~Matrix()
	{
		if (data != nullptr)
		{
			delete[] data;
		}
	}

	// Resize the dimension of a matrix, costly since a new array is made
	void Resize(int n)
	{
		if (n != dim)
		{
			dim = n;
			delete[] data;
			data = new double[n * n];
		}
	}

	// Generates either a matrix of random integers (pass true) or 0 - n^2 in order if false/default
	void GenerateElements(bool random = false)
	{
		std::uniform_real_distribution<double> uniform(-10, 10);
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

	// Prints a matrix, very simple 2D format seperated by spaces and lines
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

	// Function to simulate 2D access, example: mat(x, y) simulates mat[x][y]
	double operator() (int x, int y)
	{
		return data[x * dim + y];
	}
};

// Matrix multiplication, takes in Matrix a, b and c, then performs c = a*b.
void MatMul (const Matrix& a, const Matrix& b, Matrix& c)
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

// Basic testing I left for reference, not intended for actual use
void RunBasicTests()
{
//	Matrix a = Matrix();
//	std::cout << "Constructed" << std::endl;
//	std::cout << "Dim: " << a.dim << std::endl;
//	std::cout << std::endl;
//
//	a.Resize(10);
//	std::cout << "Resized" << std::endl;
//	std::cout << "Dim: " << a.dim << std::endl;
//	a.DisplayMatrix();
//	std::cout << std::endl;
//
//
//	a.GenerateElements(true);
//	std::cout << "Generated random elements" << std::endl;
//	a.DisplayMatrix();
//	a.GenerateElements();
//	std::cout << "Generated non-random elements" << std::endl;
//	a.DisplayMatrix();
//	std::cout << std::endl;
//
//	std::cout << "Checking element access" << std::endl;
//	std::cout << "[0][0] = " << a(0, 0) << std::endl;
//	std::cout << "[0][5] = " << a(0, 5) << std::endl;
//	std::cout << "[3][0] = " << a(3, 0) << std::endl;
//	std::cout << "[4][7] = " << a(4, 7) << std::endl;
//	std::cout << "[5][5] = " << a(5, 5) << std::endl;
//	std::cout << "[7][1] = " << a(7, 1) << std::endl;
//	std::cout << "[9][9] = " << a(9, 9) << std::endl;
//	std::cout << "Numbers should match rows/columns, ex: [3][4] = 34" << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "Checking matrix multiplication" << std::endl;
//	Matrix N = Matrix(5);
//	Matrix M = Matrix(5);
//	Matrix NM = Matrix(5);
//	Matrix MN = Matrix(5);
//	N.GenerateElements(true);
//	M.GenerateElements(true);
//	std::cout << "Matrix N:" << std::endl;
//	N.DisplayMatrix();
//	std::cout << "Matrix M:" << std::endl;
//	M.DisplayMatrix();
//	std::cout << std::endl;
//
//	MatMul(N, M, NM);
//	MatMul(M, N, MN);
//	std::cout << "Matrix N*M:" << std::endl;
//	NM.DisplayMatrix();
//	std::cout << "Matrix M*N:" << std::endl;
//	MN.DisplayMatrix();
//	std::cout << std::endl;
}

// Function to test matrices of size n
// It will perform "tests" number of tests to account for any variance, returning the average
double TestSize(int n, int tests)
{
	// Sum of times for all the tests for this size
	double timeSum = 0.0;
    // Run a certain number of tests
	for (int t = 0; t < tests; ++t)
	{

        // Generate matrices, populate randomly for a and b (c starts as all 0s)
		Matrix a(n);
		Matrix b(n);
		Matrix c(n);
		a.GenerateElements(true);
		b.GenerateElements(true);

        // Timing
        double start_time;
        get_walltime(&start_time);
        // Matrix Multiplication
		MatMul(a, b, c);
        double end_time;
        // Final Timing
        get_walltime(&end_time);
		timeSum += end_time - start_time;
	}

	// Return the average time taken
	return timeSum / tests;
}

double getMFlops(int test_size, double time_elapsed)
{
    long d = (double) test_size;
    long num_multiplication =  d * d * d;
    long num_additions = test_size * test_size * (test_size - 1);
    //    printf("#### %ld \t %ld ###\n", num_additions, num_multiplication);
    long num_operations = num_additions+num_multiplication;
    //    printf("#### %ld \t %f ###\n", num_operations,time_elapsed);
    double MFlops = num_operations / (time_elapsed * 1e6); // source used https://course.ccs.neu.edu/cs3650/ssl/TEXT-CD/Content/COD3e/InMoreDepth/IMD4-MFLOPS-as-a-Performance-Metric.pdf

    return MFlops;
}
// Function to output test data, should print to standard output
// The goal is to pipeline the data into python for plotting, ex: ./project_1 > python main.py
void OutputTestData(int test_size, double time_elapsed)
{
    printf("%d \t %.8f \t %.8f \n", test_size,  time_elapsed, getMFlops(test_size, time_elapsed));
}

int main()
{
	// Keeping a simple list of sizes to check for now, a list for storing results, and how many tests for each size
	std::vector<int> test_sizes = {5, 10, 50, 100, 500, 1000, 1200, 1500, 2500, 5000, 10000 };
	std::vector<double> test_results(test_sizes.size(), 0);
	int test_quantity = 100;

	// Run tests on each size
	for (int i = 0; i < test_sizes.size(); ++i)
	{
		test_results[i] = TestSize(test_sizes[i], test_quantity);
        OutputTestData(test_sizes[i], test_results[i]);
	}
}
