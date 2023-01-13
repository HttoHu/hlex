#include "../includes/subset-cons.h"
namespace Test
{
    namespace Hopcroft
    {
        using namespace Alg;
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
    int cnt=0;
    while (true)
    {
        cnt++;
        printf("%d\n",cnt);
        ReParser::Test::case1();
    }
    return 0;
}