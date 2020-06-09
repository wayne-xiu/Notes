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

```c++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/*
* It will iterate through all the lines in file and
* put them in given vector
*/
bool getFileContent(std::string fileName, std::vector<std::string>& vecOfStrs) {
    std::ifstream in(fileName.c_str());
    if (!in) {
        std::cerr << "Can't open file: " << fileName << std::endl;
        return false;
    }
    std::string str;
    while(std::getline(in, str)) {
        if (str.size() > 0)
            vecOfStrs.push_back(str);
    }
    in.close();
    return true;
}

/*
* It will iterate through all the lines in file and
* call the given callback on each line.
*/
bool iterateFile(std::string fileName, std::function<void (const std::string&)> callback) {
    std::ifstream in(fileName.c_str());
    if (!in) {
        std::cerr << "Cann't open the file: " << fileName << std::endl;
        return false;
    }
    std::string str;
    while(std::getline(in, str)) {
        // call the given callback
        callback(str);
    }
    in.close();
    return true;
}

int main() {
    std::vector<std::string> vecOfStr;
    bool result = getFileContent("example.cpp", vecOfStr);

    if (result) {
        for (std::string& line: vecOfStr)
            std::cout << line << std::endl;
    }

    // callback
    bool res = iterateFile("example.cpp", [&](const std::string& str) {
        vecOfStr.push_back(str);
    });
    if (res) {
        for (std::string& line: vecOfStr)
            std::cout << line << std::endl;
    }
    return 0;
}
```

make a generic function that will accept a callback or function pointer along with file name.

## 3. How to read data from a csv file in C++

```c++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;

class CSVReader {
public:
    CSVReader(std::string filename, std::string delm = ","):
        fileName(filename), delimeter(delm) {}
    std::vector<std::vector<std::string>> getData();
private:
    std::string fileName;
    std::string delimeter;
};

std::vector<std::vector<string> > CSVReader::getData() {
    std::ifstream file(fileName);
    std::vector<std::vector<std::string>> dataList;
    std::string line = "";
    // iterate through each line and split the content using delimter
    while (getline(file, line)) {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
        dataList.push_back(vec);
    }
    file.close();

    return dataList;
}

int main() {
    CSVReader reader("example.csv");
    std::vector<std::vector<std::string>> dataList = reader.getData();
    for (std::vector<std::string> vec: dataList) {
        for (std::string data: vec)
            std::cout << data << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
```

To include Boost in Qt Creator projector, se the .pro file as bellow

```c++
TEMPLATE = app
CONFIG += console c++11
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32 {
    INCLUDEPATH += C:/boost_1.70.0
    LIBS += -L"C:/boost_1.70.0/stage/lib"
}
```

## 4. How to write data in a CSV file in C++

```c++
class CSVWriter {
public:
    CSVWriter(std::string filename, std::string delm = ","):
        fileName(filename), delimeter(delm), linesCount(0) {}

    template<typename T>
    void addDatainRow(T first, T last);
private:
    std::string fileName;
    std::string delimeter;
    int linesCount;
};

template<typename T>
void CSVWriter::addDatainRow(T first, T last) {
    std::fstream file;
    // Open the file in truncate mode if first line else in Append Mode
    file.open(fileName, std::ios::out | (linesCount ? std::ios::app : std::ios::trunc));

    // Iterate over the range and add element
    for (; first != last;) {
        file << *first;
        if (++first != last)
            file << delimeter;
    }
    file << "\n";
    linesCount++;
    file.close();
}

int main() {
    CSVWriter writer("example.csv");
    // the below code: could not deduce template parameter 'T' for iterator  - TODO
    //std::vector<std::string> datalist_1 = {"20", "hi", "90"};
    //writer.addDatainRow(datalist_1.begin(), dataList.end());
    std::set<int> datalist_2 = {3, 4, 5};
    writer.addDatainRow(datalist_2.begin(), datalist_2.end());
    std::string str = "abc";
    writer.addDatainRow(str.begin(), str.end());
    int arr[] = {3, 4, 1};
    writer.addDatainRow(arr, arr+sizeof(arr)/sizeof(int));

    return 0;
}
```

## 5. Overloading Postfix/Prefix(++,--) Increment and Decrements operators

```c++
#include <iostream>

using namespace std;

class ComplexNumber {
public:
    ComplexNumber(): real(0), imaginary(0){}
    ComplexNumber(int r, int i): real(r), imaginary(i) {}
    friend std::ostream& operator<< (std::ostream& os, const ComplexNumber& obj);

    // Prefix - Postfix - Prefix - Postfix
    ComplexNumber operator++();
    ComplexNumber operator++(int);
    ComplexNumber operator--();
    ComplexNumber operator--(int);
private:
    int real;
    int imaginary;
};

// this is not a member function
ostream& operator<< (ostream& os, const ComplexNumber& obj) {
    int img = obj.imaginary < 0 ? -obj.imaginary : obj.imaginary;
    os << obj.real << (obj.imaginary < 0 ? " - " : " + ") << "i" << img << std::endl;

    return os;
}

ComplexNumber ComplexNumber::operator++() {
    ++real;
    ++imaginary;
    return *this;
}

ComplexNumber ComplexNumber::operator++(int) {
    ComplexNumber tempObj(this->real, this->imaginary);
    ++real;
    ++imaginary;
    return tempObj;
}
ComplexNumber ComplexNumber::operator--() {
    --real;
    --imaginary;
    return *this;
}

ComplexNumber ComplexNumber::operator--(int) {
    ComplexNumber tempObj(this->real, this->imaginary);
    --real;
    --imaginary;
    return tempObj;
}

int main() {

    ComplexNumber c1(2, 3);
    std::cout << "c1 = " << c1 << std::endl;

    std::cout << "Testing Postfix Increment"<<std::endl;
    ComplexNumber c2 = c1++; // Postfix Increment
    std::cout << "c2 = "<< 	c2;
    std::cout << "c1 = "<< c1;
    std::cout << "Testing Prefix Increment"<<std::endl;
    ComplexNumber c3 = ++c1; // Prefix Increment
    std::cout << "c3 = "<< 	c3;
    std::cout << "c1 = "<< c1;
    std::cout << "Testing Postfix Decrement"<<std::endl;
    ComplexNumber c4 = c1--; // Postfix Decrement
    std::cout << "c4 = "<< 	c3;
    std::cout << "c1 = "<< c1;
    std::cout << "Testing Prefix Decrement"<<std::endl;
    ComplexNumber c5 = --c1; // Prefix Decrement
    std::cout << "c5 = "<< 	c3;
    std::cout << "c1 = "<< c1;

    return 0;
}
```

