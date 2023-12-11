lines_instructions = []
lines_cycles = []
with open("../../build/data/instructions.data", "r") as file_instructions:
    lines_instructions = file_instructions.read().split("\n")

with open("../../build/data/cycles.data", "r") as file_cycles:
    lines_cycles = file_cycles.read().split("\n")


with open("../../build/data/ipc.data", "w") as file_icp:
    for (i, line) in enumerate(lines_instructions):
        if i == len(lines_instructions) - 1:
            break
        ipc = float(lines_instructions[i].split(" ")[
                    1]) / float(lines_cycles[i].split(" ")[1])
        file_icp.write(lines_instructions[i].split(" ")[
                       0] + " " + str(ipc) + "\n")
