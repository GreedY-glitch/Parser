#pragma once


#include <string>
#include <fstream>
#include <regex>
#include <iostream>


/* 
   Класс с функциями для поиска всех
   Совпадений в файле по шаблону
*/
class Find_matches  
{
    std::string path;
    std::string pattern;
    
    // Для чтения файлов
    std::ifstream fp;

public:
    Find_matches(std::string ph, std::string pn) : path(ph), 
                                                   pattern(pn) {}
    ~Find_matches() = default;

    // Нахождение всех совпадений
    int count_of_matches();
    // Печать всех совпадений
    void print_matches();
    // Запись всех совпадений в файлик
    void send_matches(std::string);
};

// C++:
int Find_matches::count_of_matches() {
    fp.open(path, std::ios::in);

    int counter = 0;    
    try {
        // Строка для считывания
        std::string line;

        // Модуль для библитеки анализа регулярных выражений
        std::regex regular_expr(pattern);
        std::cmatch match;

        while (!fp.eof()) {
            // считываем строку из файла
            getline(fp, line);
            if (std::regex_search(line.c_str(), match, regular_expr))
                ++counter;
        }

        fp.close();
    /* Проверка корректности открытия файла */
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return counter;
}

void Find_matches::print_matches() {
    fp.open(path, std::ios::in);

    try 
    {   
        // Строка для считывания
        std::string line;

        // Модуль для библитеки анализа регулярных выражений
        std::regex regular_expr(pattern);
        std::cmatch match;

        while (!fp.eof()) {
            // считываем строку из файла
            getline(fp, line);
            if (std::regex_search(line.c_str(), match, regular_expr))
                std::cout << line << std::endl;
        }

        fp.close();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Find_matches::send_matches(std::string _destination_path)
{
    fp.open(path, std::ios::in);

    // Открытие файла для записи данных 
    std::ofstream fp_destination(_destination_path, std::ios::out | std::ios::binary);

    try 
    {   
        // Строка для считывания
        std::string line;

        // Модуль для библитеки анализа регулярных выражений
        std::regex regular_expr(pattern);
        std::cmatch match;

        while (!fp.eof()) {
            // считываем строку из файла
            getline(fp, line);
            if (std::regex_search(line.c_str(), match, regular_expr))
                fp_destination << line << std::endl;
        }

        fp_destination.close();
        fp.close();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}