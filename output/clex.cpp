#include "test.h"
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
    try
    {
        HLex::Lexer L(read_file("./test.cpp"));
        auto vec = L.lex();
        for (auto item : vec)
        {
            std::cout << "<" + L.tag_to_str(item.tag) << "," << item.val << ">";
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }
}