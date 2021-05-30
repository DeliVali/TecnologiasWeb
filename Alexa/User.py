class User:
    name="User"
    age=0

    def __init__(self, name, age):
        super().__init__(name, age)
        self.name = name
        self.age = age
        

    def setName(self,name):
        self.name=name

    def getName(self):
        return self.name

    def setAge(self,age):
        self.age=age

    def getAge(self):
        return self.age

    
    