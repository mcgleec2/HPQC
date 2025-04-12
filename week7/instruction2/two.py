import numpy as np

# Hadamard gate
H_gate = np.array([[1, 1],
                   [1, -1]]) * np.sqrt(1/2)

# Global workspace
workspace = np.array([[1.]])

def pushQubit(weights):
    global workspace
    weights = np.array(weights).reshape(1, -1)
    workspace = np.kron(workspace, weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace, (-1, gate.shape[0]))
    np.matmul(workspace, gate.T, out=workspace)

# Reset and initialise
workspace = np.array([[1.]])
pushQubit([1, 0])
print("input:", workspace)

# Apply Hadamard gate
applyGate(H_gate)
print("output:", workspace)

