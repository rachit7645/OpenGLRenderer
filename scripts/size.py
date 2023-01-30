X = 1024
Y = 576

# Get input
bits = float(input("Enter bits per pixel: "))

# Convert to megabytes
rbytes = bits / 8.0
kBytes = rbytes / 1024.0
mBytes = kBytes / 1024.0

size = mBytes * X * Y * 60
print(f"The speed required is {size} MB/s.")
