#include "utils.h"
#include <iostream>
#include <string>

// –еализаци€ шаблона дл€ всех типов
template <typename T>
T safe_input(T min, T max, std::string message) {
    T attribute;
    while (!(std::cin >> attribute) || std::cin.peek() != '\n' || attribute < min || attribute > max) {
        std::cout << "¬ведите ";
        if (message == "None")
            std::cout << "число от " << min << " до " << max;
        else
            std::cout << message;
        std::cout << ": ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cerr << attribute << std::endl;
    return attribute;
}

// 
template <>
char safe_input<char>(char min, char max, std::string message) {
    char attribute;
    while (!(std::cin >> attribute) || std::cin.peek() != '\n' || attribute < min || attribute > max) {
        std::cout << "¬ведите ";
        if (message == "None")
            std::cout << "символ от " << min << " до " << max;
        else
            std::cout << message;
        std::cout << ": ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cerr << attribute << std::endl;
    return attribute;
}

// явна€ инстанциаци€ дл€ нужных типов 
template int safe_input<int>(int, int, std::string);
template double safe_input<double>(double, double, std::string);