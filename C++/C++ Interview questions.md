# C++ Interview questions

[toc]

## 1. How to read or write objects in files | Serializing & Deserializing objects

```c++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

class Student
{
public:
    std::string mId;
    std::string mName;
    std::string mPhoneNumber;
    Student(std::string id = "", std::string name = "", std::string phone = "") : mId(id), mName(name), mPhoneNumber(phone) {}
    bool operator==(const Student & obj)
    {
        return (mId == obj.mId) && (mName == obj.mName) && (mPhoneNumber == obj.mPhoneNumber);
    }
    /*
    * Write the member variables to stream objects
    */
    friend std::ostream& operator<< (std::ostream& out, const Student& obj) {
        out << obj.mId << "\n" << obj.mName << "\n" << obj.mPhoneNumber << std::endl;
        return out;
    }
    /*
    * Read data from stream object and fill it in member variables
    */
    friend std::istream& operator>> (std::istream& in, Student& obj) {
        in >> obj.mId;
        in >> obj.mName;
        in >> obj.mPhoneNumber;
        return in;
    }
};



int main()
{
    Student stud1("1","Jack", "4445554455");
    Student stud2("4","Riti", "4445511111");
    Student stud3("6","Aadi", "4040404011");
    // Open the File
    std::ofstream out("students.txt");
    // Write objects to file
    out<<stud1;
    out<<stud2;
    out<<stud3;
    out.close();

    // Open the File
    std::ifstream in("students.txt");
    Student student1;
    Student student2;
    Student student3;
    // Read objects from file and fill in data
    in>>student1;
    in>>student2;
    in>>student3;
    in.close();

    // Compare the Objects
    assert(stud1==student1);
    assert(stud2==student2);
    assert(stud3==student3);

    return 0;
}
```

## 2. How to read a file line by line into a vector

