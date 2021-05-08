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
#include <limits>
#include <string>
#include <fstream>

//-----------------------------------------------------------------------------
//                                Constructor

FunctionCalculator::FunctionCalculator(std::istream& istr, std::ostream& ostr)
    : m_actions(createActions()), 
    m_functions(createFunctions()), 
    m_istr(istr), m_ostr(ostr){}

//-----------------------------------------------------------------------------
//                              Execute Method

void FunctionCalculator::run()
{
    m_ostr << std::setprecision(2) << std::fixed;
    while (!setMaxSize()) {}

    do
    {
        m_ostr << '\n';
        printFunctions();
        m_ostr << "Enter command ('help' for the list of available commands): ";
        try {
            const auto action = readAction();
            runAction(action);
        }
        catch (const std::out_of_range& e) {
       
            this->m_ostr << e.what();
            m_istr.clear();
            m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        catch (const std::ifstream::failure& e) {
            this->m_ostr << e.what();
        }

    } while (m_running);
}

//-----------------------------------------------------------------------------
//                              Commands Methods

void FunctionCalculator::eval()
{
    if (auto i = readFunctionIndex(); i)
    {
        auto x = readArgs();

        auto sstr = std::ostringstream();
        sstr << std::setprecision(2) << std::fixed << x;
        m_ostr << m_functions[*i]->to_string(sstr.str())
            << " = "
            << (*m_functions[*i])(x)
            << '\n';
    }
}

//---------------------------------------

void FunctionCalculator::poly()
{
    auto n = readArgs();

        if (n < 0)
            throw std::out_of_range("\ncoeffient must be positive\n");

    auto coeffs = std::vector<double>(n);
    for (auto& coeff : coeffs)
    {
        coeff=readArgs();
    }

    m_functions.push_back(std::make_shared<Poly>(coeffs));
}

//---------------------------------------

void FunctionCalculator::log()
{
    int base = readArgs();
    if (base < 0 || base == 1)
        throw std::out_of_range("\nBase of log must be positive & differ then 1\n");

    if (auto f = readFunctionIndex(); f)
    {
        m_functions.push_back(std::make_shared<Log>(base, m_functions[*f]));
    }
}

//---------------------------------------

void FunctionCalculator::del()
{
    if (m_functions.size() == MIN_SIZE)
        throw std::out_of_range("\nCan't have less than 2 functions\n");

    if (auto i = readFunctionIndex(); i)
    {
        m_functions.erase(m_functions.begin() + *i);
    }
}

//---------------------------------------

void FunctionCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

//---------------------------------------

void FunctionCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

//---------------------------------------

void FunctionCalculator::read()
{
        std::string fileName, fileCommandLine;

        if (m_readFile)
            m_iss >> fileName;
        else
            m_istr >> fileName;

        std::fstream input_file(fileName);
        // add except here for file open:
        if (!input_file)
            throw std::ifstream::failure("\nWrong file path\n");
    
    while (input_file) {
        this->m_readFile = true;
        // read line
        while (std::getline(input_file, fileCommandLine)) {
            m_iss = std::istringstream(fileCommandLine);
            m_iss.exceptions(std::ios::failbit | std::ios::badbit);
            if (!(m_iss.eof() || (m_iss >> std::ws).eof())) {
                try {
                    const auto fileAction = readAction();
                    runAction(fileAction);
                }
                catch (std::exception& e) {
                    m_ostr << fileCommandLine + "\n" + e.what() + "\n";
                    m_ostr << "Keep reading from file?(y/n)\n";
                    char answer;
                    m_istr >> answer;
                    if (answer == 'y')
                        continue;
                    else {
                        input_file.close();
                        break;
                    }
                }
            }
        }
    }
    if(input_file.is_open())
        input_file.close();
    this->m_readFile = false;
    //E:\commands.txt
    //E:\commands1.txt
}

//---------------------------------------

void FunctionCalculator::resize() {
    auto newSize = readArgs(); //get new size from the user\file
    try {
        if (newSize < m_functions.size())
            throw std::out_of_range("\nNew size is smaller than current size\n");
    }
    catch (std::out_of_range& e) {
        char selection;
        m_ostr << e.what() << std::endl;
        m_ostr << "This will cause lost of all out of range data if you choose to proceed\n";
        m_ostr << "Do you with to proceed with the action? This can not be undone (y/n)\n";
        m_istr >> selection;
        selection = tolower(selection);
        if (selection == 'y' ){
            for (int i = m_functions.size(); i > newSize ; i--) {
                m_functions.pop_back();
            }
        } else  return;
    }
    m_maxFuncs = newSize;

}

//-----------------------------------------------------------------------------

void FunctionCalculator::printFunctions() const
{
    m_ostr << "List of available gates:\n";
    for (decltype(m_functions.size()) i = 0; i < m_functions.size(); ++i)
    {
        m_ostr << i << ".\t" << m_functions[i]->to_string("x") << '\n';
    }
    m_ostr << '\n';
    m_ostr << "You can create " << m_maxFuncs - m_functions.size() << " more funcions\n";
}

//-----------------------------------------------------------------------------

std::optional<int> FunctionCalculator::readFunctionIndex() 
{
    int i = readArgs();

    if (i >= m_functions.size() || int(i) < 0)
        throw std::out_of_range("\nFunction #" +std::to_string(i) +" doesn't exist\n");
    return i;
}

//-----------------------------------------------------------------------------

FunctionCalculator::Action FunctionCalculator::readAction() 
{
    auto action = std::string();
    // multi purpose this function, to use with file and with std input
    if (m_readFile) m_iss >> action;
    else            m_istr >> action;

    for (decltype(m_actions.size()) i = 0; i < m_actions.size(); ++i)
    {
        if (action == m_actions[i].command)
        {
            return m_actions[i].action;
        }
    }
    return Action::Invalid;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::runAction(Action action)
{
    if (m_functions.size() == m_maxFuncs)
        listCapacityHandler(action);
    else
        switch (action)
        {
            default:
                throw std::out_of_range("\nUnknown enum entry used!\n");

            case Action::Invalid:
                throw std::out_of_range("\nCommand not found\n");

            case Action::Eval:   eval();             break;
            case Action::Poly:   poly();             break;
            case Action::Mul:    binaryFunc<Mul>();  break;
            case Action::Add:    binaryFunc<Add>();  break;
            case Action::Comp:   binaryFunc<Comp>(); break;
            case Action::Log:    log();              break;
            case Action::Del:    del();              break;
            case Action::Help:   help();             break;
            case Action::Exit:   exit();             break;
            case Action::Read:   read();             break;
            case Action::Resize: resize();           break;
        }
}

//-----------------------------------------------------------------------------

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
        },
        {
           "resize",
           " - change the size of the functions list",
           Action::Resize
        }
    };
}

//-----------------------------------------------------------------------------

FunctionCalculator::FunctionList FunctionCalculator::createFunctions()
{
    return FunctionList
    {
        std::make_shared<Sin>(),
        std::make_shared<Ln>()
    };
}

//-----------------------------------------------------------------------------

double FunctionCalculator::readArgs() {
    auto x = 0;
    if (m_readFile)
        m_iss >> x;
    else
        m_istr >> x;

    return x;
}

//-----------------------------------------------------------------------------

bool FunctionCalculator::setMaxSize() {    
        try {
            m_ostr << "\nEnter max number of functions:\n";
            m_istr >> m_maxFuncs;
            if (m_maxFuncs >= MIN_SIZE && m_maxFuncs <= MAX_SIZE)
                return true;
            else
                throw std::out_of_range("\nThe maximum number of functions must be 2-100\n");
        }
        catch (std::out_of_range& e) {
            m_ostr << e.what();
            return false;
        }
}

//-----------------------------------------------------------------------------
/*
    This method handle in case of full capacity in the list
*/
void FunctionCalculator::listCapacityHandler(Action action) {
        switch (action)
        {
        case Action::Invalid:
            throw std::out_of_range("\nCommand not found\n");
        case Action::Eval:   eval();             break;
        case Action::Del:    del();              break;
        case Action::Help:   help();             break;
        case Action::Exit:   exit();             break;
        case Action::Read:   read();             break;
        case Action::Resize: resize();           break;
        default:
            throw std::out_of_range
            ("\nList of functions is full, please delete some to add others...\n");
        }
}