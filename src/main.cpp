// #include "../includes/subset-cons.h"
#include "../includes/re.h"

#include <time.h>
#include <unordered_map>
#include <iostream>

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
int main()
{
    using namespace Alg;
    using namespace Htto;

    time_t s = clock();

    ReExpr regex("[A-Za-z]+-[A-Za-z]+");
    
    auto res = regex.match("Hello-Now is 2023-01-15 Have a nice day");
    
    std::cout << res.start << " " << res.str << "\n";

    time_t e = clock();
    std::cout << (double)(e - s) / CLOCKS_PER_SEC;
    return 0;
}