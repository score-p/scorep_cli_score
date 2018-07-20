import operator


class Region:

    def __init__(self, type, processes, demangled_name, mangled_name):
        self.type = type
        self.processes = processes
        self.demangled_name = demangled_name
        self.mangled_name = mangled_name
        self.visits = 0
        self.hits = 0
        self.time_excl = 0
        self.time_incl = 0
        self.filtered = False
        self.exclude = False

    def add(self, visits, hits, time_incl, time_excl, process):
        self.visits += visits
        self.hits += hits
        self.time_excl += time_excl
        self.time_incl += time_incl


class RegionLoad:

    def __init__(self, exclude):
        self.exclude = exclude
        self.demangled_name = ''
        self.mangled_name = ''


class FilterBy:

    def __init__(self):
        self.attr = ''
        self.operator = ''
        self.value = -1
        self.active = False

        self.operators = {'<': operator.lt,
                          '<=': operator.le,
                          '>': operator.gt,
                          '>=': operator.ge,
                          '=': operator.eq,
                          '!=': operator.ne,
                          'lt': operator.lt,
                          'le': operator.le,
                          'gt': operator.gt,
                          'ge': operator.ge,
                          'eq': operator.eq,
                          'ne': operator.ne,
                          }

    def set(self, operator, attr, value):
        self.operator = operator
        self.attr = attr
        self.value = value
        self.active = True

    def get(self):
        return self.operators[self.operator], self.attr, self.value

    def reset(self):
        self.operator = ''
        self.attr = ''
        self.value = -1
        self.active = False
