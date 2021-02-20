

class Rule(object):
    def __init__(self, name, precondition, effect):
        self.name = name
        self.precondition = self._parse_precondition(precondition)
        self.effect = self._parse_effect(effect)


    def _parse_precondition(self, precondition):
        if precondition[0] == "and":
            return precondition[1:]
        else:
            return precondition


    def _parse_effect(self, effect):
        if effect[0] == "and":
            return effect[1:]
        else:
            return effect


    def __str__(self):
        return self.name + ": C=" + str(self.precondition) + " E=" + str(self.effect)
