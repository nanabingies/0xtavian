//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_INPUT_H
#define INTERLACE_INPUT_H

//#include "cxxopts.hpp"
#include "Output.h"
#include <vector>
#include <algorithm>


class InputParser {
public:
    InputParser(int& argc, char** argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(string(argv[i]));
        arguments = {};
    }

    const string& getCmdOption(const string & option) const {
        vector<string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const string empty_string("");
        return empty_string;
    }

    bool cmdOptionExists(const string & option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }

    std::vector<string>& setup_args(void) {
        string res{};
        if (cmdOptionExists("-t")) {
            const string& t = getCmdOption("-t");
            dest = "target";
            res = t;
            required = false;
            help = "Specify a target or domain name either in comma format, "
                   "CIDR notation, glob notation, or a single target.";
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-tL")) {
            const string& tL = getCmdOption("-tL");
            dest = "target_list";
            res = tL;
            help = "Specify a list of targets or domain names.";
            // come back to this func
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-e")) {
            const string& e = getCmdOption("-e");
            dest = "exclusions";
            res = e;
            help = "Specify an exclusion either in comma format, "
                   "CIDR notation, or a single target.";
            //auto itPs = arguments.begin() + 0;
            //arguments.insert(itPs, res);
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-eL")) {
            const string& eL = getCmdOption("-eL");
            dest = "exclusions_list";
            required = false;
            help = "Specify a list of exclusions.";
            // come back to this func
            res = eL;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-threads")) {
            const string& th = getCmdOption("-threads");
            dest = "threads";
            required = false;
            help = "Specify the maximum number of threads to run (DEFAULT:5)";
            defaultThread = 5;
            // come back to this func
            res = th;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-timeout")) {
            const string& time = getCmdOption("-timeout");
            dest = "timeout";
            required = false;
            defaultTime = 600;
            res = time;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-c")) {
            const string& c = getCmdOption("-c");
            dest = "command";
            help = "Specify a single command to execute.";
            res = c;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-cL")) {
            const string& cL = getCmdOption("-cL");
            dest = "command_list";
            required = false;
            help = "Specify a list of commands to execute";
            // come back to this func
            res = cL;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-o")) {
            const string& o = getCmdOption("-o");
            dest = "output";
            help = "Specify an output folder variable that can be used in commands as _output_";
            res = o;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-random")) {
            const string& random = getCmdOption("-random");
            dest = "random";
            help = "Specify a directory of files that can be randomly used in commands as _random_";
            // come back to this func
            res = random;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("--no-cidr")) {
            const string& cidr = getCmdOption("--no-cidr");
            dest = "nocidr";
            help = "If set then CIDR notation in a target file will not be automatically "
                   "be expanded into individual hosts.";
            res = cidr;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        if (cmdOptionExists("-p")) {
            const string& port = getCmdOption("-p");
            dest = "port";
            help = "Specify a port";
            res = port;
            arguments.push_back(res);
        }
        else
            arguments.push_back("");

        return arguments;
    }

public:
    vector <string> tokens;
    vector <string> arguments{};
    string dest, help;
    bool required = false;
    size_t defaultThread = 0, defaultTime = 0;
};

#endif //INTERLACE_INPUT_H
