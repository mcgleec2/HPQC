import torch as pt
pt.autograd.set_grad_enabled(False)      # disable autogradients
if pt.cuda.is_available():               # check if GPU is available
    print("GPU available") 
else:
    print("Sorry, only CPU available")
