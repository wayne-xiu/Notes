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
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class Record {
public:
    virtual ~Record() {}
    virtual void print() = 0;
    virtual unique_ptr<Record> clone() = 0;
};

class CarRecord: public Record {
private:
    string m_carName;
    int m_ID;
public:
    CarRecord(string carName, int ID): m_carName(carName), m_ID(ID) {}
    void print() override {
        cout << "Car Record" << endl
             << "Name: " << m_carName << endl
             << "Number: " << m_ID << endl;
    }
    unique_ptr<Record> clone() override {
        return make_unique<CarRecord>(*this);
    }
};
class BikeRecord: public Record {
private:
    string m_bikeName;
    int m_ID;
public:
    BikeRecord(string bikeName, int ID): m_bikeName(bikeName), m_ID(ID) {}
    void print() override {
        cout << "Bike Record" << endl
             << "Name: " << m_bikeName << endl
             << "Number: " << m_ID << endl;
    }
    unique_ptr<Record> clone() override {
        return make_unique<BikeRecord>(*this);
    }
};
class PersonRecord: public Record {
private:
    string m_personName;
    int m_age;
public:
    PersonRecord(string personName, int age): m_personName(personName), m_age(age) {}
    void print() override {
        cout << "Person Record" << endl
             << "Name: " << m_personName << endl
             << "Age: " << m_age << endl;
    }
    unique_ptr<Record> clone() override {
        return make_unique<PersonRecord>(*this);
    }
};

enum RecordType {
    CAR,
    BIKE,
    PERSON
};
class RecordFactory {
private:
    unordered_map<RecordType, unique_ptr<Record>, hash<int> > m_records;
public:
    RecordFactory() {
        m_records[CAR] = make_unique<CarRecord>("Ferrari", 5050);
        m_records[BIKE] = make_unique<BikeRecord>("Yamaha", 2525);
        m_records[PERSON] = make_unique<PersonRecord>("Tom", 25);
    }
    unique_ptr<Record> createRecord(RecordType recordType) {
        return m_records[recordType]->clone();
    }
};

int main()
{
    RecordFactory recordFactory;
    auto record = recordFactory.createRecord(CAR);
    record->print();
    record = recordFactory.createRecord(BIKE);
    record->print();
    record = recordFactory.createRecord(PERSON);
    record->print();

    return 0;
}
```

The client code first invokes the factory method. This factory method, depending on the parameter, finds out the concrete class. On this concrete class, the clone() method is called and the object is returned by the factory method.

The client, instead of writing code that invokes the new operator on a hard-wired class name, calls the clone() member function on the prototype, class a factory member function with a parameter designating the particular concrete derived class desired, or invokes the clone() member function through some mechanism provided by another design pattern.

### Singleton

The Singleton pattern ensures that a class has only one instance and provides a global point of access to that instance.

#### Check list

- Define a private static attribute in the "single instance" class
- Define a public static accessor function in the class
- Do "lazy initialization" (creation on first use) in the accessor function
- Define all constructors to be protected or private
- Clients may only use the accessor function to manipulate the Singleton

Like a global variable, the Singleton exists outside of the scope of any functions. Traditional implementation uses a static member function of the Singleton class, which will create a single instance of the Singleton class on the first call, and forever return that instance.

With use of the singleton, the first time the object is accessed, the object will also be created. You now have an object which will always exist, in relation to being used, and will never exist if never used.

```c++
/* Place holder for thread synchronization mutex */
class Mutex {
    // placeholder for code to create, use and free a mutex
};
/* Place holder for thread ssynchronization lock */
class Lock {
public:
    Lock(Mutex& m): mutex(m) { /* placeholder code to acquire the mutex */}
    ~Lock() { /* placeholder code to release the mutex */ }
private:
    Mutex& mutex;
};
class Singleton {
public:
    static Singleton* GetInstance();
    int a;
    ~Singleton() { cout << "In Destructor" << endl; }
private:
    Singleton(int _a): a(_a) { cout << "In Constructor" << endl; }
    static Mutex mutex;
    // not defined to prevent copying
    Singleton(const Singleton& other);
    Singleton& operator= (const Singleton& other);
};

Mutex Singleton::mutex;

Singleton* Singleton::GetInstance() {
    Lock lock(mutex);
    cout << "Get Instance" << endl;

    static Singleton inst(1);
    return &inst;
}

int main()
{
    Singleton* singleton = Singleton::GetInstance();
    cout << "The value of the singleton: " << singleton->a << endl;
    
    Singleton* singleton2 = Singleton::GetInstance();
    cout << "The value of the singleton: " << singleton2->a << endl;

    return 0;
}
```



## Structural Patterns

### Adapter

Convert the interface of a class into another interface that clients expect. Adapter lets classes work together that couldn't otherwise because of incompatible interfaces.

```c++
// Adapter
class Hindu {  // Abstract target
public:
    virtual ~Hindu() = default;
    virtual void performsHinduRitual() const = 0;
};
class HinduFemale: public Hindu {  // Concrete target
public:
    virtual void performsHinduRitual() const override {
        cout << "Hindu girl performs Hindu ritual." << endl;
    }
};

class Muslim {  // Abstract adapter
public:
    virtual ~Muslim() = default;
    virtual void performMuslimRitual() const = 0;
};
class MuslimFemale: public Muslim {  // Concrete adapter
public:
    virtual void performMuslimRitual() const override {
        cout << "Muslim gril performs Muslim ritual." << endl;
    }
};

class HinduRitual {
public:
    void carryOutRitual (Hindu* hindu) {
        cout << "On with the Hindu rituals!" << endl;
        hindu->performsHinduRitual();
    }
};

class HinduAdapter: public Hindu {  // Adapter
private:
    Muslim* muslim;
public:
    HinduAdapter(Muslim* m): muslim(m) {}
    virtual void performsHinduRitual() const override {
        muslim->performMuslimRitual();
    }
};

int main()
{
    HinduFemale* hinduGirl = new HinduFemale;
    MuslimFemale* muslimGirl = new MuslimFemale;

    HinduRitual hinduRitual;
    // hinduRitual.carryOutRitual(muslimGirl);  // won't compile
    HinduAdapter* adaptedMuslim = new HinduAdapter(muslimGirl);

    hinduRitual.carryOutRitual(hinduGirl);
    hinduRitual.carryOutRitual(adaptedMuslim);

    delete adaptedMuslim;
    delete muslimGirl;
    delete hinduGirl;

    return 0;
}
```



### Bridge

The Bridge Pattern is used to separate out the interface from its implementation.

```c++
// 7: Bridge
// Implementor
class DrawingAPI {
public:
    virtual void drawCircle(double x, double y, double radius) = 0;
    virtual ~DrawingAPI() {}
};
// Concrete implementorA
class DrawingAPI1: public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        cout << "API1.circle at " << x << ":" << y << " " << radius << endl;
    }
};
class DrawingAPI2: public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        cout << "API2.circle at " << x << ":" << y << " " << radius << endl;
    }
};

// Abstraction
class Shape {
public:
    virtual ~Shape(){}
    virtual void draw() = 0;
    virtual void resizeByPercentage(double pct) = 0;
};
// Refined Abstraction
class CircleShape: public Shape {
public:
    CircleShape(double x, double y, double radius, DrawingAPI* drawingAPI):
        m_x(x), m_y(y), m_radius(radius), m_drawingAPI(drawingAPI) {}
    void draw() override {
        m_drawingAPI->drawCircle(m_x, m_y, m_radius);
    }
    void resizeByPercentage(double pct) override {
        m_radius *= pct;
    }
private:
    double m_x, m_y, m_radius;
    DrawingAPI* m_drawingAPI;
};


int main()
{
    CircleShape circle1(1, 2, 3, new DrawingAPI1());
    CircleShape circle2(5, 7, 11, new DrawingAPI2());
    circle1.resizeByPercentage(2.5);
    circle2.resizeByPercentage(2.5);
    circle1.draw();
    circle2.draw();

    return 0;
}

```



### Composite

Composite lets clients treat individual objects and compositions of objects uniformly. The composite pattern can represent both the conditions. In this pattern, one can develop tree structures for representing part-whole hierarchies.

```c++
#include <iostream>
#include <memory>
#include <algorithm>

// 8: Composite
class Graphic {
public:
    virtual void print() const = 0;
    virtual ~Graphic() {}
};
class Ellipse: public Graphic {
public:
    void print() const override {
        cout << "Ellipse with ID: " << m_id << endl;
    }
    Ellipse(int id): m_id(id) {}
private:
    int m_id;
};
class CompositeGraphic: public Graphic {
public:
    void print() const override {
        for (Graphic* a: graphicList_)
            a->print();
    }
    void add(Graphic* aGraphic) {
        graphicList_.push_back(aGraphic);
    }
private:
    vector<Graphic*> graphicList_;
};

int main()
{
    const unique_ptr<Ellipse> ellipse1(new Ellipse(1));
    const unique_ptr<Ellipse> ellipse2(new Ellipse(2));
    const unique_ptr<Ellipse> ellipse3(new Ellipse(3));
    const unique_ptr<Ellipse> ellipse4(new Ellipse(4));

    const unique_ptr<CompositeGraphic> graphic(new CompositeGraphic());
    const unique_ptr<CompositeGraphic> graphic1(new CompositeGraphic());
    const unique_ptr<CompositeGraphic> graphic2(new CompositeGraphic());

    graphic1->add(ellipse1.get());
    graphic1->add(ellipse2.get());
    graphic1->add(ellipse3.get());

    graphic2->add(ellipse4.get());

    // composite graphic of composite graphic
    graphic->add(graphic1.get());
    graphic->add(graphic2.get());

    graphic->print();

    return 0;
}
```



### Decorator

The decorator pattern helps to attach additional behavior or responsibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality. This is also called "wrapper".

```c++
// Decorator
class Car {  // Abstract base class
protected:
    string _str;
public:
    Car() { _str = "Unknown Car"; }
    virtual string getDescription() { return _str; }
    virtual double getCost() = 0;
    virtual ~Car() { cout << "~Car()\n"; }
};
class OptionsDecorator: public Car {  // Decorator base class
public:
    virtual string getDescription() = 0;
    virtual ~OptionsDecorator() { cout << "~OptionsDecorator()\n"; }
};

class CarModel1: public Car {
public:
    CarModel1() { _str = "CarModel1"; }
    virtual double getCost() override {
        return 31000.23;
    }
    ~CarModel1() { cout << "~CarModel1()\n"; }
};

class Navigation: public OptionsDecorator {
private:
    Car* _b;
public:
    Navigation(Car* b): _b(b) {}
    string getDescription() override {
        return _b->getDescription() + ", Navigation";
    }
    double getCost() {
        return 300.56+_b->getCost();
    }
    ~Navigation() {
        cout << "~Navigation()\n";
        delete _b;
    }
};
class PremiumSoundSystem: public OptionsDecorator {
private:
    Car* _b;
public:
    PremiumSoundSystem(Car* b): _b(b) {}
    string getDescription() override {
        return _b->getDescription() + ", PremiumSoundSystem";
    }
    double getCost() {
        return 0.3+_b->getCost();
    }
    ~PremiumSoundSystem() {
        cout << "~PremiumSoundSystem()\n";
        delete _b;
    }
};
class ManualTransmission: public OptionsDecorator {
private:
    Car* _b;
public:
    ManualTransmission(Car* b): _b(b) {}
    string getDescription() override {
        return _b->getDescription() + ", ManualTransmission";
    }
    double getCost() {
        return 0.3+_b->getCost();
    }
    ~ManualTransmission() {
        cout << "~ManualTransmission()\n";
        delete _b;
    }
};

int main()
{
    Car* b = new CarModel1();
    cout << "Base model of " << b->getDescription() << " cost $" << b->getCost() << endl;

    b = new Navigation(b);
    cout << b->getDescription() << " will cost you $" << b->getCost() << endl;
    b = new PremiumSoundSystem(b);
    cout << b->getDescription() << " will cost you $" << b->getCost() << endl;
    b = new ManualTransmission(b);
    cout << b->getDescription() << " will cost you $" << b->getCost() << endl;

    delete b;

    return 0;
}

```



### Facade

The Facade Pattern hides the complexities of the system by providing an interface to the client from where the client can access the system on a unified interface. Faced defines a higher-level interface that makes the subsystem easier to use. For instance making one class method perform a complex process by calling several other classes.

```c++
// Facade
class Alarm {
public:
    void alarmOn() {
        cout << "Alarm is on and house is secured" << endl;
    }
    void alarmOff() {
        cout << "Alarm is off and you can go into the house" << endl;
    }
};
class Ac {
public:
    void acOn() {
        cout << "AC is on" << endl;
    }
    void acOff() {
        cout << "AC is off" << endl;
    }
};
class Tv {
public:
    void tvOn() {
        cout << "TV is on" << endl;
    }
    void tvOff() {
        cout << "TV is off" << endl;
    }
};

class HouseFacde {
private:
    Alarm alarm;
    Ac ac;
    Tv tv;
public:
    void goToWork() {
        ac.acOff();
        tv.tvOff();
        alarm.alarmOn();
    }
    void comeHome() {
        alarm.alarmOff();
        ac.acOn();
        tv.tvOn();
    }
};

int main()
{
    HouseFacde hf;
    hf.goToWork();
    hf.comeHome();

    return 0;
}

```



### Flyweight

Flyweight (i.e. very light) pattern to save memory (basically) by sharing properties of objects. Move shareable properties out of the objects to some external data structure and provide each object with the link to that data structure.

```c++
// Flyweight (very light)
#define NUMBER_OF_SAME_TYPE_CHARS = 3;

// Actual flyweight objects class (declaration)
class FlyweightCharacter;
class FlyweightCharacterAbstractBuilder {
    FlyweightCharacterAbstractBuilder() {}
    ~FlyweightCharacterAbstractBuilder() {}
public:
    static vector<float> fontSize;
    static vector<string> fontNames;

    static void setFontsAndNames();
    static FlyweightCharacter createFlyweightCharacter(unsigned short fontSizeIndex,
                                                       unsigned short fontNameIndex,
                                                       unsigned short positionInStream);
};
vector<float> FlyweightCharacterAbstractBuilder::fontSize(3);
vector<string> FlyweightCharacterAbstractBuilder::fontNames(3);
void FlyweightCharacterAbstractBuilder::setFontsAndNames() {
    fontSize[0] = 1.0;
    fontSize[1] = 2.0;
    fontSize[2] = 3.0;

    fontNames[0] = "first_font";
    fontNames[1] = "second_font";
    fontNames[2] = "third_font";
}

class FlyweightCharacter {
    unsigned short fontSizeIndex;  // index instead of actual font size
    unsigned short fontNameIndex;
    unsigned positionInStream;
public:
    FlyweightCharacter(unsigned short fsIndex, unsigned short fnIndex, unsigned short posInStream):
        fontSizeIndex(fsIndex), fontNameIndex(fnIndex), positionInStream(posInStream) {}
    void print() {
        cout << "Font Size: " << FlyweightCharacterAbstractBuilder::fontSize[fontSizeIndex]
                << ", font Name: " << FlyweightCharacterAbstractBuilder::fontNames[fontNameIndex]
                   << ", character stream position: " << positionInStream << endl;
    }
    ~FlyweightCharacter() {}
};

FlyweightCharacter FlyweightCharacterAbstractBuilder::createFlyweightCharacter(unsigned short fontSizeIndex, unsigned short fontNameIndex, unsigned short positionInStream) {
    FlyweightCharacter fc(fontSizeIndex, fontNameIndex, positionInStream);

    return fc;
}

int main()
{
    vector<FlyweightCharacter> chars;
    FlyweightCharacterAbstractBuilder::setFontsAndNames();
    // unsigned short limit = NUMBER_OF_SAME_TYPE_CHARS;
    unsigned short limit = 3;

    for (unsigned short i = 0; i < limit; ++i) {
        chars.push_back(FlyweightCharacterAbstractBuilder::createFlyweightCharacter(0, 0, i));
        chars.push_back(FlyweightCharacterAbstractBuilder::createFlyweightCharacter(1, 1, i+1*limit));
        chars.push_back(FlyweightCharacterAbstractBuilder::createFlyweightCharacter(2, 2, i+2*limit));
    }

    for (unsigned short i = 0; i < limit; ++i)
        chars[i].print();

    return 0;
}
```



### Proxy

The Proxy Pattern will provide an object a surrogate or placeholder for another object to control access to it. It is used when you need to represent a complex object with a simpler one. If creation of an object is expensive, it can be postponed until the very need arises and meanwhile a simpler object can serve as a placeholder. This placeholder object is called the "Proxy" for the complex object.

```c++
// Proxy
class ICar {
public:
    virtual ~ICar() { cout << "ICar destructor!" << endl; }
    virtual void DriveCar() = 0;
};
class DICar: public ICar {
public:
    void DriveCar() override {
        cout << "Car has been driven!" << endl;
    }
};

class ProxyCar: public ICar {
public:
    ProxyCar(int driver_age): driver_age_(driver_age) {}
    void DriveCar() override {
        if (driver_age_ > 16)
            real_car_->DriveCar();
        else
            cout << "Sorry, the driver is too young to drive." << endl;
    }
private:
    unique_ptr<ICar> real_car_ = make_unique<DICar>();
    int driver_age_;
};

int main() {
    unique_ptr<ICar> car = std::make_unique<ProxyCar>(16);
    car->DriveCar();

    car = std::make_unique<ProxyCar>(25);
    car->DriveCar();

    return 0;
}
```



### Curiously Recurring Template

### Interface-based Programming (IBP)

Interface-based programming is closely related with Modular Programming and Object-Oriented Programming. It defines the application as a collection of inter-coupled modules (interconnected and which plug into each other via interface). Modules can be unplugged, replaced, or ungraded, without the need of compromising the contents of other modules.

IBP increases the modularity of the application and the total system complexity is greatly reduced. The entire system is viewed as Components and the interfaces that helps them to co-act.

This is particularly convenient when third parties develop additional components for the established system. They just have to develop components that satisfy the interface specified by the parent application vendor.

## Behavioral Patterns

### Chain of Responsibility

Chain of Responsibility has the intent to avoid coupling the sender of a request to its receiver by giving more than one object a chance to handle the request.

```c++
// Chain of Responsibility
class Handler {
protected:
    Handler* next;
public:
    Handler() {
        next = nullptr;
    }
    virtual ~Handler() {}
    virtual void request(int value) = 0;
    void setNextHandler(Handler* nextInLine) {
        next = nextInLine;
    }
};
class SpecialHandler: public Handler {
private:
    int myLimit;
    int myId;
public:
    SpecialHandler(int limit, int id): myLimit(limit), myId(id) { }
    ~SpecialHandler() {}
    void request(int value) override {
        if (value < myLimit)
            cout << "Handler " << myId << " handled the request with a limit of " <<
                    myLimit << endl;
        else if (next != nullptr)
            next->request(value);
        else
            cout << "Sorry, I am the last handler (" << myId << ") and I can't handle the request." << endl;
    }
};

int main() {
    Handler* h1 = new SpecialHandler(10, 1);
    Handler* h2 = new SpecialHandler(20, 2);
    Handler* h3 = new SpecialHandler(30, 3);

    h1->setNextHandler(h2);
    h2->setNextHandler(h3);

    h1->request(18);
    h1->request(40);

    delete h1;
    delete h2;
    delete h3;
}
```



### Command

Command pattern is an Object behavior pattern that decouples sender and receiver by encapsulating a request as an object, thereby letting you parameterize clients with different requests, queue or log requests, and support undo-able operations. It can also be thought as an object oriented equivalent of call back method.

Call back: It is a function that is registered to be called at later point of time based on user actions.

```c++
// Command
// the command interface
class Command {
public:
    virtual void execute() = 0;
};
// Receiver class
class Light {
public:
    Light() {}
    void turnOn() {
        cout << "The light is on" << endl;
    }
    void turnOff() {
        cout << "The light is off" << endl;
    }
};
// the command for turning on the light
class FlipUpCommand: public Command {
public:
    FlipUpCommand(Light& light): theLight(light) {}
    virtual void execute() override {
        theLight.turnOn();
    }
private:
    Light& theLight;
};
class FlipDownCommand: public Command {
public:
    FlipDownCommand(Light& light): theLight(light) {}
    virtual void execute() override {
        theLight.turnOff();
    }
private:
    Light& theLight;
};
class Switch {
public:
    Switch(Command& flipUpCommand, Command& flipDownCommand): flipUpCommand(flipUpCommand),
        flipDownCommand(flipDownCommand) { }
    void flipUp() {
        flipUpCommand.execute();
    }
    void flipDown() {
        flipDownCommand.execute();
    }
private:
    Command& flipUpCommand;
    Command& flipDownCommand;
};

int main()
{
    Light lamp;
    FlipUpCommand switchUp(lamp);
    FlipDownCommand switchDown(lamp);

    Switch s(switchUp, switchDown);
    s.flipUp();
    s.flipDown();

    return 0;
}

```



### Interpreter

Given a language, define a representation for its grammar along with an interpreter that uses the representation to interpret sentences in the language. -TODO

```c++
// Interpreter
namespace wikibooks_design_patterns {
    // based on Java sample around here
typedef std::string String;
struct Expression;
typedef std::map<String, Expression*> Map;
typedef std::list<Expression*> Stack;

struct Expression {
public:
    virtual int interpret(Map variables) = 0;
    virtual ~Expression() {}
};

class Number: public Expression {
private:
    int number;
public:
    Number(int number): number(number) {}
    int interpret(Map variables) override {
        return number;
    }
};

class Plus: public Expression {
private:
    Expression* leftOperand;
    Expression* rightOperand;
public:
    Plus(Expression* left, Expression* right) {
        leftOperand = left;
        rightOperand = right;
    }
    ~Plus() {
        delete leftOperand;
        delete rightOperand;
    }
    int interpret(Map variables) override {
        return leftOperand->interpret(variables) + rightOperand->interpret(variables);
    }
};

class Minus: public Expression {
private:
    Expression* leftOperand;
    Expression* rightOperand;
public:
    Minus(Expression* left, Expression* right) {
        leftOperand = left;
        rightOperand = right;
    }
    int interpret(Map variables) override {
        return leftOperand->interpret(variables) - rightOperand->interpret(variables);
    }
};

class Variable: public Expression {
private:
    String name;
public:
    Variable(String name): name(name) {}
    int interpret(Map variables) override {
        if (variables.end() == variables.find(name))
            return 0;
        return variables[name]->interpret(variables);
    }
};

// While the interpreter pattern does not address parsing, a parser is provided for completeness
class Evaluator: public Expression {
private:
    Expression* syntaxTree;
public:
    Evaluator(String expression) {
        Stack expressionStack;

        size_t last = 0;
        for (size_t next = 0; String::npos != last; last = (String::npos == next) ? next: (1+next)) {
            next = expression.find(' ', last);
            String token(expression.substr(last, (String::npos == next) ? (expression.length()-last): (next-last)));

            if (token == "+") {
                Expression* right = expressionStack.back();
                expressionStack.pop_back();
                Expression* left = expressionStack.back();
                expressionStack.pop_back();
                Expression* subExpression = new Plus(right, left);
                expressionStack.push_back(subExpression);
            }
            else if (token == "-") {
                // it's necessary to remove first the right operand from the stack
                Expression* right = expressionStack.back();
                expressionStack.pop_back();
                Expression* left = expressionStack.back();
                expressionStack.pop_back();
                Expression* subExpression = new Minus(left, right);
                expressionStack.push_back(subExpression);
            }
            else
                expressionStack.push_back(new Variable(token));
        }
        syntaxTree = expressionStack.back();
        expressionStack.pop_back();
    }
    ~Evaluator() {
        delete syntaxTree;
    }
    int interpret(Map context) override {
        return syntaxTree->interpret(context);
    }
};

}

int main()
{
    using namespace wikibooks_design_patterns;
    Evaluator sentence("w x z - +");
    static const int sequences[][3] = {{5, 10, 42}, {1, 3, 2}, {7, 9, -5}};
    for (size_t i = 0; sizeof(sequences)/sizeof(sequences[0]) > i; ++i) {
        Map variables;
        variables["w"] = new Number(sequences[i][0]);
        variables["x"] = new Number(sequences[i][1]);
        variables["z"] = new Number(sequences[i][2]);
        int result = sentence.interpret(variables);
        for (Map::iterator it = variables.begin(); variables.end()!=it; ++it)
            delete it->second;

        cout << "Interpreter result: " << result << endl;
    }

    return 0;
}
```



### Iterator

The basic idea of the iterator is that it permits the traversal of a container (like a pointer moving across an array). However, to get to the next element of a container, you need not know anything about how the container is constructed. This is the iterators job.

How would we traverse a linked list, when the memory is not contiguous?

```C++
// Iterator
class IteratorCannotMoveToNext{};  // Error class
class Node {
public:
    int mValue;
    Node* mNextNode;
    Node* mPrevNode;
    Node(): mValue(0), mNextNode(nullptr), mPrevNode(nullptr) {}
    Node(int value): mValue(value), mNextNode(nullptr), mPrevNode(nullptr) {}
};
class MyIntLList {
public:
    MyIntLList(): mSize(0) {}
    ~MyIntLList() {
        while(!Empty())
            pop_front();
    }
    int Size() const {
        return mSize;
    }
    bool Empty() {
        return mSize == 0;
    }
    void push_back(int value) {
        Node* newNode = new Node(value);
        newNode->mPrevNode = mTail;
        mTail->mNextNode = newNode;
        mTail = newNode;
        mSize++;
    }
    void pop_front() {
        if (Empty())
            return;
        Node* tempnode = mHead;
        mHead = mHead->mNextNode;
        delete tempnode;
        mSize--;
    }

    // iterator definition go here
    class Iterator {
    public:
        Iterator(Node* position): mCurrNode(position) {}
        // Prefix increment
        const Iterator& operator++ () {
            if (mCurrNode == nullptr  || mCurrNode->mNextNode == nullptr)
                throw IteratorCannotMoveToNext();
            mCurrNode = mCurrNode->mNextNode;
            return *this;
        }
        // Postfix increment
        Iterator operator++ (int) {
            Iterator tempItr = *this;
            ++(*this);
            return tempItr;
        }
        bool operator!= (const Iterator& rhs) {
            return !(this->mCurrNode == rhs.mCurrNode);
        }
        // Dereferencing operator returns the current node,
        // which should then be dereferenced for the int
        Node* operator* () {
            return mCurrNode;
        }
    private:
        Node* mCurrNode;
    };

    Iterator Begin() { return Iterator(mHead); }
    Iterator End() { return Iterator(nullptr); }

private:
    Node* mHead;
    Node* mTail;
    int mSize;
};

int main()  // TODO - fix issues
{
    MyIntLList myList;
    for (int i = 0; i < 10; ++i)
        myList.push_back(i);
    for (MyIntLList::Iterator it = myList.Begin(); it != myList.End(); ++it)
        (*it)->mValue += 42;

    return 0;
}

// TODO - Implementation of iterator design pattern with a generic template
```



### Mediator

Define an object that encapsulates how a set of objects interact. Mediator promotes loose coupling by keeping objects from referring to each other explicitly, and it lets you vary their interaction independently.

```c++
// Mediator (coordinating colleagues as middleware)
class MediatorInterface;
class ColleagueInterface {
private:
    string name;
public:
    ColleagueInterface(const string& newName): name(newName) {}
    string getName() const {
        return name;
    }
    virtual void sendMessage(const MediatorInterface&, const string&) const = 0;
    virtual void receiveMessage(const ColleagueInterface*, const string&) const = 0;
};

class Colleague: public ColleagueInterface {
public:
    using ColleagueInterface::ColleagueInterface;
    virtual void sendMessage(const MediatorInterface&, const string&) const override;
private:
    virtual void receiveMessage(const ColleagueInterface*, const string&) const override;
};

class MediatorInterface {
private:
    list<ColleagueInterface*> colleagueList;
public:
    const list<ColleagueInterface*>& getColleagueList() const {
        return colleagueList;
    }
    virtual void distributeMessage(const ColleagueInterface*, const string&) const = 0;
    virtual void registerColleague(ColleagueInterface* colleague) {
        colleagueList.emplace_back(colleague);
    }
};
class Mediator: public MediatorInterface {
    virtual void distributeMessage(const ColleagueInterface *, const string &) const override;
};

void Colleague::sendMessage(const MediatorInterface& mediator, const string& message) const {
    mediator.distributeMessage(this, message);
}
void Colleague::receiveMessage(const ColleagueInterface* sender, const string& message) const {
    cout << getName() << " received the message from " << sender->getName() << ": " << message << endl;
}
void Mediator::distributeMessage(const ColleagueInterface* sender, const string& message) const {
    for (const ColleagueInterface* x: getColleagueList()) {
        if (x != sender) // Do not send the message back to the sender
            x->receiveMessage(sender, message);
    }
}

int main()
{
    Colleague* bob = new Colleague("Bob"), *sam = new Colleague("Sam"), *frank = new Colleague("Frank"), *tom = new Colleague("Tom");
    Colleague* staff[] = {bob, sam, frank, tom};
    Mediator mediatorStaff, mediatorSamsBuddies;
    for (Colleague* x: staff) {
        mediatorStaff.registerColleague(x);
    }
    bob->sendMessage(mediatorStaff, "I'm quitting this job!");

    cout << endl;
    mediatorSamsBuddies.registerColleague(frank);
    mediatorSamsBuddies.registerColleague(tom);
    sam->sendMessage(mediatorSamsBuddies, "Hooray! He's gone! Let's go for a drink, guys!");

    return 0;
}

```



### Memento

Without violating encapsulation, the Memento Pattern will capture and externalize an object's internal state so that the object can be restored to this state later. Though the GoF uses friend as a way to implement this pattern it is not the best design. It can also be implemented using PIMPL (pointer to implementation or opaque pointer). Best use case is 'Undo-Redo' in an editor.

The Originator (the object to be saved) creates a snap-shot of itself as a Memento object, and passes that reference to the Caretaker object. The Caretaker object keeps the Memento until such as time as the Originator may want to revert to a previous state as recorded in the Memento object.

```c++
// Memento
const string NAME = "Object";

template <typename T>
string toString(const T& t) {
    stringstream ss;
    ss << t;
    return ss.str();
}

class Memento;
class Object {
private:
    int value;
    string name;
    double decimal;  // and suppose there are loads of other data members
public:
    Object(int newValue): value(newValue), name(NAME+toString(value)), decimal((float)value/100) {}
    void doubleValue() {
        value *= 2;
        name = NAME + toString(value);
        decimal = (float)value/100;
    }
    void increaseByONe() {
        value++;
        name = NAME + toString(value);
        decimal = (float)value/100;
    }
    int getValue() const {
        return value;
    }
    string getName() const {
        return name;
    }
    double getDecimal() const {
        return decimal;
    }
    Memento* createMemento() const;
    void reinstateMemento(Memento* mem);
};

class Memento {
private:
    Object object;
public:
    Memento(const Object& obj): object(obj) {}
    Object snapshot() const {
        return object;
    }
};

Memento* Object::createMemento() const {
    return new Memento(*this);
}
void Object::reinstateMemento(Memento *mem) {
    *this = mem->snapshot();
}

class Command {
private:
    typedef void (Object::*Action)();
    Object* receiver;
    Action action;
    static vector<Command*> commandList;
    static vector<Memento*> mementoList;
    static int numCommands;
    static int maxCommands;
public:
    Command(Object* newReceiver, Action newAction): receiver(newReceiver), action(newAction) {}
    virtual void execute() {
        if (mementoList.size() < numCommands + 1)
            mementoList.resize(numCommands+1);
        mementoList[numCommands] = receiver->createMemento();
        if (commandList.size() < numCommands + 1)
            commandList.resize(numCommands + 1);
        commandList[numCommands] = this;
        if (numCommands > maxCommands)
            maxCommands = numCommands;
        numCommands++;
        (receiver->*action)();
    }
    static void undo() {
        if (numCommands == 0) {
            cout << "There is nothing to undo at this point." << endl;
            return;
        }
        commandList[numCommands-1]->receiver->reinstateMemento(mementoList[numCommands-1]);
        numCommands--;
    }
    static void redo() {
        if (numCommands > maxCommands) {
            cout << "There is nothing to redo at this point." << endl;
            return;
        }
        Command* commandRedo = commandList[numCommands];
        (commandRedo->receiver->*(commandRedo->action))();
        numCommands++;
    }

};
vector<Command*> Command::commandList;
vector<Memento*> Command::mementoList;
int Command::numCommands = 0;
int Command::maxCommands = 0;

int main()
{
    int i;
    cout << "Plese enter an integer: ";
    cin >> i;
    Object* object = new Object(i);

    Command* commands[3];
    commands[1] = new Command(object, &Object::doubleValue);
    commands[2] = new Command(object, &Object::increaseByONe);

    cout << "0.Exit, 1.Double, 2.Increase by one, 3.Undo, 4.Redo: ";
    cin >> i;

    while (i != 0) {
        if (i == 3)
            Command::undo();
        else if (i == 4)
            Command::redo();
        else if (i > 0 && i <= 2)
            commands[i]->execute();
        else {
            cout << "Enter a proper choice: ";
            cin >> i;
            continue;
        }
        cout << "  " << object->getValue() << " " << object->getName() << " " << object->getDecimal() << endl;
        cout << "0.Exit, 1.Double, 2.Increase by one, 3.Undo, 4.Redo: ";
        cin >> i;
    }

    return 0;
}
```



### Observer

The Observer Pattern defines a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically.

a system event or application state change. The notification should be automated after an interested party subscribed to the system event or application state change. There also should be a way to unsubscribe.

Observers and observables probably should be represented by objects. The observer objects will be notified by the observable objects.

The observable object registers its own observers (this). Observables put its own observers in a list; Observer has observable as one member variable by reference.

````c++
// Observer
class ObserverBoardInterface {
public:
    virtual void update (float a, float b, float c) = 0;
};
class DisplayBoardInterface {
public:
    virtual void show() = 0;
};
// Observable
class WeatherDataInterface {
public:
    virtual void registerOb(ObserverBoardInterface* ob) = 0;
    virtual void removeOb(ObserverBoardInterface* ob) = 0;
    virtual void notifyOb() = 0;
};
class ParaWeatherData: public WeatherDataInterface {
public:
    void SensorDataChange(float a, float b, float c) {
        m_humidity = a;
        m_temperature = b;
        m_pressure = c;
        notifyOb();
    }
    void registerOb(ObserverBoardInterface* ob) override {
        m_obs.push_back(ob);
    }
    void removeOb(ObserverBoardInterface* ob) override {
        m_obs.remove(ob);
    }
protected:
    void notifyOb() override {
        list<ObserverBoardInterface*>::iterator pos = m_obs.begin();
        while (pos != m_obs.end()) {
            dynamic_cast<ObserverBoardInterface*>(*pos)->update(m_humidity, m_temperature, m_pressure);
            ++pos;
        }
    }
private:
    float m_humidity;
    float m_temperature;
    float m_pressure;
    list<ObserverBoardInterface*> m_obs;
};

class CurrentConditionBoard: public ObserverBoardInterface, public DisplayBoardInterface {
public:
    CurrentConditionBoard(ParaWeatherData& a): m_data(a) {
        m_data.registerOb(this);
    }
    void update(float h, float t, float p) override {
        m_h = h;
        m_t = t;
        m_p = p;
    }
    void show() override {
        cout << "__________CurrentConditionBoard_______________" << endl;
        cout << "humidity: " << m_h << endl;
        cout << "temperature: " << m_t << endl;
        cout << "pressure: " << m_p << endl;
        cout << "______________________________________________" << endl;
    }

private:
    float m_h;
    float m_t;
    float m_p;
    ParaWeatherData& m_data;
};
class StatisticBoard: public ObserverBoardInterface, public DisplayBoardInterface {
public:
    StatisticBoard(ParaWeatherData& a): m_maxt(-1000), m_mint(1000), m_avet(0), m_count(0), m_data(a) {
        m_data.registerOb(this);
    }
    void update(float h, float t, float p) override {
        ++m_count;
        if (t > m_maxt)
            m_maxt = t;
        if (t < m_mint)
            m_mint = t;
        m_avet = (m_avet*(m_count-1) + t)/m_count;
    }
    void show() override {
        cout << "__________StatisticBoard_______________" << endl;
        cout << "highest temperature: " << m_maxt << endl;
        cout << "lowest temperature: " << m_mint << endl;
        cout << "average temperature: " << m_avet << endl;
        cout << "_______________________________________" << endl;
    }

private:
    float m_maxt;
    float m_mint;
    float m_avet;
    int m_count;
    ParaWeatherData& m_data;
};


int main() {
    ParaWeatherData* wdata = new ParaWeatherData();
    CurrentConditionBoard* currentB = new CurrentConditionBoard(*wdata);
    StatisticBoard* statisticB = new StatisticBoard(*wdata);

    wdata->SensorDataChange(10.2, 28.2, 1001);
    wdata->SensorDataChange(12, 30.12, 1003);
    wdata->SensorDataChange(10.2, 26, 806);
    wdata->SensorDataChange(10.3, 35.9, 900);

    currentB->show();
    statisticB->show();

    wdata->removeOb(currentB);  // currentB no longer observing
    wdata->SensorDataChange(100, 40, 1900);

    currentB->show();
    statisticB->show();

    delete statisticB;
    delete currentB;
    delete wdata;

    return 0;
}
````



### State

The State Pattern allows an object to alter its behavior when its internal state changes. The object will appear as having changed its class.

```c++
// State
enum Input {DUCK_DOWN, STAND_UP, JUMP, DIVE};

class Fighter;
class StandingState; class JumpingState; class DivingState;

class FighterState {
public:
    static shared_ptr<StandingState> standing;
    static shared_ptr<DivingState> diving;

    virtual ~FighterState() = default;

    virtual void handleInput(Fighter&, Input) = 0;
    virtual void update(Fighter&) = 0;
};

class DuckingState: public FighterState {
public:
    DuckingState(): chargingTime(0) {}
    virtual void handleInput(Fighter &, Input) override;
    virtual void update(Fighter &) override;
private:
    int chargingTime;
    static const int FullRestTime = 5;
};
class StandingState: public FighterState {
public:
    virtual void handleInput(Fighter &, Input) override;
    virtual void update(Fighter &) override;
};
class JumpingState: public FighterState {
public:
    JumpingState() {
        jumpingHeight = rand()%5+1;
    }
    virtual void handleInput(Fighter &, Input) override;
    virtual void update(Fighter &) override;
private:
    int jumpingHeight;
};
class DivingState: public FighterState {
public:
    virtual void handleInput(Fighter &, Input) override;
    virtual void update(Fighter &) override;
};

shared_ptr<StandingState> FighterState::standing (new StandingState);
shared_ptr<DivingState> FighterState::diving (new DivingState);

class Fighter {
public:
    Fighter(const string& newName): name(newName), state(FighterState::standing) {}
    string getName() const {
        return name;
    }
    int getFatigueLevel() const {
        return fatigueLevel;
    }
    virtual void handleInput(Input input) {
        state->handleInput(*this, input);  // delegate input handling to 'state'
    }
    void changeState(shared_ptr<FighterState> newState) {
        state = newState;
        updateWithNewState();
    }
    void standsUp() {
        cout << getName() << " stands up." << endl;
    }
    void ducksDown() {
        cout << getName() << " ducks down." << endl;
    }
    void jumps() {
        cout << getName() << " jumps into the air." << endl;
    }
    void dives() {
        cout << getName() << " makes a dive attack in the middle of the jumpt!" << endl;
    }
    void feelsStrong() {
        cout << getName() << " feels strong" << endl;
    }
    void changeFatigueLevelBy(int change) {
        fatigueLevel += change;
        cout << "fatigueLevel = " << fatigueLevel << endl;
    }
private:
    virtual void updateWithNewState() {
        state->update(*this);  // delegate updating to 'state'
    }
private:
    string name;
    shared_ptr<FighterState> state;
    int fatigueLevel = rand()%10;
};

void StandingState::handleInput(Fighter& fighter, Input input) {
    switch(input) {
    case STAND_UP: cout << fighter.getName() << " remains standing." << endl; return;
    case DUCK_DOWN: fighter.changeState(shared_ptr<DuckingState> (new DuckingState)); return fighter.ducksDown();
    case JUMP: fighter.jumps(); return fighter.changeState(shared_ptr<JumpingState>(new JumpingState));
    default: cout << "One cannot do that while standing. " << fighter.getName() << " remains standing by default." << endl;
    }
}
void StandingState::update(Fighter& fighter) {
    if (fighter.getFatigueLevel() > 0)
        fighter.changeFatigueLevelBy(-1);
}

void DuckingState::handleInput(Fighter& fighter, Input input) {
    switch (input) {
    case STAND_UP: fighter.changeState(FighterState::standing); return fighter.standsUp();
    case DUCK_DOWN:
        cout << fighter.getName() << " remains in ducking position, ";
        if (chargingTime < FullRestTime)
            cout << "recoving in the meantime." << endl;
        else
            cout << "fully recovered." << endl;
        return update(fighter);
    default:
        cout << "One cannot do that while ducking. " << fighter.getName() << " remains in ducking position by default." << endl;
        update(fighter);
    }
}
void DuckingState::update(Fighter& fighter) {
    chargingTime++;
    cout << "Charging time = " << chargingTime << "." << endl;
    if (fighter.getFatigueLevel() > 0)
        fighter.changeFatigueLevelBy(-1);
    if (chargingTime >= FullRestTime && fighter.getFatigueLevel() <= 3)
        fighter.feelsStrong();
}

void JumpingState::handleInput(Fighter& fighter, Input input) {
    switch(input) {
    case DIVE: fighter.changeState(FighterState::diving);
        return fighter.dives();
    default:
        cout << "One cannot do that in the middle of a jump. " << fighter.getName() << " lands from his jump and is now in standing position." << endl;
        fighter.changeState(FighterState::standing);
    }
}
void JumpingState::update(Fighter& fighter) {
    fighter.changeFatigueLevelBy(2);
}

void DivingState::handleInput (Fighter& fighter, Input)  {
    std::cout << "Regardless of what the user input is, " << fighter.getName() << " lands from his dive and is now standing again." << std::endl;
    fighter.changeState (FighterState::standing);
}

void DivingState::update (Fighter& fighter) {
    fighter.changeFatigueLevelBy(2);
}

int main() {
    srand(time(nullptr));
    Fighter rex("Rex the Fighter"), borg("Borg the Fighter");
    cout << rex.getName() << " and " << borg.getName() << " are currently standing" << endl;
    int choice;
    auto chooseAction = [&choice](Fighter& fighter) {
        cout << endl << DUCK_DOWN + 1 << ") Duck down " << STAND_UP+1 << ") Stand up " << JUMP + 1
             << ") Jump " << DIVE+1 << ") Dive in the middle of a jump" << endl;
        cout << "Choice for " << fighter.getName() << "? ";
        cin >> choice;
        const Input input1 = static_cast<Input>(choice-1);
        fighter.handleInput(input1);
    };
    while (true) {
        chooseAction(rex);
        chooseAction(borg);
    }

    return 0;
}
```



### Strategy

### Template Method

### Visitor

### Model-View-Controller (MVC)

