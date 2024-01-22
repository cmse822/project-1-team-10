# Project 1 Report
## Warmup

|              Kernel              | Arithmetic Intensity (flops/byte) |
| :-------------------------------: | :-------------------------------: |
| `Y[j] += Y[j] + A[j][i] * B[i]` |     $\frac{2}{4 \times 8}$     |
|       `s += A[i] * A[i]`       |     $\frac{2}{1 \times 8}$     |
|       `s += A[i] * B[i]`       |     $\frac{2}{2 \times 8}$     |
|     `Y[i] = A[i] + C*B[i]`     |     $\frac{2}{3 \times 8}$     |

## Part 1
### 1
Code is available in `project_1.cpp`.
```python
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
```

### 2
for a matrix multiplication with two matrices of size $N \times N$, we will have $N \times N  \times N$ multiplication and at the end we will have $N \times N  \times (N-1)$ additions. Which will result in $N \times N  \times (2N-1) flops$
### 3 
![matrix_size_flops.png](assets%2Fmatrix_size_flops.png)

