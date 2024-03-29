## Regex Usage 
1. `make` to build the project to test
2. include `includes/re.h`
```cpp
    Htto::ReExpr regex("[A-Za-z]+-[A-Za-z]+");
    
    auto res = regex.match("Hello-Now is 2023-01-15 Have a nice day");

    std::cout << res.start << " " << res.str << "\n";
```

## Lexer Usage 
1. `make` to build the project to test, the target binary will generated into `bin/hlex`
2. write a rule file, and every rule ended with $

   there are four type rules: 
   * keywords: The hlex can not distinct keywords from symbols, so you need to specify them.
   * ignore: Some tokens are useless for following phases, you can ignore it.
   * user_def: You can define your method to scan special tokens which the DFA is difficult to process. 
   * tag definitions: `[tag_name] : [regular expression]`, hlex makes different tokens to different tags by their regular expression.

   here is an example 
```
[keywords]{
    IF: if,
    WHILE: while
}$

[ignore]{
    SPACE,ANNOTATION
}$

# Here is a simple string scanner. You need to define the LITERAL first.
# The user_def function is (const std::string &s,int &pos) -> string, the tag of the token still remains and the value is what you returned.

[user_def,LITERAL]{
    pos++;
    std::map<char, std::string> escape = {
        {'n', "\n"}, {'t', "\t"}, {'r', "\r"}, {'\\', "\\"}};
    std::string ret = "";
    while (pos < s.size() && s[pos] != '\"')
    {
        if (s[pos] == '\\')
        {
            if (pos == s.size())
                throw std::runtime_error("invalid string");
            pos++;
            if (!escape.count(s[pos]))
                throw std::runtime_error("invalid escape string!");
            ret += escape[s[pos]];
        }
        else
            ret += s[pos];
        pos++;
    }
    pos++;
    return ret;
}$
[user_def,ANNOTATION]{
    while(pos<s.size() && s[pos]!='\n')
        pos++;
    return "";
}$

ANNOTATION://$
NUMBER: [0-9]+$
LONG:[0-9]+ll$
SYMBOL: [_a-zA-Z][_a-zA-Z0-9]*$
REAL:[0-9]+.[0-9]+$
DOT:.$
COMMA:,$

# LITERAL is scaned by user-define function. 
LITERAL:"$
ADD:\+$
SUB:-$
MUL: \*$
DIV:/$

ASSIGN: =$

COMMA: ;$
# compare operation 
LT: <$
GT: >$
LE: <=$
GE: >=$

LPAR:\($
RPAR:\)$
SPACE: \s|\t$
NEWLINE:\n$
```
4. run `./bin/lexer [rule-file] [output-path]` to generate your lexer   
5. add the main function to test

```cpp
namespace HLex...

int main()
{
    HLex::Lexer L(read_file("test.txt"));
    auto vec = L.lex();
    for (auto item : vec)
    {
        std::cout << "<" + L.tag_to_str(item.tag) << "," << item.val << ">";
    }
}
```

4.  test cases(using upper rule) 

```
//===========================test-case 1==========================================
INPUT:
int max(int a,int b)
{
	if(a > b)
		return a;
	return b;
}

OUTPUT:
<INT,int><SYMBOL,max><LPAR,(><INT,int><SYMBOL,a><COMMA,,><INT,int><SYMBOL,b><RPAR,)><NEWLINE,
><BEGIN,{><NEWLINE,
><IF,if><LPAR,(><SYMBOL,a><GT,>><SYMBOL,b><RPAR,)><NEWLINE,
><RETURN,return><SYMBOL,a><SEMI,;><NEWLINE,
><RETURN,return><SYMBOL,b><SEMI,;><NEWLINE,
><END,}>
//===========================test-case 2==========================================
INPUT:
// this is an ANNOTATION. the output will ignore me
int a = 3.1415+1ull-1ll+"hello World\n";

OUTPUT:
<INT,int><SYMBOL,a><ASSIGN,=><REAL,3.1415><NUMBER,+1><SYMBOL,ull><LONG_NUMBER,-1ll><ADD,+><LITERAL,hello World
><SEMI,;><NEWLINE,
>
```

## Update Log
v2.0 
* 生成的Lexer 使用 enum 作为 tag 而不是字符串
* 修复了许多bug
* 修改了模版
* 修改了examples中c-lang.hlex 的部分错误

## hlex principle
构造一个自动的词法分析器，能够分析如下规则的正则表达式

[中科大的华保健编译原理](https://www.bilibili.com/video/BV16h411X7JY/?spm_id_from=333.999.0.0)

```
[a-z], from a to z
[0-9] from 0 to 9
[_a-zA-Z0-9] 
| : a|b ; hello world | hello earth
concat: abcdefg
* : Kleene closure
+ : positive closure
some indent chars.
\s: space
```

### 需要实现的Lab(算法)
1. Tompson 算法分析正则表达式，构造出NFA
2. 子集构造算法将NFA转换为DFA，目的是生成一个确定的转换表
3. Hopocroft 算法：优化DFA

本代码大部分指针用法类似于week_ptr，每次传入会转移所有权，由接受的对象负责析构!!!.


### Tompson 算法
总的来说，就是使用 $\epsilon$ 实现正则表达式的并，连接，还有闭包。闭包就是重复，分为Kleene 闭包和正闭包分别代表重复0次或多次以及至少重复一次。

* 实现连接: 很简单，两个状态之间连上一个 $\epsilon$ 即可。
* 实现并：如图所示，实现 A|B

![实现 A|B](./img/01-tompson-or.drawio.png)

* 实现重复: R* 

![实现重复](./img/02-tompson-repeat.drawio.png)

如果是正闭包就去掉下面那个 epsilon 飞线就行

实现难度不高，连图就行。由于我们不仅仅做正则匹配，还要做词法分析，我们要在图结束状态添加 tag。

### 子集构造法
子集构造法将 NFA转换为DFA

前置概念：

* $\epsilon$ 闭包: NFA 中一个状态或状态集合经过一个或多个 $\epsilon$ 走到的状态集合我们称为 $\epsilon$ 闭包（$\epsilon -closure$)
* $move$ 函数， $move(T,a)$ 能从 $T$ 中某个状态 $s$ 出发通过 $a$ 符号转换到达的NFA状态集合。

算法思想

1.  我们先处理一下所有单个NFA状态的$\epsilon-closure$ 函数。

2. 我们将 NFA 初始状态 $s_0$ 可以经过 $\epsilon$ 闭包的状态集合作为 DFA 的初始状态 $T_0$，然后不停的计算 $T_0$ 经过一些符号能到达的状态集合(计算move函数) $T_1,T_2,T_3 \cdots$ 然后不停的 dfs 直到没有新的状态集合产生。

实现见: subset-cons.cpp 

### Hopcroft 算法
Hopcroft 算法，就是不停的划分集合，有几个状态可以合并，但是我们可以自顶向下，先划分成两个大状态集合，然后不停的切分。

step1. 首先划分出两个大集合，一个是非fin state的状态集合，一个是fin状态的集合。

step2. 我们考虑所有的边，我们要确保同一个集合中所有状态吃到同一个字符转移到同一个集合，不然就要分割直到满足条件。

算法原理非常简单，只不过我实现的比较复杂了。这又是一个 fixed pointer iterative.

**为了实现词法分析器的自动生成，我们在第一步首先要将tag不同的fin状态划分为不同的集合。**

### 下面就是解析正则了

基本的算法搞完了，下面开始分析正则表达式了。干脆直接递归下降吧，简单方便。

先确定表达式优先级，写点产生式
```
Expr -> Term ( | Term )* // (xxx) * 类似于正则的那个repeat，这个比较方便递归下降的分析
     -> Term Term 
     
Term -> Factor * 
     -> Factor +
      // OPT | Factor {number}  
      // OPT | Factor {number , number} 
     -> Factor 

Factor -> (Expr)
       -> [Range]
          Atom 

Atom -> alpha_number 
     -> \escape_char 
```
### 正则表达式引擎的优化
使用 BitSet 替换 set<Node*> 或者 set<int> 将会得到几十倍的性能提升（主要提升在子集构造法，涉及到集合运算）