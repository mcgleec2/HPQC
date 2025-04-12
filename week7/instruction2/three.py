import numpy as np

# T gate
T_gate = np.array([
    [1, 0],
    [0, np.exp(np.pi / -4j)]
])

# Initialise workspace
workspace = np.array([[1.+0j]])

def pushQubit(weights):
    global workspace
    weights = np.array(weights, dtype=complex).reshape(1, -1)
    workspace = np.kron(workspace, weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace, (-1, gate.shape[0]))
    np.matmul(workspace, gate.T, out=workspace)

# Push
pushQubit([0.6, 0.8])
print("input:", workspace)

# Apply T gate
applyGate(T_gate)
print("output:", workspace)
