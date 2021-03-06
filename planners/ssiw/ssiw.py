#! /usr/bin/env python3
import fd.grounding
import sys
import os
from libssiw import Sketch_SIW_Planner
# MRJ: Profiler imports
#from prof import profiler_start, profiler_stop

def main( domain_file, problem_file, sketch_file, use_goal_counter, plan_file ) :
	task = Sketch_SIW_Planner( )

	# fd.grounding.default(domain_file, problem_file, task)
	fd.grounding.sketch( domain_file, problem_file, sketch_file, use_goal_counter, task )

	#MRJ: Uncomment to check what actions are being loaded
	#for i in range( 0, task.num_actions() ) :
	#	task.print_action( i )

	# MRJ: Setting planner parameters is as easy as setting the values
	# of Python object attributes

	# MRJ: Maximum bound on width is set to 1
	task.iw_bound = 2

	# MRJ: log filename set
	task.log_filename = 'iw.log'

	# MRJ: plan file
	task.plan_filename = plan_file

	# DD: We call the setup method in SIW_Planner
	task.setup()

	# DD: find solution
	task.solve()

def debug() :
	main( "/home/bowman/Sandboxes/Fast-Downward/benchmarks/miconic-simpleadl/domain.pddl",
		"/home/bowman/Sandboxes/Fast-Downward/benchmarks/miconic-simpleadl/s3-0.pddl" )

if __name__ == "__main__":
	main( sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5] )
