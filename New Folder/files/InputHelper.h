//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_INPUTHELPER_H
#define INTERLACE_INPUTHELPER_H

#pragma once
#include "Header.h"
#include "threader.hpp"

namespace fs = std::experimental::filesystem;

//template <typename T>
class InputHelper {

    InputHelper() {}

    static struct tasks {
        string name;
        string clone;
    };

    static struct arguments {
        string target;
        string target_list;
    };

    static bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    static std::string strip_whitespace(const std::string& inpt)
    {
        auto start_it = inpt.begin();
        auto end_it = inpt.rbegin();
        while (std::isspace(*start_it))
            ++start_it;
        while (std::isspace(*end_it))
            ++end_it;
        return std::string(start_it, end_it.base());
    }

    static std::vector<std::string> split_string(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    static bool check_path(const string& path) {
        ifstream f(path.c_str());
        if (f.good() == false) {
            std::cout << "[-] Invalid File Path.\n";
            exit(-1);
        }
        return f.good();
    }

    static fstream& readable_file(const string& file) {
        if (check_path(file)) {
            //ofstream MyFile(file);
            //return MyFile;
            fstream MyFile;
            MyFile.open(file, ios::in);
            return MyFile;
        }
        throw std::invalid_argument("Invalid File Passed.");
    }

    static int check_positive(int arg) {
        auto i = arg;
        if (i <= 0) {
            cout << i << " is not a valid positive ineteger!\n";
            exit (-1);
        }
        // except ???*/
        return static_cast<int>(i);
    }

    /*static void get_files_from_directory(const string& filePath) {
        for (const auto& entry : fs::directory_iterator(filePath)) {
            file_list.push_back(entry.path());
            //return entry.path();
        }
    }*/

    /*static void get_files_from_directory(const string& filePath) {
        struct dirent* entry;
        DIR* dir = opendir(filePath);
        if (dir == NULL) {
            return;
        }

        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
            file_list.push_back(entry->d_name);
        }

        closedir(dir);
    }*/

    static vector<string>& _process_port(string& port_type) {
        vector<string> vect{};
        stringstream s_stream(port_type);
        if (port_type.find(",") != string::npos) {
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ',');
                vect.push_back(substr);
            }
            return vect;
        }
        else if (port_type.find("-") != string::npos) {
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, '-');
                vect.push_back(substr);
            }
            auto begin_range = vect.at(0);
            auto end_range = vect.at(1);
            if (std::stoi(begin_range) >= std::stoi(end_range)) {
                throw std::invalid_argument("Invalid range provided");
            }
            return vect;
        }
        vect.push_back(port_type);
        return vect;
    }

    static vector<Task>& pre_process_commands(vector<string>& command_list,
                                              string task_name = "", bool is_global_task = true) {
        vector<Task> task_block{};
        int sibling, blocker;
        for (auto command : command_list) {
            command = strip_whitespace(command);
            if (command.length() == 0)
                continue;

            // the start or end of a command block
            // Not implementing blocking. YAY!!!
        }
        return task_block;
    }

    static void _replace_target_variables_in_commands(vector<Task>& tasks,
                                                      const vector<string>& str_targets, const vector<string>& ipset_targets) {
        string TARGET_VAR = "_target_";
        string HOST_VAR = "_host_";
        string CLEANTARGET_VAR = "_cleantarget_";
        Task yielded_task{};
        string dirty_target{};
        /*for (auto it = begin(tasks); it != end(tasks); ++it) {
            string command = it->name;
        }*/
        for (auto task : tasks) {
            string command = task.name;
            if ((command.find(TARGET_VAR) != string::npos) || (command.find(HOST_VAR) != string::npos)) {
                //for (itertools.chain)
                yielded_task = task;
                dirty_target = static_cast<string>(dirty_target);
                yielded_task.replace(TARGET_VAR, dirty_target);
                yielded_task.replace(HOST_VAR, dirty_target);
            }
        }
    }

    static void _replace_variable_in_commands(std::function<vector<Task>(void)> tasks_generator_func,
                                              const string& variable, const vector<string>& replacements) {
        Task yielded_task{};
        for (auto task : tasks_generator_func()) {
            if ((task.name).find(variable) != string::npos) {
                for (auto replacement : replacements) {
                    yielded_task = task;
                    yielded_task.replace(variable, static_cast<std::string>(replacement));
                    std::this_thread::yield();
                }
            }
            else
                std::this_thread::yield();
        }
    }

    static void _replace_variable_array(std::function<vector<Task>(void)>task_generator_func,
                                        const string& variable, const vector<string>::iterator replacements_iterator) {
        for (auto task : task_generator_func()) {
            replace(task.name, variable, (string)replacements_iterator.operator++);
            std::this_thread::yield();
        }
    }

    // Should be inline to _process_targets
    static vector<string>& pre_process_target_spec(string& target_spec) {
        target_spec = strip_whitespace(target_spec);
        vector<string> vect{};
        stringstream s_stream(target_spec);
        if (target_spec.find(",") != string::npos) {
            while (s_stream.good()) {
                string substr;
                getline(s_stream, substr, ',');
                vect.push_back(substr);
            }
            return vect;
        }
        vect.push_back(target_spec);
        return vect;
    }

    static vector<string>& parse_and_group_target_specs(const vector<string>& target_specs,
                                                        int nocidr) {
        vector<string>str_targets{};
        vector<string>ips_list{};
        for (string target_spec : target_specs) {
            if ((reinterpret_cast<string&>(target_spec).rfind(".", 0) == 0) ||
                (isalpha(target_spec.at(0))) || (isalpha(target_spec.at(target_spec.length()))) ||
                nocidr && (target_spec.find("/") != string::npos)) {
                str_targets.push_back(target_spec);
            }
            else {
                if (target_spec.find("-") != string::npos) {
                    vector<string>res = split_string(target_spec, '-');
                    string start_ip = res.at(0);
                    string post_dash_segment = res.at(1);
                    res = split_string(start_ip, '.');
                    string end_ip = res.at(0) + '.' + post_dash_segment;
                    //target_spec = IPRange(start_ip, end_ip);
                }

                for (auto i : target_spec) {
                    //ips_list.push_back('a');
                }
            }
        }
        return str_targets, ips_list;
    }

    static void _process_targets(struct arguments& arguments) {
        string target_specs;
        if (arguments.target.length() > 0) {
            auto target_specs = pre_process_target_spec(arguments.target);
        }
        else {
            auto target_specs_file = arguments.target_list;
            for (auto target_spec : target_specs_file) {
                target_specs = strip_whitespace(reinterpret_cast<string&>(target_spec));
            }
            for (auto target_spec : target_specs) {
                if (target_spec) {
                    pre_process_target_spec(reinterpret_cast<string&>(target_spec));
                }
            }
            // Itertools.chain
            // implement it with boost
        }


    }



private:
    vector<string> file_list{};

};

#endif //INTERLACE_INPUTHELPER_H
