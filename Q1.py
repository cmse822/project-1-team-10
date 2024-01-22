import matplotlib.pyplot as plt

# Replace this with the path to your file
file_path = 'results_1.txt'

# Initialize lists to store the data
matrix_sizes = []
performances = []

# Read the file
with open(file_path, 'r') as file:
    for line in file:
        # Split the line into components: Matrix Size, Time Taken, Performance
        parts = line.split()
        if len(parts) == 3:  # Ensure the line has 3 parts
            size, time, performance = parts
            matrix_sizes.append(int(size))
            performances.append(float(performance))

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(matrix_sizes, performances, marker='o', linestyle='-')
plt.title('Matrix-Matrix Multiplication Performance')
plt.xlabel('Matrix Size')
plt.ylabel('Performance (Mflop/s)')
plt.grid(True)
plt.savefig("./assets/matrix_size_flops.png")
