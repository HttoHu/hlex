## Regex Usage 
1. `make` to build the project to test
2. include `includes/re.h`
```cpp
    Htto::ReExpr regex("[A-Za-z]+-[A-Za-z]+");
    
    auto res = regex.match("Hello-Now is 2023-01-15 Have a nice day");

    std::cout << res.start << " " << res.str << "\n";
```

## Lexer Principle
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
3. Hopocroft 算法（选做）：优化DFA

本代码大部分指针用法类似于week_ptr，每次传入会转移所有权，由接受的对象负责析构!!!.


### Tompson 算法
总的来说，就是使用 $\epsilon$ 实现正则表达式的并，连接，还有闭包。闭包就是重复，分为Kleene 闭包和正闭包分别代表重复0次或多次以及至少重复一次。

* 实现连接: 很简单，两个状态之间连上一个 $\epsilon$ 即可。
* 实现并：如图所示，实现 A|B

![实现 A|B](./img/01-tompson-or.drawio.png)

* 实现重复: R* 

![实现重复](.\img\02-tompson-repeat.drawio.png)

如果是正闭包就去掉下面那个 epsilon 飞线就行

实现难度不高，连图就行

### 子集构造法
子集构造法将 NFA转换为DFA

前置概念：

* $\epsilon$ 闭包: NFA 中一个状态或状态集合经过一个或多个 $\epsilon$ 走到的状态集合我们称为 $\epsilon$ 闭包（$\epsilon-closure$)
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


### 下面就是解析正则了

基本的算法搞完了，下面开始分析正则表达式了。干脆直接递归下降吧，简单方便。

先确定表达式优先级，写点产生式

Expr -> Term ( | Term )* // (xxx) * 类似于正则的那个repeat，这个比较方便递归下降的分析
     -> Term Term 
     
Term -> Factor * 
      | Factor +
      // OPT | Factor {number}  
      // OPT | Factor {number , number} 
      | Factor 
Factor -> (Expr)
       -> [Range]
          Atom 

Atom -> alpha_number 
     -> \escape_char 

### 正则表达式引擎的优化
使用 BitSet 替换 set<Node*> 或者 set<int> 将会得到几十倍的性能提升（主要提升在子集构造法，涉及到集合运算）