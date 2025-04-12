import numpy as np

# Initialise workspace
workspace = np.array([[1.]])

def pushQubit(weights):
    global workspace
    weights = np.array(weights, dtype=complex).reshape(1, -1)
    workspace = np.kron(workspace, weights)

def tosQubit(k):
    global workspace
    if k > 1:
        # Reshape to (2**(k-1), 2, 2)
        workspace = np.reshape(workspace, (-1, 2, 2**(k - 1)))
        workspace = np.swapaxes(workspace, -2, -1)
        # 1D Vector
        workspace = workspace.reshape(1, -1)

# Push
workspace = np.array([[1.]])
pushQubit([1, 0])       # Q1
pushQubit([0.6, 0.8])   # Q2

print("before tosQubit:")
print(np.real_if_close(workspace))

# Move top of stack
tosQubit(2)

print("after tosQubit:")
print(np.real_if_close(workspace))

