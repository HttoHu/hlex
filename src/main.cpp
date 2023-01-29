// #include "../includes/subset-cons.h"
#include "../includes/lexer.h"
#include "../includes/re.h"
#include <time.h>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>

namespace Utils
{
    std::string read_file(const std::string &filename);
}
char buf[1024];

std::string cur_dir(char *path)
{
    int len = strlen(path);
    int end_pos = len - 1;
    while (end_pos && path[end_pos] != '\\' && path[end_pos] != '/')
        end_pos--;
    return std::string(path, path + end_pos);
}
void gen_lexer(int argc, char **argv)
{
    using namespace Alg;

    if (argc != 3)
    {
        std::cerr << "invalid argument, usage: hlex [rule] [output]\n";
        exit(1);
    }
    std::string exe_path = cur_dir(argv[0]);
    std::string file = Utils::read_file(argv[1]);
    Lexer::Scanner scan(file);
    Lexer::LexerGenerator L(scan);
    std::ofstream ofs(argv[2]);
    ofs << L.gen_code(exe_path + "/hlex/template.txt");
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