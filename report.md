# Project 1 Report

## Warmup

|              Kernel              | Arithmetic Intensity (flops/byte) |
| :-------------------------------: | :-------------------------------: |
| `Y[j] += Y[j] + A[j][i] * B[i]` |     $\frac{3}{4 \times 8}$     |
|       `s += A[i] * A[i]`       |     $\frac{2}{1 \times 8}$     |
|       `s += A[i] * B[i]`       |     $\frac{2}{2 \times 8}$     |
|     `Y[i] = A[i] + C*B[i]`     |     $\frac{2}{3 \times 8}$     |

## Part 1

### 1

Code is available in `project_1.cpp`.

```c
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

To calculate the Peak Performance, we have:

$$
\text{Theoretical peak performance (FLOPS)} = \text{Clock speed (GHz)} \times \text{Number of cores} \times \text{Number of FLOPS per clock cycle}
$$

By assuming that the processor is capable of one flop per clock cycle, the theoretical peak performance for each core is equal to its clock speed.

For $N=100$, the performance is lower than the theoretical peak performance, which means that the process is memory-bounded.

For our two systems
| CPU                | Avg Runtime (ms) | Mflops/s |
| ------------------ | ---------------- | -------- |
| G14 Zephyrus (local) | 0.13351        | 14904.759 |
| amd20              | 0.4576           | 4348.807  |

#### HPCC (amd20)
We threw the matrix-matrix kernel onto the HPCC to test its performance.  We used the AMD20 development node to run our code.  That node was equipped with a 2.6GHz processor, with 128 cores and 960GBs of memory according to https://docs.icer.msu.edu/development_nodes/.  This gives a theoretical performance of **332.8 GFLOPs/sec**, though the ERT shows a peak of **16GFLOPs/sec**. We submitted a job to the hpcc and the resulting graph is shown below. 

![mflops_hpcc.png](assets%2Fmflops_hpcc.png)

The theoretical performance is much, much higher than what's shown on the graph. The extremely low performance of the code compared to the theoretical max is probably due to having to share the node with other users so not all cores were dedicated to this process.

#### G14 Zephyrus
We tested the performance of a laptop, G14 Zephyrus, that is equipped with a processor with a base clock speed of 3.3GHz, 8 cores, and 3 caches(L1=512KB, L2=4MB, L3=16MB). These specs give it the theoretical performance of **26.4 GFLOPs/sec** assuming 1 flop per clock cycle. For this laptop we compiled the matrix-matrix multiplication code with the -o3 flag and got the following graph.

![matrix_size_flops_daniel_laptop.png](assets%2Fmatrix_size_flops_daniel_laptop_line.png)

We can see that the performance of this laptop is much less than the theoretical limit, marked by the red line.  This is probably because of background programs being run while the matrix-matrix multiplication program was running which would limit the amount of processing power it would have access to.  

Some features of the graph we can see is that there are 2 distinct valleys as the matrix size increases.  These would be when the the matrix size becomes too large to store in the caches, and the processor has to reach for a larger size of cache.  After the second drop, the performance can only be limited by the compute given us a flat trend after N = 2000.

## Part 2: The Roofline Model

### 3: ERT Ridge Points

#### HPCC

The figure below is the roofline chart generated by the ERT using the 'config.edem.hpcc.onur.cse' config.
![dev_amd20_roofline.png](assets%2Fdev_amd20_roofline.png)
We can see the theoretical peak to be around 16 GFlOPS/s for most ridge points, but if the kernel happens to be bound by the Spec DRAM, the theoretical peak rises to be almost 80 GFLOP/s.  The ridgepoints of L1 starts at ~0.20 and L2 starts at ~0.24.  The regualr DRAM's ridgepoint is ~0.70 but interestingly the Spec DRAM's ridgepoint is at ~1.5 due to it not stopping at the same GLOP/s roof as the other ridgepoints.  We theorize that the hpcc has some special memory that it can access if it needs quick access for larger programs

#### G14 Zephyrus

The figure below is roofline chart generated by the ERT using 'config.serial.local.64' config on the g14 zephyrus laptop.
![daniel_laptop_roofline.jpeg](assets%2Fdaniel_laptop_roofline.jpg)

We can see that it calculate the peak performance to be 23.9 GFLOPS/s which is slightly less than the theoretical peak of 26.4 GFLOPS/s.  The ridgepoints for the caches are as follows:

| Memory | FLOPS/byte |
| :----: | :--------: |
|   L1   |    0.17    |
|   L2   |    0.19    |
|   L3   |    0.58    |
|   L4   |    0.76    |
|  DRAM  |    0.95    |

Comparing this roofline model to the matrix-matrix multiplication chart, we can see that the 2 drops would correspond with the matrix size not being able to be fit into a singular cache.  The 2 drops suggest that after around N = 1500, the processor was bandwidth-bound in the L3 cache.

### 4: FP kernels in "Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures"

In the paper "Roofline: An Insightful Visual Performance Model for Floating-Point Programs and Multicore Architectures", the authors detail 4 kernels that they used to test performance.  The 4 kernels were Sparse Matrix-Vector Multiply. Lattice-Boltzmann Magnetohydro-dynamics, Stencil, and Three-Dimensional Fast Fourier.  In the following sections, we'll talk about how they would perform our the platforms we tested as well as suggest some optimization strategies for them.

#### Sparce Matrix-Vector Multiply Optimizations (SpMV)

The high end of operational intensity for this kernel was 0.25 FLOPS/byte. For the g14 laptop, this would mean that this kernel reaches around 10 GFLOPS/s for being compute bound.  For the HPCC it can reach 12 GFLOPS/s.

#### Lattice-Boltzmann Magnetohydro-dynamics (LBMHD)

The high end of operational intensity for this kernel was 1.07 FLOPS/byte. For the g14 laptop, the kernel is bound by our peak.  For the HPCC it is also bound by the peak, but can each just under the Spec DRAM peak of 80GB/s

#### Multigrid kernel that updates 3-D stencil (Stencil)

The high end of operational intensity for this kernel was 0.50 FLOPS/byte.  The g14 reaches a peak of around 12GB/s being bandwith bound, and around 13gb/s also bandwidth bound.

#### Three-Dimensional Fast Fourier Transform (3-D FFT)

The high end of operational intensity for this kernel was 1.64 FLOPS/byte.  The g14 and hpcc again are bound by the peak.

### 4:

In this analysis, we focus on the upper bound of operation intensity. Under this assumption, and based on the roofline plots demonstrated, it becomes clear that all four of our approaches are predominantly compute-bound. In such cases, where the limitation is computational, two key recommendations emerge: enhancing instruction level parallelism (ILP) and balancing floating-point operations. Balancing involves striving for a similar number of floating-point additions and subtractions, an aspect deeply ingrained in the specific algorithm being utilized, which may not always be subject to modification. It's important to note that the impact of ILP can vary depending on the system's architecture. Nevertheless, a viable method to effectively implement ILP is through loop unrolling, which can help optimize computational kernels despite these varying architectural influences

### 5: Warm Up Kernel Performance Analysis

The respective arithmetic intensity for each kernel in the warm up is 0.0625, 0.25, 0.125, and 0.083 (in order). Assuming this is the high end of intensity for each, and they are running on the G14 Zephyrus, they will likely have the following performance:

![warm_up_prediction.png](assets%2Fwarm_up_prediction.png)

As can be seen, all but the second kernel are compute bound, while the second has a mix of somput and memory bounding. As such, all of them could potentially improve if optimized using techniques to maximize efficiency. To break through the limitations of memory-bound issues, reorganizing loops to ensure stride access and utilizing software prefetching stand out as potential optimizations. These techniques help get the most out of memory, improve cache utilization, reduce register pressure, and allow for long unit stride accesses. These would very likely imporve the performance for most of the warm up kernels.

### 6:

The rooflines of memory bandwidth are directly related to the algorithmic performance as a function of matrix size.
As matrix size increases, the ability to reuse data efficiently becomes more critical, impacting arithmetic intensity and, consequently, overall performance. In addition, the theoretical peak performance (26.4Gflops/s) is the computational roof. In the matrix-matrix multiplication results, the  performance generally stays below this peak, indicating that computational capability may not the sole limiting factor.
