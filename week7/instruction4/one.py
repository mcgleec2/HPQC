import numpy as np

# Initialise the workspace
workspace = np.array([[1.]])

def pushQubit(state):
    global workspace
    workspace = np.kron(workspace, state)

def probQubit():
    global workspace
    workspace = np.reshape(workspace, (-1, 2))
    return np.linalg.norm(workspace, axis=0)**2

def measureQubit():
    global workspace
    prob = probQubit()
    measurement = np.random.choice(2, p=prob)
    workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement]))
    return str(measurement)

# Run simulation
output = ""
for n in range(30):
    pushQubit([0.6, 0.8])
    output += measureQubit()

print(output)
