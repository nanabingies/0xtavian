//
// Created by kali on 11/6/21.
//

#ifndef INTERLACE_EXECUTER_H
#define INTERLACE_EXECUTER_H

#include "Input.h"
#include "InputFile.h"
#include "Output.h"
#include "thread_pool.h"

#include <limits>
#include <cstddef>
#include <sstream>
#include <iomanip>
#include <unistd.h>

std::random_device rand_dev;

namespace Executer {

    void ExecuteWithoutPorts(vector<string> comm, string targetFile, string thr, string c) {
        InputFile::InputFile f;
        //std::cout << "[+] Arguments Passed.\n";
        /*for (auto com : comm)
            if (com != "")
                cout << com << endl;*/

        vector<string> commandstoExecute = {};
        vector<string>  com{}, cop;
        vector<string> ccc{}, ccp{};


        commandstoExecute = f._process_files(targetFile);
        /*cout << "\n[+] Commands to execute\n";
        for (auto command : commandstoExecute) {
            cout << command << endl;
        }*/

        ccc.push_back(c);

        cout << endl << endl;

        com = f._replace_target_variables_in_commands(ccc, commandstoExecute, commandstoExecute, "99");

        /*for (auto aut : com)
            cout << aut << endl;*/

        usleep(5);
        thread_pool::run(com, (int)stoi(thr));
        //thread_pool::run(InputFile::file_contents, (int)stoi(thr));
        usleep(5);
    }

    void ExecuteWithPorts(vector<string> comm, string targetFile, string thr, string c,
                          vector<string> ports) {
        InputFile::InputFile f;
        /*std::cout << "[+] Arguments Passed.\n";
        for (auto com : comm)
            if (com != "")
                cout << com << endl;*/

        vector<string> commandstoExecute = {};
        vector<string>  com{}, cop;
        vector<string> ccc{}, ccp{};


        commandstoExecute = f._process_files(targetFile);
        /*cout << "\n[+] Commands to execute\n";
        for (auto command : commandstoExecute) {
            cout << command << endl;
        }*/

        ccc.push_back(c);

        //cout << endl << endl;
        int size = ports.size();
        //cout << size << endl;

        //vector<string> processPorts = f._process_port(ports);
        //for (auto t : ports) {
        com = f._replace_target_variables_in_commands(ccc, commandstoExecute, commandstoExecute, ports.at(0));

        //for (auto aut : com)
        //	cout << aut << endl;

        usleep(5);
        thread_pool::run(com, (int)stoi(thr));
        usleep(5);
    }

    void usage(char** argv){
        cout << "Usage : " << argv[0] << " -tL <file> -c <operation> \n";
        exit (-1);
    }

    void Execute(int argc, char** argv) {
        m_Output::args.silent = 0;
        m_Output::args.verbose = 1;

        OutputHelper help{ m_Output::args };

        help.print_banner();

        help.terminal(1, "nikto", "-t", "nikto run");

        InputParser input(argc, argv);
        InputFile::InputFile f;
        vector<string> comm = input.setup_args();


        string target{}, excluList{}, c{}, random{}, ports{};
        string targetFile{}, thr{}, cL{};
        string excluFile{}, timeout{}, o{};
        vector<string> port{};

        if (comm.at(0) != "")
            target = comm.at(0);
        if (comm.at(1) != "")
            targetFile = comm.at(1);
        if (comm.at(1) == "") {
            usage(argv);
        }
        if (comm.at(2) != "")
            excluFile = comm.at(2);
        if (comm.at(3) != "")
            excluList = comm.at(3);
        if (comm.at(4) != "")
            thr = comm.at(4);
        if (comm.at(5) != "")
            timeout = comm.at(5);
        if (comm.at(6) != "")
            c = comm.at(6);
        if (comm.at(6) == "") {
            usage(argv);
        }
        if (comm.at(7) != "")
            cL = comm.at(7);
        if (comm.at(8) != "")
            o = comm.at(8);
        if (comm.at(9) != "")
            random = comm.at(9);
        if (comm.at(11) != "") {
            ports = comm.at(11);
            port = f._process_port(ports);
            cout << "[+] Executing with ports.\n";
            ExecuteWithPorts(comm, targetFile, thr, c, port);
        }
        if (comm.at(11) == "") {
            cout << "[+] Executing without ports.\n";
            ExecuteWithoutPorts(comm, targetFile, thr, c);
        }
    }


}

#endif //INTERLACE_EXECUTER_H
