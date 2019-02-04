from moduleElement import *

class Module(object):

    module_count = 0

    def __init__(self,ects,title,semester,grade=None):
        "constructor for class module"

        self.ects = ects
        self.grade = grade
        self.title = title
        self.semester = semester

        self.dates = []

        self.elements = []

        Module.module_count += 1


    def get_important_dates_overview(self):
        "prints all the important dates for a module"

        print("Important dates for {0:s}:".format(self.title))

        for kind,date in self.dates:
            print("\t{0:s} on {1:s}".format(kind,date))


    def set_grade(self,grade):
        "set the grade to a given value"

        self.grade = grade


    def add_module_element(self,other_class,date):
        "add a new module element to the elements list"

        obj = other_class(self)
        obj.add_important_date(date)
        self.elements.append((obj))

    def get_title(self):
        return self.title

    def get_grade(self):
        return self.grade

#########################################################################

class Course(Module):

    def __str__(self):
        return "Course: {0:s}".format(self.title)

#########################################################################

class Seminar(Module):

    def __init__(self,ects,title,semester,topic, grade=None):
        self.ects = ects
        self.title = title
        self.semester = semester
        self.topic = topic
        self.grade = grade

    
    def __str__(self):
        return "Seminar in Software Engineering under the topic: {0:s}".format(self.topic)

    def get_topic(self):
        return self.topic

#########################################################################

class Thesis(Module):

    def __init__(self,ects,title,semester,topic,research_group, grade=None):
        self.ects = ects
        self.title = title
        self.semester = semester
        self.topic = topic
        self.grade = grade
        self.research_group = research_group
    
    def __str__(self):
        return "Bachelor Thesis on the topic: {0:s} in the Research " \
               "Group {1:2}".format(self.topic, self.research_group)

    def get_topic(self):
        return self.topic

    def get_research_group(self):
        return self.research_group

#########################################################################

### test cases ###

info1 = Course(6,"Info 1",1)
info1.add_module_element(Midterm,"31.10.2017")
info1.add_module_element(FinalExam,"20.12.2017")
info1.get_important_dates_overview()
print(info1)
# expected output:
# Course: Info 1

math1 = Course(6, "Mathematik I", 1)
math1.add_module_element(Midterm,"18.12.2017")
math1.get_important_dates_overview()
# expected output:
# Important dates for Info 1:
#	Midterm on 31.10.2017
#	Final Exam on 20.12.2017
# Important dates for Mathematik I:
#	Midterm on 18.12.2017


print(Module.module_count)
# expected output: 2

thesis = Thesis(18,"Bachelor Thesis",6,"A promising research topic on Software Engineering","SEAL")
print(thesis)
# expected output:
# Bachelor Thesison the topic: A promising research topic on Software Engineering in the Research Group SEAL


sem = Seminar(3,"Seminar in Software Engineering",4,"A Seminar topic")
print(sem)
# print(thesis)
# expected output:
# Seminar in Software Engineering under the topic: A Seminar topic

info1.set_grade(6)
