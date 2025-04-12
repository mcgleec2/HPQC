# Instruction 1
import numpy as np

# Initialise workspace
workspace = np.array([[1.]])

def pushQubit(weights):
        global workspace
        weights = np.array(weights).reshape(1, -1)
        workspace = np.kron(workspace, weights)

# First quibit
pushQubit([1, 0])
print(workspace)

# Second qubit
pushQubit([3/5, 4/5])
print(workspace)

