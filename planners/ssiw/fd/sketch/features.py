
class Tree(object):
    """
    Inner nodes in the tree are dl elements.
    Leafs in the tree are predicate names.
    """
    def __init__(self, name="", children=[], depth=0):
        assert isinstance(name, str)
        assert isinstance(children, list)
        # check if feature must be evaluated in goal
        if len(name) >= 2 and name[len(name) - 2:] == "_g":
            self.goal = True
            self.predicate = name[:-2]
        else:
            self.goal = False
            self.predicate = name
        self.name = name
        self.children = children
        self.depth = depth

    def is_leaf(self):
        return self.children == []


    def __iter__(self):
        for child in self.children:
            for node in child:
                yield node
        yield self


    def __str__(self):
        return self.depth * "  " + self.name + "\n" + "".join([child.__str__() for child in self.children])



class Feature(object):
    def __init__(self, name, dl_sentence):
        self.name = name
        self.dl_sentence = dl_sentence
        self.predicates = []
        self.dl_tree = self._parse_dl_sentence(dl_sentence)


    def _parse_dl_sentence(self, dl_sentence, depth=0):
        if isinstance(dl_sentence, list):
            assert len(dl_sentence) >= 2
            # inner node
            children = []
            for child in dl_sentence[1:]:
                children.append(self._parse_dl_sentence(child, depth+1))
            return Tree(dl_sentence[0], children, depth)
        else:
            # leaf
            self.predicates.append(dl_sentence)
            return Tree(dl_sentence, [], depth)


    def __str__(self):
        return self.name + "\n" + str(self.dl_tree)
