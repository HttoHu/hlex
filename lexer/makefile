target = bin/lexer
cc = g++ -O2 -std=c++17

SRC= $(wildcard ./src/*.cpp)
OBJ= $(addprefix obj/,$(patsubst %.cpp,%.o,$(notdir $(SRC))))
BIN= ./bin/lexer

define COM_INS 
@echo "\033[31m 正在编译$@ \033[0m"
@$(cc) -O2 -c $< -o $@
endef

# Dependencies
main_dep = subset-cons.h
subset_cons_dep = to-nfa.h
hopcroft_dep = subset-cons.h
parser_dep = subset-cons.h

all:$(target)

obj/main.o:$(addprefix ./src/, main.cpp) $(addprefix includes/,$(main_dep))
	$(COM_INS)
obj/to-nfa.o:$(addprefix ./src/, to-nfa.cpp)
	$(COM_INS)
obj/subset-cons.o: src/subset-cons.cpp $(addprefix includes/, $(subset_cons_dep))
	$(COM_INS)
obj/hopcroft.o: src/hopcroft.cpp $(addprefix includes/, $(hopcroft_dep))
	$(COM_INS)
obj/parser.o: src/parser.cpp $(addprefix includes/, $(parser_dep))
	$(COM_INS)

$(target):$(OBJ)
	@$(cc) -g $^ -o $(target) -lpthread
	@echo "\033[33m 生成完毕\033[0m"

.PHONY clean:
clean:
	@rm $(OBJ)
	@rm $(target)
run:
	@$(target)

back: 
	zip -r ./back.zip ./*
