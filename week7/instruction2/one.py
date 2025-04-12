import numpy as np

# Pauli-X gate (NOT gate)
X_gate = np.array([[0, 1],
                   [1, 0]])

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

# Reset workspace and push
workspace = np.array([[1.]])
pushQubit([1, 0])
print("input:", workspace)

# Apply NOT gate
applyGate(X_gate)
print("output:", workspace)

