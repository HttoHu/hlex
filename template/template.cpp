#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace Lexer
{
    using std::map;
    using std::vector;
    using char_type = char;

    extern map<int, std::string> fin_stat_tab;
    extern vector<map<char_type, int>> tab;
    

    struct Token
    {
        std::string tag;
        std::string val;
        std::string to_string() const
        {
            return "<" + tag +"," + val + ">";
        }
    };

    

}
