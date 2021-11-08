//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_THREAD_POOL_H
#define INTERLACE_THREAD_POOL_H

#pragma once
#include "Header.h"
#include "files/threader.h"
#include "subprocess.h"

using namespace std::chrono_literals;

namespace thread_pool {
    std::random_device rand_dev;
    thread_local size_t worker_idx = -1;
    std::atomic_size_t workers_cnt;

    std::vector<std::string> split_string(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    void popen_exec(std::string operation, std::vector<std::string> args){
        subprocess::popen cmd(operation, {args});
        std::cout << cmd.stdout().rdbuf();
    }

    void run_my_task(std::string str) {
        if (worker_idx == -1) {
            worker_idx = ++workers_cnt;
        }

        std::vector<std::string> stripped = split_string(str, ' ');
        std::string op = stripped.at(0);
        std::vector<std::string> vec{};
        for (int i=1; i<stripped.size(); i++){
            vec.push_back(stripped.at(i));
        }

        std::stringstream message;

        std::uniform_int_distribution<int> dist(0, 10);
        std::this_thread::sleep_for(std::chrono::seconds(dist(rand_dev)));

        message << "[+] [THREAD] [" << str << "] added to queue" << std::endl;

        std::cout << message.str();
        std::cout << endl;
        //popen_exec(op, vec);
    }

    void task(size_t i) {
        if (worker_idx == -1) {
            worker_idx = ++workers_cnt;
        }
        std::stringstream message;

        std::uniform_int_distribution<int> dist(0, 10);
        std::this_thread::sleep_for(std::chrono::seconds(dist(rand_dev)));

        message << "[" << "worker-" << std::setfill('0') << std::setw(2) << worker_idx << "]" << "\t"
                << "task-" << std::setw(2) << i << " has been compleated." << std::endl;
        std::cout << message.str();
    }

    template <class T>
    void run(std::vector<T> tasks, size_t threads = 5) {
        std::uniform_int_distribution<int> dist(0, 500);

        size_t pool_size = threads;
        size_t max_pool_size = 16;
        size_t max_queue_size = 64;
        std::chrono::seconds keep_alive_time = 5s;

        ThreadPoolExecutor executor(pool_size, max_pool_size, keep_alive_time, max_queue_size);

        /*for (size_t i = 0; i < 100; ++i) {
            executor.submit(std::bind(task, i));
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(rand_dev)));
        }*/

        for (auto vec : tasks) {
            executor.submit(std::bind(run_my_task, vec));
            std::this_thread::sleep_for(std::chrono::microseconds(dist(rand_dev)));
        }

        executor.shutdown();
        executor.wait();
    }


}

#endif //INTERLACE_THREAD_POOL_H
