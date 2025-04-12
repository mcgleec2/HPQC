import numpy as np

# Gates
H_gate = (1 / np.sqrt(2)) * np.array([[1, 1], [1, -1]])
X_gate = np.array([[0, 1], [1, 0]])
Z_gate = np.array([[1, 0], [0, -1]])
I_gate = np.eye(2)

# Initialise workspace
workspace = np.array([[1.]])

# functions

def pushQubit(state):
    global workspace
    state = np.array(state).reshape(-1, 1)
    state = state / np.linalg.norm(state)
    workspace = np.kron(workspace, state)

def applyGate(gate, target):
    global workspace
    n = int(np.log2(workspace.shape[0]))
    gate_list = [I_gate] * n
    gate_list[target] = gate
    full_gate = gate_list[0]
    for g in gate_list[1:]:
        full_gate = np.kron(full_gate, g)
    workspace = full_gate @ workspace

def probQubit(qubit):
    global workspace
    n = int(np.log2(workspace.shape[0]))
    probs = np.abs(workspace.flatten())**2
    total0 = 0
    total1 = 0
    for i in range(len(probs)):
        bits = format(i, '0' + str(n) + 'b')
        if bits[qubit] == '0':
            total0 += probs[i]
        else:
            total1 += probs[i]
    return [total0, total1]

def measureQubit(qubit):
    global workspace
    n = int(np.log2(workspace.shape[0]))
    probs = np.abs(workspace.flatten())**2
    bitstrings = [format(i, '0' + str(n) + 'b') for i in range(2**n)]
    prob0 = sum(p for b, p in zip(bitstrings, probs) if b[qubit] == '0')
    prob1 = 1 - prob0
    result = np.random.choice(['0', '1'], p=[prob0, prob1])
    for i, b in enumerate(bitstrings):
        if b[qubit] != result:
            workspace[i] = 0
    workspace /= np.linalg.norm(workspace)
    return result

def TOFFn_gate(controls, target):
    global workspace
    n = int(np.log2(workspace.shape[0]))
    size = 2 ** n
    gate = np.eye(size)
    for i in range(size):
        bits = format(i, '0' + str(n) + 'b')
        if all(bits[c] == '1' for c in controls):
            flipped = list(bits)
            flipped[target] = '1' if bits[target] == '0' else '0'
            j = int(''.join(flipped), 2)
            gate[i, i] = 0
            gate[j, i] = 1
    workspace = gate @ workspace

def applyMultiControlledZ(qubits):
    global workspace
    n = int(np.log2(workspace.shape[0]))
    size = 2 ** n
    gate = np.eye(size)
    gate[-1, -1] = -1
    workspace = gate @ workspace

# Oracle

def zero_booleanOracle(qubits, result):
    for qubit in qubits:
        applyGate(X_gate, qubit)
    TOFFn_gate(qubits, result)
    for qubit in qubits:
        applyGate(X_gate, qubit)

def zero_phaseOracle(qubits):
    for qubit in qubits:
        applyGate(X_gate, qubit)
    applyMultiControlledZ(qubits)
    for qubit in qubits:
        applyGate(X_gate, qubit)

def sample_phaseOracle(qubits):
    applyGate(X_gate, qubits[1])
    applyMultiControlledZ(qubits)
    applyGate(X_gate, qubits[1])

#Grover's

def groverSearch(n, printProb=True):
    global workspace
    workspace = np.array([[1.]]) 

    qubits = list(range(n))
    for _ in qubits:
        pushQubit([1, 1])

    optimalTurns = int(np.pi/4 * np.sqrt(2**n) - 0.5)

    for _ in range(optimalTurns):
        sample_phaseOracle(qubits)

        for qubit in qubits:
            applyGate(H_gate, qubit)

        zero_phaseOracle(qubits)

        for qubit in qubits:
            applyGate(H_gate, qubit)

        if printProb:
            print(probQubit(0))

    for qubit in reversed(qubits):
        print(measureQubit(qubit), end="")
    print()

groverSearch(6)

