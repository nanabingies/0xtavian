//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_OUTPUT_H
#define INTERLACE_OUTPUT_H

#include <string>
#include <ctime>
#include <chrono>
#include <iostream>
#include <unordered_map>
using namespace std;


namespace m_Output {
    struct args {
        bool silent;
        bool verbose;
    }args;
}


class OutputHelper {
private:
    string separator = "=====================================================";
    bool silent{};
    bool verbose{};

public:
    // Constructors
    OutputHelper(struct m_Output::args& argument) {
        silent = argument.silent;
        verbose = argument.verbose;
    }

    ~OutputHelper() {}

    void print_banner(void) {
        if (silent == 1)
            return;

        cout << separator << endl;
        cout << "Interlace by Michael Skelton (@codingo_)\n";
        cout << "\t& Sajeeb Lohani (@sml555_)\n";
        cout << separator << endl;
    }

    void terminal(std::size_t level, string target,
                  string command, string message = "") {
        if ((level == 0) && (!verbose))
            return;

        /*unordered_map<int, string> format_args{
            {"time", chrono::system_clock::now()},
        {"target", target},
        {"command", command},
        {"message", message}
        };*/
        auto time_format = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(time_format);

        //auto stringTemplate;
        if (!silent) {
            if (level == 1)
                cout << " " << target << " "
                     << command << " " << message << endl;
            else
                cout << " " << target << " "
                     << command << " " << message << endl;
            //cout << stringTemplate;
        }
    }
};



class Level {
private:
    std::size_t VERBOSE{};
    std::size_t THREAD{};
    std::size_t ERROR{};

public:
    //Level(class L);
    Level() {
        VERBOSE = 0;
        THREAD = 1;
        ERROR = 3;
    }
};

#endif //INTERLACE_OUTPUT_H
