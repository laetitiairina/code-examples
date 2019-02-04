from module import *
from moduleElement import *

class Student(object):

    def __init__(self, name):
        self.name = name
        self.modules = []
        self.grades = {}

    def add_module(self, module):
        self.modules.append(module)
        self.grades[module] = Module.get_grade(module)

    def get_list_modules(self):
        print("Modules of Student {0:s}:".format(self.name))
        for m in self.modules:
            print(Module.get_title(m))

    def get_grades(self):
        print("Grades of Student {0:s}:".format(self.name))
        for key, value in self.grades.items():
            print("{0:s}: {1:d}".format(Module.get_title(key), value))


### test cases ###

me = Student("FirstName LastName")
me.add_module(info1)
me.get_list_modules()
# expected output:
# Modules of Student FirstName LastName:
#	Info 1

me.get_grades()
# expected output:
# Grades of Student FirstName LastName:
#	Info 1: 6
