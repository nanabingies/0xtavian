//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_INPUTFILE_H
#define INTERLACE_INPUTFILE_H

#include "Header.h"
#include "thread_pool.h"
#include "Input.h"

namespace InputFile {

    vector<string> file_contents, port_contents;
    static size_t file_count = 0;

    class InputHelperFunctions {
    public:



    };


    class InputFile {
    public:
        InputFile() {}

        bool replace(std::string& str, const std::string& from, const std::string& to) {
            size_t start_pos = str.find(from);
            if (start_pos == std::string::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        }

        std::string strip_whitespace(const std::string & inpt) {
            auto start_it = inpt.begin();
            auto end_it = inpt.rbegin();
            while (std::isspace(*start_it))
                ++start_it;
            while (std::isspace(*end_it))
                ++end_it;
            return std::string(start_it, end_it.base());
        }

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

        bool check_path(const string& path) {
            ifstream f(path.c_str());
            if (f.good() == false) {
                std::cout << "[-] Invalid File Path.\n";
                exit(-1);
            }
            return f.good();
        }

        std::vector<std::string> _process_files(std::string filepath) {
            check_path(filepath);
            std::vector<std::string> file_contents{};

            fstream newfile;
            newfile.open(filepath, std::ios::in);
            static int count = 0;
            if (newfile.is_open()){
                std::string tp;
                while(std::getline(newfile, tp)){
                    file_contents.push_back(tp);
                    ++count;
                }
                newfile.close();
            }
            std::cout << "Count : " << count << std::endl;
            return file_contents;
        }

        std::vector<std::string> _replace_target_variables_in_commands(vector<string> tasks,
                                                                       const vector<string> target_string, const vector<string> clean_string,
                                                                       const string port_vars = ""/*const vector<string>& ipset_targets*/) {
            string TARGET_VAR = "_target_";
            string HOST_VAR = "_host_";
            string CLEANTARGET_VAR = "_cleantarget_";
            string PORT_VAR = "_port_";
            string yielded_task{};
            string dirty_host{};

            for (auto task : tasks) {
                string command = task;
                if ((command.find(TARGET_VAR) != string::npos) || (command.find(HOST_VAR) != string::npos)) {
                    //for (itertools.chain)
                    //if ((command.find(PORT_VAR) != string::npos)) {
                        for (auto t : target_string) {
                            yielded_task = task;
                            string dirty_target{ t };
                            replace(yielded_task, TARGET_VAR, dirty_target);
                            replace(yielded_task, HOST_VAR, dirty_target);
                            replace(yielded_task, PORT_VAR, port_vars);
                            replace(yielded_task, "http://", "");
                            replace(yielded_task, "https://", "");
                            replace(yielded_task, "/", "-");
                            std::this_thread::yield();
                            file_contents.push_back(yielded_task);
                        }
                    //}
                }
                else if ((command.find(CLEANTARGET_VAR) != string::npos)) {
                    for (auto t : target_string) {
                        yielded_task = task;
                        string dirty_target{ t };
                        replace(yielded_task, CLEANTARGET_VAR, dirty_target);
                        replace(yielded_task, "http://", "");
                        replace(yielded_task, "https://", "");
                        replace(yielded_task, "/", "-");
                        std::this_thread::yield();
                        file_contents.push_back(yielded_task);
                    }
                }

                /*if ((command.find(PORT_VAR) != string::npos)){
                    yielded_task = task;
                    string dirty_target{ port_vars };
                    replace(yielded_task, PORT_VAR, dirty_target);
                    std::this_thread::yield();
                    file_contents.push_back(yielded_task);
                }*/
                else {
                    std::this_thread::yield();
                    file_contents.push_back(task);
                }
            }
            return file_contents;
        }

        std::vector<std::string> _process_port(string port_type) {
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

    private:


    };
}

#endif //INTERLACE_INPUTFILE_H
