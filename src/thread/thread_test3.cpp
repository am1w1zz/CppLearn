#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
 
void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // 提醒 future
}
 
void do_work(std::promise<std::string> barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value("123");
}
 
int f(int x, int y) { return std::pow(x,y); }
 
void task_lambda()
{
    std::packaged_task<int(int,int)> task([](int a, int b) {
        return std::pow(a, b); 
    });
    std::future<int> result = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << result.get() << '\n';
}
 
void task_thread()
{
    std::packaged_task<int(int,int)> task(f);
    std::future<int> result = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
 
    std::cout << "task_thread:\t" << result.get() << '\n';
}

void worker(std::future<void>& output)
{
    std::packaged_task<void(bool&)> my_task{ [](bool& done) { done=true; } };
 
    auto result = my_task.get_future();
 
    bool done = false;
 
    my_task.make_ready_at_thread_exit(done); // 立即执行任务
 
    std::cout << "worker: done = " << std::boolalpha << done << std::endl;
 
    auto status = result.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::timeout)
        std::cout << "worker: result is not ready yet" << std::endl;
 
    output = std::move(result);
}
void shared_future () {
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());
 
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
 
    auto fun1 = [&, ready_future]() -> std::chrono::duration<double, std::milli> 
    {
        t1_ready_promise.set_value();
        ready_future.wait(); // 等待来自 main() 的信号
        return std::chrono::high_resolution_clock::now() - start;
    };
 
    auto fun2 = [&, ready_future]() -> std::chrono::duration<double, std::milli> 
    {
        t2_ready_promise.set_value();
        ready_future.wait(); // 等待来自 main() 的信号
        return std::chrono::high_resolution_clock::now() - start;
    };
    auto result1 = std::async(std::launch::async, fun1);
    auto result2 = std::async(std::launch::async, fun2);
 
    // 等待线程变为就绪
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();
 
    // 线程已就绪，开始时钟
    start = std::chrono::high_resolution_clock::now();
 
    // 向线程发信使之运行
    ready_promise.set_value();
 
    std::cout << "Thread 1 received the signal "
              << result1.get().count() << " ms after start\n"
              << "Thread 2 received the signal "
              <<result2.get().count() << " ms after start\n";
}
std::mutex m;
template <typename RandomIt>
    int parallel_sum(RandomIt beg, RandomIt end)
    {
        auto len = end - beg;
        if (len < 1000)
            return std::accumulate(beg, end, 0);
    
        RandomIt mid = beg + len/2;
        auto handle = std::async(std::launch::async,
                                parallel_sum<RandomIt>, mid, end);
        int sum = parallel_sum(beg, mid);
        return sum + handle.get();
}
void async(){
    struct X {
        void foo(int i, const std::string& str) {
            std::lock_guard<std::mutex> lk(m);
            std::cout << str << ' ' << i << '\n';
        }
        void bar(const std::string& str) {
            std::lock_guard<std::mutex> lk(m);
            std::cout << str << '\n';
        }
        int operator()(int i) {
            std::lock_guard<std::mutex> lk(m);
            std::cout << i << '\n';
            return i + 10;
        }
    };
    
    std::vector<int> v(10000, 1);
    std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
 
    X x;
    // 以默认策略调用 x.foo(42, "Hello") ：
    // 可能同时打印 "Hello 42" 或延迟执行
    auto a1 = std::async(&X::foo, &x, 42, "Hello");
    // 以 deferred 策略调用 x.bar("world!")
    // 调用 a2.get() 或 a2.wait() 时打印 "world!"
    auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");
    // 以 async 策略调用 X()(43) ：
    // 同时打印 "43"
    auto a3 = std::async(std::launch::async, X(), 43);
    a2.wait();                     // 打印 "world!"
    std::cout << a3.get() << '\n'; // 打印 "53"
}


template< class Function, class... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>>
async(Function&& f, Args&&... args);

template<class Function, class... Args>
std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>> 
async(Function&& f, Args&&... args);

template< class Function, class... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>> 
async(std::launch policy, Function&& f, Args&&... args);


template< class Function, class... Args>
std::future<std::invoke_result_t<std::decay_t<Function>,std::decay_t<Args>...>>
async(std::launch policy, Function&& f, Args&&... args);

template< class Function, class... Args>
[[nodiscard]]
std::future<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>
async(std::launch policy, Function&& f, Args&&... args);

int main()
{
    // 演示用 promise<int> 在线程间传递结果。
    // std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
    // std::promise<int> accumulate_promise;
    // std::future<int> accumulate_future = accumulate_promise.get_future();
    // std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
    //                         std::move(accumulate_promise));
 
    // // future::get() 将等待直至该 future 拥有合法结果并取得它
    // // 无需在 get() 前调用 wait()
    // //accumulate_future.wait();  // 等待结果
    // std::cout << "result=" << accumulate_future.get() << '\n';
    // work_thread.join();  // wait for thread completion
 
    // // 演示用 promise<void> 在线程间对状态发信号
    // std::promise<std::string> barrier;
    // std::future<std::string> barrier_future = barrier.get_future();
    // std::thread new_work_thread(do_work, std::move(barrier));
    // barrier_future.wait();
    // new_work_thread.join();
    // std::cout<<barrier_future.get()<<std::endl;
   

    // task_lambda();
    // task_bind();
    // task_thread();
    // std::future<void> result;
 
    // std::thread{worker, std::ref(result)}.join();
 
    // auto status = result.wait_for(std::chrono::seconds(0));
    // if (status == std::future_status::ready)
    //     std::cout << "main: result is ready" << std::endl;

    // shared_future();
    async();
}