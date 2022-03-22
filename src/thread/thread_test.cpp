#include <iostream>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <sstream>
#include <vector>
#include <functional>

template <class T>
void measrue(T&& args) {
    auto start = std::chrono::system_clock::now();
    args();
    std::chrono::duration<double> diff =  std::chrono::system_clock::now() - start;
    std::cout<< diff.count() <<std::endl;
}


    class ThreadSafeCounter {
    public:
    ThreadSafeCounter() = default;

    // Multiple threads/readers can read the counter's value at the same time.
    unsigned int get() const {
        std::shared_lock lock(mutex_);
        return value_;
    }

    // Only one thread/writer can increment/write the counter's value.
    void increment() {
        std::unique_lock lock(mutex_);
        value_++;
    }

    // Only one thread/writer can reset/write the counter's value.
    void reset() {
        std::unique_lock lock(mutex_);
        value_ = 0;
    }

    private:
    mutable std::shared_mutex mutex_;
    unsigned int value_ = 0;
    };

// std::chrono::milliseconds interval(100);
// std::mutex mutex;
// int job_shared = 0;
// int job_exclusive = 0;

// void job_1() 
// {
//     std::this_thread::sleep_for(interval);
//     while (true) {
//         if (mutex.try_lock()) {
//             std::cout << "job shared (" << job_shared << ")\n";
//             mutex.unlock();
//             return;
//         } else {
//             ++job_exclusive;
//             std::cout << "job exclusive (" << job_exclusive << ")\n";
//             std::this_thread::sleep_for(interval);
//         }
//     }
// }

// void job_2() 
// {
//     mutex.lock();
//     std::this_thread::sleep_for(9 * interval);
//     ++job_shared;
//     mutex.unlock();
// }
 
// std::timed_mutex test_mutex;

// auto now=std::chrono::steady_clock::now(); 

// void f(int val)
// {
//     // test_mutex.try_lock_until(now + std::chrono::seconds(10));
//     std::lock_guard<std::mutex> lock(mutex);
//     for(int i = 0; i < 20; ++i) {
       
//     }
//     // std::cout << "hello world\n";
// }

// std::mutex cout_mutex; // 控制到 std::cout 的访问
// std::timed_mutex mutex;
 
// void job(int id) 
// {
//     using Ms = std::chrono::milliseconds;
//     std::ostringstream stream;
 
//   for (int i = 0; i < 3; ++i) {
//         if (mutex.try_lock_for(Ms(90))) {
//             stream << "success ";
//             std::this_thread::sleep_for(Ms(100));
//             mutex.unlock();
//         } else {
//             stream << "failed ";
//         }
//         std::this_thread::sleep_for(Ms(100));
//     }
 
//     std::lock_guard<std::mutex> lock(cout_mutex);
//     std::cout << "[" << id << "] " << stream.str() << "\n";
// }


// class X {
//     std::recursive_mutex m;
//     std::string shared;
//   public:
//     void fun1() {
//       std::lock_guard<std::recursive_mutex> lk(m);
//       shared = "fun1";
//       std::cout << "in fun1, shared variable is now " << shared << '\n';
//     }
//     void fun2() {
//       std::lock_guard<std::recursive_mutex> lk(m);
//       shared = "fun2";
//       std::cout << "in fun2, shared variable is now " << shared << '\n';
//       fun1(); // 递归锁在此处变得有用
//       std::cout << "back in fun2, shared variable is " << shared << '\n';
//     };
// };
 
struct Employee {
    Employee(std::string id) : id(id) {}
    std::string id;
    std::vector<std::string> lunch_partners;
    std::mutex m;
    std::string output() const
    {
        std::string ret = "Employee " + id + " has lunch partners: ";
        for( const auto& partner : lunch_partners )
            ret += partner + " ";
        return ret;
    }
};
 
void send_mail(Employee &, Employee &)
{
    // 模拟耗时的发信操作
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
void assign_lunch_partner(Employee &e1, Employee &e2)
{
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
    }
 
    {
        // 用 std::scoped_lock 取得二个锁，而无需担心
        // 其他对 assign_lunch_partner 的调用死锁我们
        // 而且它亦提供便利的 RAII 风格机制
 
        std::scoped_lock lock(e1.m, e2.m);
 
        // 等价代码 1 （用 std::lock 和 std::lock_guard ）
        // std::lock(e1.m, e2.m);
        // std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
        // std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);
 
        // 等价代码 2 （若需要 unique_lock ，例如对于条件变量）
        // std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
        // std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
        // std::lock(lk1, lk2);
        {
            std::lock_guard<std::mutex> lk(io_mutex);
            std::cout << e1.id << " and " << e2.id << " got locks" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }
 
    send_mail(e1, e2);
    send_mail(e2, e1);
}

int main() {
    // measrue([](){
    //     for(int i = 0; i < 1000; ++i){
    //     }
    // });
    
    // ThreadSafeCounter counter;
    // auto increment_and_print = [&counter]() {
    //     for (int i = 0; i < 3; i++) {
    //         counter.increment();
    //         std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';
    //     }
    // };
    // std::thread thread1(increment_and_print);
    // std::thread thread2(increment_and_print);
    // thread1.join();
    // thread2.join();     
    // std::thread thread_1(job_1);
    // int a = 1;
    // std::thread thread_2(f, a);
    // int b = 2;
    // std::thread thread_3(f, b);
    // int c = 3;
    // std::thread thread_4(f, c);
    // thread_2.join();
    // thread_3.join();
    // thread_4.join();

    // std::lock_guard<std::timed_mutex> l(test_mutex);
    // std::thread t(f);
    // t.join();
   

    // std::vector<std::thread> threads;
    // for (int i = 0; i < 4; ++i) {
    //     threads.emplace_back(job, i);
    // }
 
    // for (auto& i: threads) {
    //     i.join();
    // // }
    // X x;
    // // std::thread t1(&X::fun1, &x);
    // std::thread t2(&X::fun2, &x);
    // // t1.join();
    // t2.join();



    Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");
 
    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));
 
    for (auto &thread : threads) thread.join();
    std::cout << alice.output() << '\n'  << bob.output() << '\n'<< christina.output() << '\n' << dave.output() << '\n';
}
