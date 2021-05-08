#pragma once
#include "Macros.h"

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <sstream>

class Function;

class FunctionCalculator
{
public:
    FunctionCalculator(std::istream& istr, std::ostream& ostr);
    void run();

private:
    void eval();
    void poly();
    void log();
    void del();
    void help();
    void exit();
    bool read();

    template <typename FuncType>
    void binaryFunc()
    {
        if (auto f0 = readFunctionIndex(), f1 = readFunctionIndex(); f0 && f1)
        {
            m_functions.push_back(std::make_shared<FuncType>(m_functions[*f0], m_functions[*f1]));
        }
    }

    void printFunctions() const;

    enum class Action
    {
        Invalid,
        Eval,
        Poly,
        Mul,
        Add,
        Comp,
        Log,
        Del,
        Help,
        Exit,
        Read
    };

    struct ActionDetails
    {
        std::string command;
        std::string description;
        Action action;
    };

    using ActionMap = std::vector<ActionDetails>;
    using FunctionList = std::vector<std::shared_ptr<Function>>;

    const ActionMap m_actions;
    FunctionList m_functions;
    bool m_running = true;
    bool m_readFile=false;
    std::istream& m_istr;
    std::ostream& m_ostr;
    std::istringstream m_iss;
    int m_maxFuncs = MAX_SIZE;

    std::optional<int> readFunctionIndex();
    Action readAction() ;
    void runAction(Action action);
    double readArgs();
    void setMaxSize();
    void listCapacityHandler(Action action);

    static ActionMap createActions();
    static FunctionList createFunctions();
};
