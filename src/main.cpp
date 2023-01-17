// #include "../includes/subset-cons.h"
#include "../includes/lexer.h"
#include <time.h>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace Utils
{
    std::string read_file(const std::string &filename);
}

void gen_lexer(int argc, char **argv)
{
    using namespace Alg;

    if (argc != 2)
    {
        std::cerr << "invalid argument, usage: hlex [output]\n";
        exit(1);
    }

    std::string file = Utils::read_file("./rules.hlex");
    Lexer::Scanner scan(file);
    Lexer::LexerGenerator L(scan);

    std::ofstream ofs(argv[1]);
    ofs << L.gen_code("./template/template.cpp");
    ofs.close();
}

int main(int argc, char **argv)
{
    gen_lexer(argc,argv);
    // std::string file = Utils::read_file("./rules.hlex");
    // Lexer::Scanner scan(file);
    // Lexer::LexerGenerator L(scan);

    // auto ans = L.lex("if(a<b)\n return 1.23");
    // for (auto item : ans)
    //     std::cout << item.to_string() << "\n";
    return 0;
}