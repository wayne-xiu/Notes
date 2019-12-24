# C++ Design Patterns

[toc]

## Programming Patterns

A design pattern is neither a static solution, nor is it an algorithm. A pattern is a way to describe and address by name a repeatable solution or approach to a common design problem, that is, a common way to solve a generic problem (how generic or specific that pattern is depends on how restricted the target goal is). Design patterns are useful as an abstraction over the implementation an a help at design stage.

OOD patterns typically show relationships and interactions between classes or objects without specifying the final application classes or objects that are involved.



## Creational Patterns

object creation mechanisms

### Builder

The Builder Creational Pattern is used to separate the construction of a complex object from its representation so that the same construction process can create different objects representations.

#### Problem

We want to construct a complex object, however we do not want to have a complex constructor member or one that would need many arguments.

#### Solution

Define an intermediate object whose member functions define the desired object part by part before the object is available to the client.

```c++
#include <iostream>
#include <string>
#include <memory>

using namespace std;

// "Product"
class Pizza {
public:
    void setDough (const string& dough) {
        m_dough = dough;
    }
    void setSauce (const string& sauce) {
        m_sauce = sauce;
    }
    void setTopping(const string& topping) {
        m_topping = topping;
    }
    void open() const {
        cout << "Pizza with " << m_dough << " dough, " << m_sauce << " sauce and "
             << m_topping << " topping. Mmm." << endl;
    }
private:
    string m_dough;
    string m_sauce;
    string m_topping;
};

//"Abstract Builder"
class PizzaBuilder {
public:
    virtual ~PizzaBuilder() = default;
    Pizza* getPizza() {
        return m_pizza.release();
    }
    void createNewPizzaProduct() {
        m_pizza = make_unique<Pizza>();
    }
    virtual void buildDough() = 0;
    virtual void buildSauce() = 0;
    virtual void buildTopping() = 0;
protected:
    unique_ptr<Pizza> m_pizza;
};


class HawaiianPizzaBuilder: public PizzaBuilder {
public:
    virtual ~HawaiianPizzaBuilder() = default;
    virtual void buildDough() override {
        m_pizza->setDough("cross");
    }
    virtual void buildSauce() override {
        m_pizza->setSauce("mild");
    }
    virtual void buildTopping() override {
        m_pizza->setTopping("ham+pineapple");
    }
};
class SpicyPizzaBuilder: public PizzaBuilder {
public:
    virtual ~SpicyPizzaBuilder() = default;
    virtual void buildDough() override {
        m_pizza->setDough("pan baked");
    }
    virtual void buildSauce() override {
        m_pizza->setSauce("hot");
    }
    virtual void buildTopping() override {
        m_pizza->setTopping("pepperoni+salami");
    }
};


class Cook {
public:
    void openPizza() {
        m_pizzaBuilder->getPizza()->open();
    }
    void makePizza(PizzaBuilder* pb) {
        m_pizzaBuilder = pb;
        m_pizzaBuilder->createNewPizzaProduct();
        m_pizzaBuilder->buildDough();
        m_pizzaBuilder->buildSauce();
        m_pizzaBuilder->buildTopping();
    }
private:
    PizzaBuilder* m_pizzaBuilder;
};

int main()
{
    Cook cook;
    HawaiianPizzaBuilder hawaiianPizzaBuilder;
    SpicyPizzaBuilder spicyPizzaBuilder;

    cook.makePizza(&hawaiianPizzaBuilder);
    cook.openPizza();

    cook.makePizza(&spicyPizzaBuilder);
    cook.openPizza();

    return 0;
}
```



Console output: Pizza with cross dough, mild sauce and ham+pineapple topping. Mmm.
Pizza with pan baked dough, hot sauce and pepperoni+salami topping. Mmm.

### Factory

A utility class that creates an instance of a class from a family of derived classes.

### Abstract Factory

A utility class that creates an instance of several families of classes. It can also return a factory of a certain group.

The Factory Design Pattern is useful in a situation that requires the creation of many different types of objects, all derived from a common base type. The Factory Method defines a method for creating the objects, which subclasses can then override to specify the derived type that will be created. Thus, at run time, the Factory Method can be passed a description of a desired object (e.g. a sting read from user input) and return a base class pointer to a new instance of that object. The pattern works best when a well-designed interface is used for the base class, so there is no need to cast the returned object.

#### Problem

We want to decide at run time what object is to be created based on some configuration or application parameter.

#### Solution

Define an interface for creating an object, but let subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses.

Everyone who uses the factory will only care about the interface, which should remain consistent throughout the life of the application.

```c++
#include <stdexcept>
#include <iostream>
#include <memory>

using namespace std;

// 2: Abstract Factory
class Pizza {
public:
    virtual int getPrice() const = 0;
    virtual ~Pizza() = default;
};
class HamAndMushroomPizza: public Pizza {
public:
    virtual int getPrice() const override { return 850; }
    virtual ~HamAndMushroomPizza() {}
};
class DeluxePizza: public Pizza {
public:
    virtual int getPrice() const override { return 1050; }
    virtual ~DeluxePizza() = default;
};
class HawaiianPizza: public Pizza {
public:
    virtual int getPrice() const override { return 1150; }
    virtual ~HawaiianPizza() = default;
};

class PizzaFactory {
public:
    enum PizzaType {
        HamMushroom,
        Deluxe,
        Hawaiian
    };
    static unique_ptr<Pizza> createPizza(PizzaType pizzaType) {
        switch(pizzaType) {
        case HamMushroom: return make_unique<HamAndMushroomPizza>();
        case Deluxe: return make_unique<DeluxePizza>();
        case Hawaiian: return make_unique<HawaiianPizza>();
        }
        throw "invalid pizza type";
    }
};
void pizza_information(PizzaFactory::PizzaType pizzatype) {
    unique_ptr<Pizza> pizza = PizzaFactory::createPizza(pizzatype);
    cout << "Pirce of " << pizzatype << " is " << pizza->getPrice() << endl;
}


int main()
{
    // 2: Abstract Factory
    pizza_information(PizzaFactory::HamMushroom);
    pizza_information(PizzaFactory::Deluxe);
    pizza_information(PizzaFactory::Hawaiian);

    return 0;
}
```



### Prototype

A prototype pattern is used when the type of objects to create is determined by a prototypical instance, which is cloned to produce new object. This pattern is used, for example, when the inherent cost of creating a new object in the standard way (e.g. using new) is prohibitively expensive for a given application.

#### Implementation

Declare an abstract base class that specifies a pure virtual clone() method. Any class that needs a "polymorphic constructor" capability derives itself from the abstract base class, and implements the clone() operation

```c++

```

The client code first invokes the factory method. This factory method, depending on the parameter, finds out the concrete class. On this concrete class, the clone() method is called and the object is returned by the factory method.

### Singleton

## Structural Patterns

### Adapter

### Bridge

### Composite

### Decorator

### Facade

### Flyweight

### Proxy

### Curiously Recurring Template

### Interface-based Programming (IBP)

## Behavioral Patterns

### Chain of Responsibility

### Command

### Interpreter

### Iterator

### Mediator

### Memento

### Observer

### State

### Strategy

### Template Method

### Visitor

### Model-View-Controller (MVC)

