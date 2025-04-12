import torch as pt
import numpy as np
import time

# Check for GPU
pt.autograd.set_grad_enabled(False)
if pt.cuda.is_available():
    print("GPU available")
else:
    print("Sorry, only CPU available")

# Gates
H_gate = (1 / np.sqrt(2)) * np.array([[1, 1], [1, -1]])
X_gate = np.array([[0, 1], [1, 0]])
Z_gate = np.array([[1, 0], [0, -1]])

# Global variables
workspace = None
namestack = []

# Quantum Stack
def pushQubit(name, weights):
    global workspace, namestack
    if workspace.shape == (1, 1):
        namestack = [] 
    namestack.append(name)
    weights = weights / np.linalg.norm(weights)
    weights = pt.tensor(weights, device=workspace.device, dtype=workspace.dtype)
    workspace = pt.reshape(workspace, (1, -1))
    workspace = pt.kron(workspace, weights)

def tosQubit(name):
    global workspace, namestack
    k = len(namestack) - namestack.index(name)
    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = pt.reshape(workspace, (-1, 2, 2**(k - 1)))
        workspace = pt.swapaxes(workspace, -2, -1)

def applyGate(gate, *names):
    global workspace
    if list(names) != namestack[-len(names):]:
        for name in names:
            tosQubit(name)
    workspace = pt.reshape(workspace, (-1, 2**len(names)))
    subspace = workspace[:, -gate.shape[0]:]
    gate = pt.tensor(gate.T, device=workspace.device, dtype=workspace.dtype)
    if workspace.device.type == 'cuda':
        pt.matmul(subspace, gate, out=subspace)
    else:
        subspace[:, :] = pt.matmul(subspace, gate)

def probQubit(name):
    global workspace
    tosQubit(name)
    workspace_2col = pt.reshape(workspace, (-1, 2))
    prob = pt.linalg.norm(workspace_2col, axis=0)**2
    prob = pt.Tensor.cpu(prob).numpy()
    return prob / prob.sum()

def measureQubit(name):
    global workspace, namestack
    prob = probQubit(name)
    result = np.random.choice(2, p=prob)
    workspace = workspace[:, [result]] / np.sqrt(prob[result])
    namestack.pop()
    return result

# Grover
def applyMultiControlledZ(qubits):
    global workspace
    size = workspace.shape[1]
    gate = pt.eye(size, device=workspace.device)
    gate[-1, -1] = -1
    workspace = workspace @ gate

def sample_phaseOracle(qubits):
    applyGate(X_gate, qubits[1])
    applyMultiControlledZ(qubits)
    applyGate(X_gate, qubits[1])

def zero_phaseOracle(qubits):
    for q in qubits:
        applyGate(X_gate, q)
    applyMultiControlledZ(qubits)
    for q in qubits:
        applyGate(X_gate, q)

# Grover
def groverSearch(n, printProb=True):
    global workspace
    workspace = pt.tensor([[1.]], device=workspace.device, dtype=workspace.dtype)

    qubits = [f'q{i}' for i in range(n)]
    for name in qubits:
        pushQubit(name, [1, 1])

    turns = int(np.pi/4 * np.sqrt(2**n) - 0.5)

    for _ in range(turns):
        sample_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        zero_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        if printProb:
            print(probQubit(qubits[0]))

    result = ''.join(str(measureQubit(q)) for q in reversed(qubits))
    print("Result:", result)

# GPU
workspace = pt.tensor([[1.]], device=pt.device('cuda' if pt.cuda.is_available() else 'cpu'), dtype=pt.float32)
t = time.process_time()
groverSearch(16, printProb=False)
print("\nWith GPU:", time.process_time() - t, "s")

# CPU
workspace = pt.tensor([[1.]], device=pt.device('cpu'), dtype=pt.float32)
t = time.process_time()
groverSearch(16, printProb=False)
print("\nWith CPU (single-core):", time.process_time() - t, "s")
