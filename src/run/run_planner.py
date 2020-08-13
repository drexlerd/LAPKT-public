from .. import planner
from os import access, X_OK, environ, pathsep
from contextlib import contextmanager
from time import time, process_time
from sys import stdout
from os.path import isfile, join, dirname, realpath
from pathlib import Path
from subprocess import run

rel_validate_file   =   Path('../../../bin/validate')
CWD                 =   dirname(realpath(__file__))
#-----------------------------------------------------------------------------#
@contextmanager
def time_taken( task_name: str) :
    """
    Track the time taken for a task.
    Usage - "with time_taken("<process name>") :"
    Arguments
    =========
    time_taken: string - name of the task
    """
    start = (time(), process_time())
    print("|----------------------------------------------------------|")
    print("***Started - {} ...***".format(task_name))
    print("|----------------------------------------------------------|")
    stdout.flush()
    yield
    #print( "{:.3f} , ".format(time.time()-start[0]), end="")
    print("----------------------------------------------------------")
    print(("***Finished {} after {:.3f} seconds wall-clock time, {:.3f} seconds "+
            "CPU time***").format( task_name, time()-start[0],
                process_time()-start[1] ))
    print("----------------------------------------------------------")
    stdout.flush()
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx#

#-----------------------------------------------------------------------------#
def exists_exec(runfile, name):
    if runfile and isfile(runfile) and access(runfile, X_OK) :
        return runfile
    elif any([isfile(join(path, name)) and 
            access(join(path, name), X_OK) 
            for path in environ["PATH"].split(pathsep)]) :
        return name
    else :
        print("Please install external dependency/package - " + name)
        return None
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx#

#-----------------------------------------------------------------------------#
class Run_planner:
    """
    Container to run the planner
    """
    # store
    config = None
    planner_instance = None
    solved = False

    # process
    def __init__(self, config: dict):
        self.config             =   config
        with time_taken('LAPKT_ALL_TASKS') :
            self._spawn_container(config['planner']['value'])
            self._configure_planner()
            with time_taken('LAPKT_PARSE_GROUND_TASK') :
                self._load_problem()
            with time_taken('LAPKT_SOLVE_TASK') :
                self._exec()
            if config.get('validate', None) and config['validate']['value'] :
                retval = validate_plan(self.config['domain']['value'],
                    self.config['problem']['value'],
                    self.config['plan_file']['value'])
                if retval == -1 :
                    print("Plan was not validated, since 'validate'"+
                            " binary was not found")
                else :
                    self.solved = not retval
                    out_status = "passed" if self.solved else "failed"
                    print ("'validate' check : Plan "+ out_status)

    def _load_problem(self) :
        """
        load problem from pddl files
        """
        if self.config['lapkt_instance_generator']['value'] == 'Tarski' :
            from ..tarski import ground_generate_task as process_task
        elif self.config['lapkt_instance_generator']['value'] == 'FF' :
            try :
                from ..ff import gen_problem_description as process_task
            except :
                print('FF Translate is not installed!')
                exit()
        elif self.config['lapkt_instance_generator']['value'] == 'FD' :
            try :
                from ..fd import default as process_task
            except :
                print('FD Translate is not installed!')
                exit()
        else :
            # We can add options for procedurally generated problems here
            raise ValueError("The value doesn't match supported parsers - Tarski/FF/FD")

        if self.config['lapkt_instance_generator']['value'] == 'FF' :
            process_task(self.config['domain']['value'], 
                self.config['problem']['value'], self.planner_instance,
                self.planner_instance.ignore_action_costs, False)
        elif self.config['lapkt_instance_generator']['value'] in ['Tarski', 'FD'] :
            process_task(self.config['domain']['value'], 
                self.config['problem']['value'], self.planner_instance)
        else :
            # We can add handle  procedurally generated problems here
            pass
        print('#Actions:', self.planner_instance.num_actions())
        print('#Fluents:', self.planner_instance.num_atoms())
        return 0

    def _configure_planner(self) :
        """
        load configs into planner
        """
        print("Loading Planner Config...")
        for k, v in self.config.items() :
            if isinstance(v, dict) and v.get('var_name', None) :
                try :
                    getattr(self.planner_instance, v['var_name'])
                except :
                    raise ValueError('Check the variable name ',v['var_name'])
                setattr(self.planner_instance, v['var_name'],v['value'])
                print('"'+v['var_name']+'"', "value set to: ", getattr(self.planner_instance, v['var_name']))
            elif isinstance(v, dict) and v.get('func_name', None) :
                try :
                    getattr(self.planner_instance, v['func_name'])(v['value'])
                except :
                    raise ValueError('Check the setter name ',v['func_name'])
        return 0

    def _exec(self) :
        """
        run planner
        """
        self.planner_instance.setup(bool(not (self.config.get('no_match_tree', 
            None) and self.config['no_match_tree']['value'])))
        self.planner_instance.solve()
        return 0

    def _spawn_container(self, name) :
        """
        Used to spawn a planner type object
        """
        try :
            self.planner_instance   =   getattr(planner, name)()
        except :
            raise ValueError("Either planner name, " +name+
                    ", is wrong or lapkt didn't load it")
        return 0
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx#


#-----------------------------------------------------------------------------#
def validate_plan(domain_f, problem_f, plan_f) :
    """
    Used to validate the plan
    """
    validate = exists_exec(join(CWD, rel_validate_file), 'validate')
    if validate : 
        with time_taken('LAPKT_VALIDATE_SOL') :
            return  run([validate, domain_f, problem_f, plan_f]).returncode
    else :
        return -1
#xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx#
