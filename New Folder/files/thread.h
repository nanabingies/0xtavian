//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_THREAD_H
#define INTERLACE_THREAD_H

#pragma once
#include <mutex>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>

class threadSafe {
private:
    std::vector<int> rawQueue;
    std::mutex m;

public:
    /*int& retrieve_and_delete() {
        std::lock_guard<std::mutex> lg(m);
        int back_value = 0;

        if (!rawQueue.empty()) {
            back_value = rawQueue.back();
            rawQueue.pop_back();
        }
        return back_value;
    }*/

    threadSafe(std::vector<int>& q) {
        this->rawQueue = q;
    }

};

#endif //INTERLACE_THREAD_H
