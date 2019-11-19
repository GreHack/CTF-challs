#!/usr/bin/env python2
import sys
import angr
import claripy

def main(argv):

    p = angr.Project(argv[1], load_options={'auto_load_libs':False})

    theFlag = claripy.BVS('arg1', 50 * 8)
    state = p.factory.entry_state(args=["prog", theFlag])

    simulator = p.factory.simgr(state)
    simulator.explore(find=lambda s: "Yay" in s.posix.dumps(1))

    if len(simulator.found) == 0:
        raise Exception("Path not found")

    state = simulator.found[0]

    solution = state.se.eval(theFlag, cast_to=str)
    print("solution: " + repr(solution))

    return state

main(sys.argv)
