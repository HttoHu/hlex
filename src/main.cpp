// #include "../includes/subset-cons.h"
#include "../includes/lexer.h"
#include "../includes/re.h"
#include <time.h>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Utils
{
    std::string read_file(const std::string &filename);
}
char buf[1024];
void gen_lexer(int argc, char **argv)
{
    using namespace Alg;

    if (argc != 3)
    {
        std::cerr << "invalid argument, usage: hlex [input] [output]\n";
        exit(1);
    }
    auto dir = std::filesystem::canonical(std::filesystem::path(argv[0])).parent_path().c_str();
    sprintf(buf, "%S", dir);
    std::string exe_path = buf;

    std::string file = Utils::read_file(argv[1]);
    Lexer::Scanner scan(file);
    Lexer::LexerGenerator L(scan);

    std::ofstream ofs(argv[1]);
    ofs << L.gen_code(exe_path+"/hlex/template.cpp");
    ofs.close();
}
void lex()
{
    std::string file = Utils::read_file("./rules.hlex");
    Lexer::Scanner scan(file);
    Lexer::LexerGenerator L(scan);

    auto ans = L.lex("i+j=23");
    for (auto item : ans)
        std::cout << item.to_string() << "\n";
}
void RE()
{
    Htto::ReExpr re("[_a-bA-B][_a-bA-B0-1]*");
    auto res = re.match("i");
    std::cout << res.start << " " << res.str << "\n";
}
int main(int argc, char **argv)
{
    try
    {
        // RE();
        gen_lexer(argc, argv);
        // lex();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
    }
    return 0;
}