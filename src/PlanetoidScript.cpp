#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Value.hpp"
#include "Parser.hpp"
#include "Token.hpp"

Interpreter G_interpreter;

void Evaluate(const std::string& input, const std::string& source)
{
    Lexer lexer(source, input);
    std::vector<Token> tokens = lexer.generateTokens();

    if (tokens.size() == 0)
        return;

    Parser parser(tokens);
    TokenNode* node = parser.Parse();

    if (!node)
        return;

    G_interpreter.Interpret(node);
}

void Bench(const std::string& input, const std::string& source)
{
    Lexer lexer(source, input);
    std::vector<Token> tokens = lexer.generateTokens();

    if (tokens.size() == 0)
        return;

    Parser parser(tokens);
    TokenNode* node = parser.Parse();

    if (!node)
        return;

    auto start = std::chrono::high_resolution_clock::now();
    G_interpreter.Interpret(node);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}

void Verify(const std::string& input)
{
    Lexer lexer("eval", input);
    std::vector<Token> tokens = lexer.generateTokens();
    if (tokens.size() == 0)
    {
        std::cout << "Invalid input.\n";
    }
    
    Parser parser(tokens);
    TokenNode* node = parser.Parse();
    if (!node)
    {
        std::cout << "Invalid input.\n";
    }
    else
    {
        std::cout << "Valid input.\n";
    }
}

void RanAsExecutable()
{
    // While the input isn't "exit", prompt for an input following a ">"
    // and print the input to the console.
    std::string input;
    while (input != "exit")
    {
        std::cout << "> ";
        // Get full line of input.
        std::getline(std::cin, input);

        if (input == "help")
        {
            std::cout << "PlanetoidScript v0.2\n\n";
            std::cout << "eval <...> - Evaluate an expression\n";
            std::cout << "verify <...> - Verify an expression\n";
            std::cout << "verifyfile [filename] - Verify a file\n]";
            std::cout << "load [filename] - Load a file\n";
            std::cout << "generate [filename] - Generate a file\n";
            std::cout << "bench <...> - Benchmark an expression\n";
            std::cout << "benchfile [filename] - Benchmark a file\n";
            std::cout << "exit - Exit the program\n";
        }
        else if (input.substr(0, 4) == "eval")
        {
            Evaluate(input.substr(5, input.length()), "eval");
        }
        else if (input.substr(0, 6) == "verify")
        {
            Verify(input.substr(7, input.length()));
        }
        else if (input.substr(0, 10) == "verifyfile")
        {
            std::cout << "Loading file " << input.substr(5, input.length()) << '\n';
            // Open the file using C++'s file I/O.
            std::fstream file;
            file.open(input.substr(5, input.length()), std::ios::in);
            if (file.is_open())
            {
                // Load the file into a string.
                std::string fileContents;
                file.seekg(0, std::ios::end);
                fileContents.resize(file.tellg());
                file.seekg(0, std::ios::beg);
                file.read(&fileContents[0], fileContents.size());
                file.close();

                // Evaluate the file.
                Verify(fileContents);
            }
            else
            {
                std::cout << "Failed to open file.\n";
            }
        }
        else if (input.substr(0, 4) == "load")
        {
            std::cout << "Loading file " << input.substr(5, input.length()) << '\n';
            // Open the file using C++'s file I/O.
            std::fstream file;
            file.open(input.substr(5, input.length()), std::ios::in);
            if (file.is_open())
            {
                // Load the file into a string.
                std::string fileContents;
                file.seekg(0, std::ios::end);
                fileContents.resize(file.tellg());
                file.seekg(0, std::ios::beg);
                file.read(&fileContents[0], fileContents.size());
                file.close();

                // Evaluate the file.
                Evaluate(fileContents, input.substr(5, input.length()));
            }
            else
            {
                std::cout << "Failed to open file.\n";
            }
        }
        else if (input.substr(0, 8) == "generate")
        {
            std::cout << "Generating file " << input.substr(9, input.length()) << '\n';
            // Create a file with the name specified using C++'s file I/O.
            std::fstream file;
            file.open(input.substr(9, input.length()), std::ios::out);
            file.close();
        }
        else if (input.substr(0, 9) == "benchfile")
        {
            std::cout << "Loading file " << input.substr(5, input.length()) << '\n';
            // Open the file using C++'s file I/O.
            std::fstream file;
            file.open(input.substr(10, input.length()), std::ios::in);
            if (file.is_open())
            {
                // Load the file into a string.
                std::string fileContents;
                file.seekg(0, std::ios::end);
                fileContents.resize(file.tellg());
                file.seekg(0, std::ios::beg);
                file.read(&fileContents[0], fileContents.size());
                file.close();

                // Evaluate the file.
                Bench(fileContents, input.substr(10, input.length()));
            }
            else
            {
                std::cout << "Failed to open file.\n";
            }
        }
        else if (input.substr(0, 5) == "bench")
        {
            Bench(input.substr(6, input.length()), "bench");
        }
        else if (input != "exit")
        {
            std::cout << input << '\n';
        }
    }
}

int main(int argc, char** argv) 
{
    std::cout << "PlanetoidScript v0.1\n";
    switch (argc)
    {
        case 1:
            RanAsExecutable();
            break;
        case 2:
            {
                std::cout << "Loading file " << argv[1] << '\n';
                // Open the file using C++'s file I/O.
                std::fstream file;
                file.open(argv[1], std::ios::in);
                if (file.is_open())
                {
                    // Load the file into a string.
                    std::string fileContents;
                    file.seekg(0, std::ios::end);
                    fileContents.resize(file.tellg());
                    file.seekg(0, std::ios::beg);
                    file.read(&fileContents[0], fileContents.size());
                    file.close();

                    // Evaluate the file.
                    Evaluate(fileContents, argv[1]);
                }
                else
                {
                    std::cout << "Failed to open file.\n";
                }
            }
            break;
        case 3:
            {
                std::cout << "Loading file " << argv[1] << '\n';
                // Open the file using C++'s file I/O.
                std::fstream file;
                file.open(argv[1], std::ios::in);
                if (file.is_open())
                {
                    // Load the file into a string.
                    std::string fileContents;
                    file.seekg(0, std::ios::end);
                    fileContents.resize(file.tellg());
                    file.seekg(0, std::ios::beg);
                    file.read(&fileContents[0], fileContents.size());
                    file.close();

                    if (std::string(argv[2]) == "-bench")
                        Bench(fileContents, argv[1]);
                    else if (std::string(argv[2]) == "-verify")
                        Verify(fileContents);
                    else
                        std::cout << "Invalid argument " << argv[2] << '\n';
                }
                else
                {
                    std::cout << "Failed to open file.\n";
                }
            }
            break;
        default:
            std::cout << "Invalid arguments" << '\n';
            break;
    }
}
