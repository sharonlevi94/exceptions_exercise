#include "FunctionCalculator.h"

#include "Sin.h"
#include "Ln.h"
#include "Poly.h"
#include "Mul.h"
#include "Add.h"
#include "Comp.h"
#include "Log.h"

#include <istream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <string>
#include <fstream>

FunctionCalculator::FunctionCalculator(std::istream& istr, std::ostream& ostr)
    : m_actions(createActions()), m_functions(createFunctions()), m_istr(istr), m_ostr(ostr)
{
}

void FunctionCalculator::run()
{
    m_ostr << std::setprecision(2) << std::fixed;
    // add for file usage

    //bool readingFromInputFile = false;
    do
    {
        // file:
//        if (readingFromInputFile){
//            std::string currInput;
//
//            continue;
//        }
        // end of file usage.
        m_ostr << '\n';
        printFunctions();
        m_ostr << "Enter command ('help' for the list of available commands): ";
        try {
            const auto action = readAction("");
            if (action == Action::Read) {
                //readingFromInputFile = true;
                m_ostr << "Enter file path:\n";
                std:: string fileName, lineOfCommand;
                m_istr >> fileName;
                std::fstream inputFile(fileName);
                // add except here for file open
                while (inputFile){
                    // read line
                    std::getline(inputFile,lineOfCommand);
                    // send line to "runAction"
                    const auto fileAction = readAction(lineOfCommand.substr(0, lineOfCommand.find(' ')));
                    runAction(fileAction);
                    ///Users/barifrah/CLionProjects/exceptions_exercise
                }

                inputFile.close();
                continue;
            }
            runAction(action);
        }
        catch (std::out_of_range& e) {
            this->m_ostr << e.what();
            m_istr.clear();
            m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (m_running);
}

void FunctionCalculator::eval()
{
    if (auto i = readFunctionIndex(); i)
    {
        auto x = 0.;
        m_istr >> x;

        auto sstr = std::ostringstream();
        sstr << std::setprecision(2) << std::fixed << x;
        m_ostr << m_functions[*i]->to_string(sstr.str())
            << " = "
            << (*m_functions[*i])(x)
            << '\n';
    }
}

void FunctionCalculator::poly()
{
    auto n = 0;

        m_istr >> n;
        if (n < 0)
            throw std::out_of_range("\ncoeffient must be positive\n");

    auto coeffs = std::vector<double>(n);
    for (auto& coeff : coeffs)
    {
        m_istr >> coeff;
    }

    m_functions.push_back(std::make_shared<Poly>(coeffs));
}

void FunctionCalculator::log()
{
    auto base = 0;
    m_istr >> base;
    if (base < 0 || base == 1)
        throw std::out_of_range("\nBase of log must be positive & differ then 1\n");

    if (auto f = readFunctionIndex(); f)
    {
        m_functions.push_back(std::make_shared<Log>(base, m_functions[*f]));
    }
}

void FunctionCalculator::del()
{
    if (auto i = readFunctionIndex(); i)
    {
        m_functions.erase(m_functions.begin() + *i);
    }
}

void FunctionCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

void FunctionCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

// add "Read" function here:
// bool - to return if EOF ot not.
bool FunctionCalculator::read()
{
    //
    return false;
}

void FunctionCalculator::printFunctions() const
{
    m_ostr << "List of available gates:\n";
    for (decltype(m_functions.size()) i = 0; i < m_functions.size(); ++i)
    {
        m_ostr << i << ".\t" << m_functions[i]->to_string("x") << '\n';
    }
    m_ostr << '\n';
}

std::optional<int> FunctionCalculator::readFunctionIndex() const
{
    auto i = 0;
    m_istr >> i;
    if (i >= m_functions.size() || i < 0)
        throw std::out_of_range("\nFunction #" +std::to_string(i) +" doesn't exist\n");
    return i;
}

FunctionCalculator::Action FunctionCalculator::readAction(const std::string& possibleAction) const
{
    auto action = std::string();
    // multi purpose this function, to use with file and with std input
    if (!possibleAction.empty())   action = possibleAction;
    else    m_istr >> action;

    for (decltype(m_actions.size()) i = 0; i < m_actions.size(); ++i)
    {
        if (action == m_actions[i].command)
        {
            return m_actions[i].action;
        }
    }
    return Action::Invalid;
}

//add FILE* here
void FunctionCalculator::runAction(Action action)
{
    switch (action)
    {
        default:
            throw std::out_of_range("\nUnknown enum entry used!\n");

        case Action::Invalid:
            throw std::out_of_range("\nCommand not found\n");

        case Action::Eval: eval();             break;
        case Action::Poly: poly();             break;
        case Action::Mul:  binaryFunc<Mul>();  break;
        case Action::Add:  binaryFunc<Add>();  break;
        case Action::Comp: binaryFunc<Comp>(); break;
        case Action::Log:  log();              break;
        case Action::Del:  del();              break;
        case Action::Help: help();             break;
        case Action::Exit: exit();             break;
        // add read:
        case Action::Read: read();             break;
    }
}

FunctionCalculator::ActionMap FunctionCalculator::createActions()
{
    return ActionMap
    {
        {
            "eval",
            "(uate) num x - compute the result of function #num on x",
            Action::Eval
        },
        {
            "poly",
            "(nomial) N c_0 c_1 ... c_(N-1) - creates a polynomial with N coefficients",
            Action::Poly
        },
        {
            "mul",
            "(tiply) num1 num2 - Creates a function that is the multiplication of "
            "function #num1 and function #num2",
            Action::Mul
        },
        {
            "add",
            " num1 num2 - Creates a function that is the sum of function #num1 and "
            "function #num2",
            Action::Add
        },
        {
            "comp",
            "(osite) num1 num2 - creates a function that is the composition of "
            "function #num1 and function #num2",
            Action::Comp
        },
        {
            "log",
            " N num - create a function that is the log_N of function #num",
            Action::Log
        },
        {
            "del",
            "(ete) num - delete function #num from the function list",
            Action::Del
        },
        {
            "help",
            " - print this command list",
            Action::Help
        },
        {
            "exit",
            " - exit the program",
            Action::Exit
        },
        /* Added this "Read" inquiry to the vector */
        {
           "read",
           " - read commands from external file",
           Action::Read
        }
    };
}

FunctionCalculator::FunctionList FunctionCalculator::createFunctions()
{
    return FunctionList
    {
        std::make_shared<Sin>(),
        std::make_shared<Ln>()
    };
}
