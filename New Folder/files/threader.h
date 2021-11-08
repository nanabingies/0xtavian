//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_THREADER_H
#define INTERLACE_THREADER_H

#pragma once
//#include "InputHelper.hpp"
#include "../Header.h"
#include "thread.h"
using namespace std::chrono_literals;

thread_local size_t worker_idx = -1;

//template <class T>
class Task {
public:
    Task() {
        str = "Default";
        this->task = (Task *)&str;
    }
    Task(const std::string& st) {
        str = st;
        this->task = (Task *)&str;
        //self_lock = { 0 };
    }
    ~Task() {}

    Task(class Task& command){
        this->task = command.task;
        //self_lock = { 0 };
    }

    Task& operator=(class Task& task) {
        //this->self_lock = task.self_lock();
        //this->sibling_locks = task.sibling_locks();
        class Task* new_task = this->task;
        task.self_lock = this->self_lock;
        task.sibling_locks = this->sibling_locks;
        return *new_task;
    }

    bool __cmp__(class Task& other){
        return name() == other.name();
        //return task == &other;
    }

    Task& __hash__(void) {
        return this->task->__hash__();
    }

    Task* clone(const Task* task1) {
        class Task* new_task = this->task;
        new_task->self_lock = task1->self_lock;
        new_task->sibling_locks = task1->sibling_locks;
        return new_task;
    }

    /*Task replace(class Task& old_task, class Task& new_task) {
        this->task = &this->task->replace(old_task, new_task);
    }*/

    std::string replace(std::string old, std::string new_string) {
        std::string str = old;
        str = str.replace(old.begin(), old.end(), new_string);
        return str;
    }

    // Come back and check implementation
    void run(bool t = false) {
        for (auto lock : this->sibling_locks) {
            std::unique_lock<std::mutex>lck(mtx);
            cv.wait(lck);
            // lock.wait() - Implemented ???????
        }
        this->_run_task(t);
        if (this->self_lock.empty() == false) {
            //this->self_lock.set();
            mtx.lock();
        }
    }

    void wait_for(std::vector<Task> siblings) {
        //size_t i = 0;
        for (auto sibling : siblings) {
            //reinterpret_cast<std::string&>(sibling_locks).push_back(sibling.get_lock());

        }
    }

    Task* name() {
        return this->task;
    }

    std::vector<std::string>& get_lock() {
        std::mutex mut;
        std::condition_variable data_cond;
        if (!this->self_lock.empty()) {
            std::lock_guard<std::mutex> lk(mut);
            data_cond.notify_one();
            //this->self_lock = Event();
            this->self_lock.clear();
        }
        return this->self_lock;
    }

    void _run_task(bool t = false) {
        //subprocess::popen cmd(this->task, shell = true,
        //	subprocess::pipe, executable = shell);
        //std::cout << *this->task << std::endl;
        printf("\n%s\n", *this->task);
    }

private:
    class Task* task;
    std::string str;
    std::vector<std::string> self_lock;
    std::vector<int> sibling_locks{};

protected:
    std::mutex mtx;
    std::condition_variable cv;
};

template <class T>
class Worker : public Task{
public:
    Worker(std::vector<T>& task_queue,
           size_t timeout, std::string& output, std::string& tq):Task() {
        this->queue.assign(task_queue.begin(), task_queue.end());
        this->timeout = timeout;
        this->output = output;
        this->tqdm = tq;
    }

    void __cal__(void) {
        //list<string>queue = this->queue;
        std::vector<std::string>queue{};
        queue.assign(this->queue.begin(), this->queue.end());
        while (true) {
            try {
                auto it = queue.begin();
                auto tx = std::next(it, 1);
                Task::run();
                //tx.run();
            }
            catch (...) {
                throw std::invalid_argument("[-] Stopping Iteration.");
            }
        }
    }

private:
    std::vector<std::string> queue{};
    size_t timeout{ 0 };
    std::string output{};
    std::string tqdm{};
    //class Task task;
};

//template<class T>
class Pool {
public:
    Pool(int max_worker, std::vector<std::string>& task_queue, int timeout,
         std::string output) {
        size_t _max_workers = max_worker;
        //this->max_workers = max_worker;
        if (_max_workers <= 0) {
            throw std::invalid_argument("Workers must be >= 1");
        }

        auto it = task_queue.begin();
        auto tasks_count = std::next(it, 1);

        /*if (task_queue.empty() == true) {
            std::cout << "[-] The queue is empty.\n";
            exit(-1);
        }*/
        /*if (tasks_count == 0) {
            std::cout << "The queue is empty\n";
            exit(-1);
        }*/

        this->queue = task_queue;
        this->timeout = timeout;
        this->output = output;
        //this->max_workers = std::min((int)tasks_count.operator++, _max_workers);
        /*if (tasks_count.operator++ >= _max_workers) {
            this->max_workers = tasks_count.operator++;
        }
        else {
            this->max_workers = _max_workers;
        }*/
        this->max_workers = _max_workers;
    }

    void* _run_task() {
        printf("\n%s\n", this->t);
    }

    void run(void) {
        std::chrono::seconds keep_alive_time = 5s;
        size_t pool_size = 4;
        size_t max_queue_size = 64;
        std::random_device rand_dev;
        std::uniform_int_distribution<int> dist(0, 500);
        Worker<std::string> worker(this->queue, this->timeout,
                                   this->output, this->tqdm);
        //ThreadPoolExecutor executer(4, pool_size, keep_alive_time, max_queue_size);
        //for (auto work = 0; work < max_workers; work++) {

        //executer.submit(_run_task);
        //std::this_thread::sleep_for(std::chrono::milliseconds(dist(rand_dev)));
        //worker->_run_task();
        //}
        //executer.shutdown();
        //executer.wait();
    }

    void func() {
        /*vector<int> vect{ 4, 7, 11, 21, 34 };
        threadSafe t1(vect);

        int num = t1.retrieve_and_delete();
        cout << num << endl;

        num = t1.retrieve_and_delete();
        cout << num << endl;

        num = t1.retrieve_and_delete();
        cout << num << endl;

        num = t1.retrieve_and_delete();
        cout << num << endl;

        num = t1.retrieve_and_delete();
        cout << num << endl;

        Task t("nana");
        Task p("bingies");
        Task l = t;
        if (l.__cmp__(t) == true) {
            cout << "Same\n";
        }
        else {
            cout << "Not the Same\n";
        }

        printf("%s : %x\n", *t.name(), t.name());
        printf("%s : %x\n", *p.name(), p.name());
        printf("%s : %x\n", *l.name(), l.name());

        vector<std::string>vecT{};
        std::string str = "bingies";
        vecT.push_back(str);
        //vecT.push_back(p);
        //vecT.push_back(l);

        //Worker<std::string>* worker = new Worker<std::string>(vecT, 0, str, str);
        //worker->_run_task();
        Pool* pool = new Pool(4, vecT, 0, str);
        pool->run();

        std::string m = "nana";
        std::string n = "bingies";
        m.replace(m.begin(), m.end(), n);
        std::cout << m << std::endl;*/
    }

private:
    //Worker* worker;
    std::vector<std::string> queue;
    size_t timeout{};
    std::string output;
    size_t max_workers;
    std::string tqdm{};
    Task t;
};

#endif //INTERLACE_THREADER_H
