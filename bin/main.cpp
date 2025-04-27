#include <iostream>

#include <processing.h>
#include <vector>
#include <string>

// #include <Pipline.h>

/*
Программа считает частоту слов, встречающихся во всех файлах в директории 
и выводит итоговую статистику в консоль
*/

int main(int argc, char **argv) {
	// std::vector<std::string> input = {"name4", "name0", "name1", "name0", "name2", "name0", "name1"};

    std::vector<std::string> input = {"1, 2, 3, 4, 5" , "asdasd", "asdasd", "asdasd"};
    
    auto result = AsDataFlow(input) | Write(std::cout, '|');
}
