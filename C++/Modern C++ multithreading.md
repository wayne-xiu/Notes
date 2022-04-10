# Modern C++ Multithreading

[toc]

## Introduction

```c++
#include <iostream>
#include <thread>
#include <chrono>

void work() {
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(chrono::milliseconds(500));
        cout << "loop " << i << endl;
    }
}

int main() {
    std::thread t1(work);
    std::thread t2(work);

    t1.join();
    t2.join();

    return 0;
}
```



When is multi-threading useful?

1. When you're waiting for something external and want to execute code meanwhile (asynchronous execution)

   example: pinging remote servers

2. Distributing processing across multiple cores

## Locks

### Shared Data

lambda expression, anonymous function

```c++
int main() {
    int count = 0;
    const int ITERATIONS = /*1e3*/1e6;

    std::thread t1([&count]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            ++count;
        }
    });

    std::thread t2([&count]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            ++count;
        }
    });

    t1.join();
    t2.join();

    cout << count << endl;

    return 0;
}
```

when `ITERATIONS` set as 1000, the output will be 2000 as (wrongly) expected. However, for 1e6, the result varies each time.

simple fix with `atomic`

```c++
    atomic<int> count = 0;  // #include <atomic>
    const int ITERATIONS = /*1e3*/1e6;

    std::thread t1([&count]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            ++count;
        }
    });

    std::thread t2([&count]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            ++count;
        }
    });

    t1.join();
    t2.join();

    cout << count << endl;
```



### Mutexes

more general issue for the above issue with `mutex`

```c++
    int count = 0;
    const int ITERATIONS = /*1e3*/1e6;

    std::mutex mtx;	// include <mutex>

    auto func = [&count, &mtx]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            mtx.lock();
            ++count;
            mtx.unlock();
        }
    };

    std::thread t1(func);
    std::thread t2(func);
    t1.join();
    t2.join();

    cout << count << endl;
```

critical section; only one thread can access at one time.

### Function Arguments

```c++
void work(int& count, std::mutex& mtx) {
    const int ITERATIONS = 1e6;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        mtx.lock();
        ++count;
        mtx.unlock();
    }
}
void shared_data_mutex_func_arg() {
    int count = 0;
    std::mutex mtx;

    std::thread t1(work, std::ref(count), std::ref(mtx));
    std::thread t2(work, std::ref(count), std::ref(mtx));

    t1.join();
    t2.join();

    cout << count << endl;
}
```

It is not actually very common to use mutex lock/unlock directly. Exception can be thrown in between. We can use `lock_guard` or `unique_lock`

### Lock Guards

Prefer `RAII` over manual lock/unlock

```c++
void work_lockguard(int& count, std::mutex& mtx) {
    const int ITERATIONS = 1e6;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        std::lock_guard<std::mutex> guard(mtx);
        ++count;
    }
}
void shared_data_lockguards() {
    int count = 0;
    std::mutex mtx;

    std::thread t1(work_lockguard, std::ref(count), std::ref(mtx));
    std::thread t2(work_lockguard, std::ref(count), std::ref(mtx));

    t1.join();
    t2.join();

    cout << count << endl;
}
```

`unique_lock` has a bunch of methods that `lock_guard` doesn't

```c++
void work_uniquelock(int& count, std::mutex& mtx) {
    const int ITERATIONS = 1e6;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        std::unique_lock<std::mutex> guard(mtx);
        ++count;
    }
}
void shared_data_uniquelock() {
    int count = 0;
    std::mutex mtx;

    std::thread t1(work_uniquelock, std::ref(count), std::ref(mtx));
    std::thread t2(work_uniquelock, std::ref(count), std::ref(mtx));

    t1.join();
    t2.join();

    cout << count << endl;
}
```

### Threads with Callable Objects

A callable object is something that can be called like a function, with the the syntax `object()` or `object(args)`; In C++, that is a function pointer, or an object of a class that overloads `operator()`

```c++
class App {
private:
    int count_ = 0;
    std::mutex mtx_;
public:
    void operator()() {
        for (int i = 0; i < 1e6; ++i) {
            const std::lock_guard<std::mutex> guard(mtx_);
            ++count_;
        }
    }

    int get_count() const {
        return count_;
    }
};

void thread_with_callable_object() {
    App app;

    std::thread t1(std::ref(app));
    std::thread t2(std::ref(app));

    t1.join();
    t2.join();

    cout << app.get_count() << endl;
}
```



## Returning values from Threads

### Promises and Futures

```c++
double calculate_pi(int terms) {
    double sum = 0.0;
    for (int i = 0; i < terms; ++i) {
        int sign = pow(-1, i);
        double term = 1.0 / (i *2 + 1);
        sum += sign * term;
    }
    return sum*4;
}
void future_promise_pi() {
    std::promise<double> promise;
    auto do_calculation = [&](int terms) {
        auto result = calculate_pi(terms);
        promise.set_value(result);
    };

    std::thread t1(do_calculation, 1e8);
    future<double> future = promise.get_future();
    cout << future.get() << endl; // get() is a block call

    t1.join();
}
```

### Promises and Exceptions

```c++
double calculate_pi(int terms)
{
    double sum = 0.0;
    if (terms < 1) {
        throw runtime_error("Terms cannot be less than 1");
    }
    for (int i = 0; i < terms; ++i)
    {
        int sign = pow(-1, i);
        double term = 1.0 / (i * 2 + 1);
        sum += sign * term;
    }
    return sum * 4;
}
void future_promise_pi()
{
    std::promise<double> promise;
    auto do_calculation = [&](int terms)
    {
        try
        {
            auto result = calculate_pi(terms);
            promise.set_value(result);
        }
        catch (const std::exception &e)
        {
            promise.set_exception(current_exception());
        }
    };

    std::thread t1(do_calculation, 0);
    future<double> future = promise.get_future();
    try
    {
        cout << future.get() << endl; // get() is a block call
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }

    t1.join();
}
```

### Packaged Tasks

```c++
void packaged_task_pi() {
    std::packaged_task<double(int)> task1(calculate_pi);
    future<double> future1 = task1.get_future();
    std::thread t1(std::move(task1), 1e6);

    try
    {
        double res = future1.get();
        cout << res << endl;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    
    t1.join();
}
```



## Signaling

### Waiting for Threads

```c++
void naive_thread_signaling() {
    /*bool*/ /*thread_local bool*/atomic_bool ready = false;

    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        ready = true;
    });
    t1.join();

    while(!ready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    cout << "ready: " << std::boolalpha << ready << endl;
}
```

### Condition Variables

```c++
void condition_variable_signaling() {
    bool ready = false;
    std::mutex mtx;
    std::condition_variable cond;
    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::unique_lock<std::mutex> lock(mtx);
        ready = true;
        lock.unlock();
        cond.notify_one();
    });

    std::unique_lock<std::mutex> lock(mtx);
    while(!ready) {
        cond.wait(lock);
    }
    cout << "ready: " << std::boolalpha << ready << endl;

    t1.join();
}
```

### Checking Condition Shared Resources

```c++
void condition_variable_check_signaling() {
    bool ready = false;
    std::mutex mtx;
    std::condition_variable cond;
    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "t1 acquiring lock" << std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "t1 acquired lock" << std::endl;
        ready = true;
        lock.unlock();
        std::cout << "t1 released lock. notifying" << std::endl;
        cond.notify_one();
    });

    std::cout << "main thread acquiring lock" << std::endl;
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "main thread acquired lock; waiting" << std::endl;
    cond.wait(lock, [&ready](){return ready;});
    std::cout << "main thread finished" << std::endl;
    cout << "ready: " << std::boolalpha << ready << endl;

    t1.join();
}
```

### Blocking Queue

Java has one `ArrayBlockingQueue` for multi-threading use

Producer-Consumer pattern

### Containers and Thread Safety

```c++
// naive implementation
template <typename E>
class BlockingQueue {
public:
    BlockingQueue(size_t max_size): max_size_{max_size} {}

    void push(E e) {
        queue_.push(e);
    }

    E pop() {
        E item = queue_.front();
        queue_.pop();
        return item;
    }

private:
    const size_t max_size_;
    std::queue<E> queue_;
};
```

may or may not crash

```c++
void blocking_queue_producer_consumer() {
    BlockingQueue<int> qu(5);
    std::thread t1([&](){
        for (int i = 0; i < 10; ++i) {
            qu.push(i);
            std::cout << "produced " << i << std::endl;
        }
    });
    std::thread t2([&]{
        for (int i = 0; i < 10; ++i) {
            auto item = qu.pop();
            std::cout << "consumed " << item << std::endl;
        }
    });
    t1.join();
    t2.join();
}
```

### Producer Consumer

```c++
template <typename E>
class BlockingQueue
{
public:
    BlockingQueue(size_t max_size = std::numeric_limits<size_t>::max())
        : max_size_{max_size} {}

    void push(E e) {
        std::unique_lock<std::mutex> lock(mtx_);
        notifier_var_.wait(lock, [this]()
                           { return queue_.size() < max_size_; });
        queue_.push(e);

        lock.unlock();
        notifier_var_.notify_one();
    }

    E pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        notifier_var_.wait(lock, [this]()
                           { return !queue_.empty(); });
        E item = queue_.front();
        queue_.pop();

        lock.unlock();
        notifier_var_.notify_one();

        return item;
    }

private:
    const size_t max_size_;
    std::queue<E> queue_;
    std::mutex mtx_;
    std::condition_variable notifier_var_;
};
```

### Slight improved version

```c++
template <typename E>
class BlockingQueue
{
public:
    BlockingQueue(size_t max_size = std::numeric_limits<size_t>::max())
        : max_size_{max_size} {}

    void push(E e) {
        std::unique_lock<std::mutex> lock(mtx_);
        notifier_var_.wait(lock, [this]()
                           { return queue_.size() < max_size_; });
        queue_.push(e);

        lock.unlock();
        notifier_var_.notify_one();
    }

    E front() {
        std::unique_lock<std::mutex> lock(mtx_);
        notifier_var_.wait(lock, [this]()
                           { return !queue_.empty(); });
        
        return queue_.front();
    }

    void pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        notifier_var_.wait(lock, [this]()
                           { return !queue_.empty(); });
        queue_.pop();

        lock.unlock();
        notifier_var_.notify_one();
    }

    size_t size() {
        std::lock_guard<std::mutex> guard(mtx_);
        return queue_.size();
    }

private:
    const size_t max_size_;
    std::queue<E> queue_;
    std::mutex mtx_;
    std::condition_variable notifier_var_;
};
```

## Processing Work Efficiently

### Async

- Calling `std::async` with *function pointer* as callback

```c++
std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
```

- Calling `std::async` with *Function Object* as callback

```c++
struct DataFetcher; // with std::string operator()(std::string) defined
std::future<std::string> fileResult = std::async(DataFetcher(), "Data");
```

- Calling `std::async` with *Lambda function* as callback

```c++
std::future<std::string> resultFromDB = std::async([](std::string recvdData){
                        std::this_thread::sleep_for (seconds(5));
                        //Do stuff like creating DB Connection and fetching Data
                        return "DB_" + recvdData;
                    }, "Data");
```



### Hardware Concurrency

```c++
void hardware_concurrency() {
    cout << std::thread::hardware_concurrency() << endl;
}
// output (depends): 16
```

### Launching Lots of Threads

```c++
void invoke_multiple_thread() {
    std::mutex mtx_;
    auto work = [&mtx_](int id){
        std::unique_lock<std::mutex> lock(mtx_);
        std::cout << "Starting " << id << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        return id;
    };

    int hardware_core = std::thread::hardware_concurrency();
    std::vector<std::shared_future<int>> futures;
    for (int i = 0; i < hardware_core; ++i) {
        std::shared_future<int> f = std::async(std::launch::async, work, i);
        futures.emplace_back(f);
    }
}
```

### Thread Pool

```c++
void thread_pool_blockingqueue() {
    BlockingQueue<shared_future<int>> futures(2);  // n+1
    std::mutex mtx_;
    auto work = [&mtx_](int id){
        std::unique_lock<std::mutex> lock(mtx_);
        std::cout << "Starting " << id << std::endl;
        lock.unlock();

        int seconds = (int)(5.0 * std::rand()) / RAND_MAX + 2;
        std::this_thread::sleep_for(std::chrono::seconds(seconds));

        return id;
    };

    std::thread t([&]() {
        for (int i = 0; i < 20; i++) {
            std::shared_future<int> f = std::async(std::launch::async, work, i);
            futures.push(f);
        }
    });

    for (int i = 0; i < 20; i++) {
        std::shared_future<int> f = futures.front();
        int value = f.get();
        futures.pop();
        std::cout << "Returned: " << value << std::endl;
    }
    t.join();
}
```

TODO: revisit

### Distributing Work Between Cores

distributing the calculate *pi* work on multiple threads

```c++
void calculate_pi_distribute_cores()
{
    // in-efficient pi calculation by design
    auto cal_pi = [](int terms, int start, int skip) {
        double sum = 0.0;
        for (int i = start; i < terms; i += skip) {
            int sign = std::pow(-1, i);
            double term = 1.0 / (i*2 + 1);
            sum += sign*term;
        }
        return sum * 4;
    };

    std::vector<std::shared_future<double>> futures;
    const int CONCURRENCY = std::thread::hardware_concurrency();

    for (int i = 0; i < CONCURRENCY; ++i) {
        std::shared_future<double> f = std::async(std::launch::async, cal_pi, 1e7, i, CONCURRENCY);
        futures.push_back(f);
    }

    double sum = 0.0;
    for (auto f: futures) {
        sum += f.get();
    }
    std::cout << std::setprecision(15) << "Pi: " << sum << std::endl;
}
```

significant improvements over 1 thread

## Conclusion
