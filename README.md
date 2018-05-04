# Discrete-Robot-Planner
RANDOM​ PLANNER:
The random planner tries to find a path to the goal by randomly moving in the environment (only orthogonal moves are legal). If the planner can not find an acceptable solution in less than max_step_number, the search should fail. The random planner, while being erratic, has a short memory, and it will never attempt to visit a cell that was visited in the last sqrt(max_step_number)steps except if this is the only available option.
OPTIMAL PLANNER:
​A planner that goes to the goal with the shortest (non-colliding) path. Again, only orthogonal moves are legal.