import numpy as np

# Initialise workspace
workspace = np.array([[1.]])

# Gate definitions
H_gate = (1 / np.sqrt(2)) * np.array([[1, 1], [1, -1]])

# Create Toffoli gate matrix
TOFF_gate = np.eye(8)
TOFF_gate[6, 6] = 0
TOFF_gate[7, 7] = 0
TOFF_gate[6, 7] = 1
TOFF_gate[7, 6] = 1

def pushQubit(state):
    global workspace
    workspace = np.kron(workspace, np.array(state).reshape(-1, 1))

def applyGate(gate):
    global workspace
    n_qubits = int(np.log2(workspace.shape[0]))
    gate_size = int(np.log2(gate.shape[0]))

    identity = np.eye(2 ** (n_qubits - gate_size))
    full_gate = np.kron(identity, gate)
    workspace = full_gate @ workspace

def probQubit():
    global workspace
    workspace = np.reshape(workspace, (-1, 2))
    return np.linalg.norm(workspace, axis=0)**2

def measureQubit():
    global workspace
    prob = probQubit()
    measurement = np.random.choice(2, p=prob)
    workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement]))
    workspace = np.reshape(workspace, (-1, 1))
    return str(measurement)

# Main loop
workspace = np.array([[1.]])
for i in range(16):
    pushQubit([1, 0])
    applyGate(H_gate)

    pushQubit([1, 0])
    applyGate(H_gate)

    pushQubit([1, 0])
    applyGate(TOFF_gate)

    q3 = measureQubit()
    q2 = measureQubit()
    q1 = measureQubit()
    print(q1 + q2 + q3, end=",")
