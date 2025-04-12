import numpy as np

# SWAP gate for 2 qubits
SWAP_gate = np.array([
    [1, 0, 0, 0],
    [0, 0, 1, 0],
    [0, 1, 0, 0],
    [0, 0, 0, 1]
], dtype=complex)

# Workspace
workspace = np.array([[1.]])

def pushQubit(weights):
    global workspace
    weights = np.array(weights, dtype=complex).reshape(1, -1)
    workspace = np.kron(workspace, weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace, (-1, gate.shape[0]))
    np.matmul(workspace, gate.T, out=workspace)

def printWorkspace():
    real_workspace = np.real_if_close(workspace, tol=1e-10)
    print(real_workspace)

# Push two qubits
workspace = np.array([[1.]])
pushQubit([1, 0])
pushQubit([0.6, 0.8])

print("before SWAP:")
printWorkspace()

applyGate(SWAP_gate)

print("after SWAP:")
printWorkspace()

