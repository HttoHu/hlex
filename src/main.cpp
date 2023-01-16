// #include "../includes/subset-cons.h"
#include "../includes/lexer.h"

#include <time.h>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace Test
{
    namespace Hopcroft
    {
        void case1();
    }
}
namespace ReParser
{
    namespace Test
    {
        void case1();
    }
}
std::string read_file(const std::string &filename)
{
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    if (!ifs)
        throw std::runtime_error("open file " + filename + " failed\n");
    char *file_content;
    ifs.seekg(0, std::ios::end);
    size_t file_length = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    file_content = new char[file_length];

    ifs.read(file_content, file_length);
    ifs.close();

    std::string str(file_content, file_content + file_length);
    delete[] file_content;
    return str;
}

int main()
{
    using namespace Alg;
    // freopen("output.txt", "w",stdout);
    time_t s = clock();
    std::string file = read_file("./rules.hlex");
    Lexer::Scanner scan(file);
    Lexer::LexerGenerator L(scan);
    auto vec = L.lex("while printf 123+1.23+234ull+0ll+31.4 if");
    for (auto token : vec)
        std::cout << token.to_string();
    time_t e = clock();
    // std::cout << (double)(e - s) / CLOCKS_PER_SEC;
    return 0;
}