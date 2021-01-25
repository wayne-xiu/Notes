# General C++ Note

[toc]

## How to convert a numerical type to string

```c++
string convertDoubleToString(const long double value, const int precision)
{
    stringstream stream{};
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

int main()
{
    double e{1e-9};
    // string valueAsString = std::to_string(e);	// 0.000000
    string valueAsString = convertDoubleToString(e, 10);  // ok
    cout << valueAsString << endl;
}
```

1. Since C++11, std::to_string can be used to convert all numerical types to string. But it has inaccuracy issue and no configuration capability
2. stringstream is better choice

