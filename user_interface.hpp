#pragma once


#include <regex>
#include <string>
#include <iostream>
#include <vector>       // Для обработки массива строк

#include "Find_matches.hpp" // Для работы с режимами

/* Данные о программе */
class Program_data {

public:
    std::string name_of_program = "./Parse";
    std::string version = "0.1";
};



/* Класс проверки синтаксиса */
class Syntax {

protected:
    /* Ключи для работы из консоли */
    struct {
        std::string KEY_R = "r";    /* Для вывода количества совпадений */
        std::string KEY_O = "o";    /* Для вывода совпадающих строк */
        std::string KEY_F = "f";    /* Для записи в файл всех совпадений */
    } keys;

private:
    struct {    /* Флаги для проверки корректности синтакса */
        unsigned int is_name    : 1;
        unsigned int is_path    : 1;
        unsigned int is_key     : 1;
        unsigned int is_pattern : 1;
    } correct_syntax;

    Program_data pd;

protected:
    /* Введено просто имя файла*/
    std::string path_pattern_name = "[\\w]{1,}[\\.][\\w]{1,}";
    /* Путь к файлу */
    std::string path_pattern = "[/home]([/\\w]{1,}|[/\\w]{1,}[\\.][\\w]{1,})";
    // Конструктор по-умолчанию
    /*
        Инициализируем данные о программе
    */
    Syntax() = default;
    ~Syntax() = default;
    // Проверка синтаксиса ввода
    bool check_syntax(std::vector<std::string>&);
};


bool Syntax::check_syntax(std::vector<std::string>& args)
{
    std::regex regular_for_path_name_pattern(path_pattern_name);
    std::cmatch match_name;

    std::regex regular_for_path_pattern(path_pattern);
    std::cmatch match_path;

    // По умолчанию синтаксис неверный
    bool is_correct = false;

    // Выставляем по-умолчанию флаги корректности
    correct_syntax.is_key = 0;
    correct_syntax.is_name = 0;
    correct_syntax.is_path = 0;
    correct_syntax.is_pattern = 0;

    // Проверка размерности массива args
    if (args.size() >= 3 && args.size() <= 4) {
        // Константный итератор на начало массива
        // Защита от возможности изменения параметров
        std::vector<std::string>::const_iterator c_iter;

        for (c_iter = args.begin(); c_iter != args.end(); c_iter++)
        {
            // копирование содержимого c_iter
            std::string str = *c_iter;
            // Для отладки
            // std::cout << str << std::endl;

            if (*c_iter == pd.name_of_program) correct_syntax.is_name = 1;
            
            else if (std::regex_match(str.c_str(), match_name, regular_for_path_name_pattern) ||
                std::regex_match(str.c_str(), match_path, regular_for_path_pattern))
                correct_syntax.is_path = 1;

            else if ((*c_iter == keys.KEY_F) ||
                (*c_iter == keys.KEY_O) ||
                (*c_iter == keys.KEY_R) )
            {
                correct_syntax.is_key = 1;
            }
            
            else if (args.size() == 4) correct_syntax.is_pattern = 1;
        }
    }

    // Для отладки
    // std::cout << correct_syntax.is_key << "\n" 
    //           << correct_syntax.is_name << "\n"
    //           << correct_syntax.is_path << "\n"
    //         << correct_syntax.is_pattern << "\n";

    // std::cout << "\n";
    // Проверка выставленных флагов
    if (correct_syntax.is_name  && 
        correct_syntax.is_path  &&
        correct_syntax.is_key)
    {
        is_correct = true;
    }

    return is_correct;
}



/* 
   Класс с реализацией пользовательского 
   Интерфейса. Наследуется от класса Program_data
*/
class user_interface : protected Syntax {
    /* Интерфейс для взаимодействия с пользователем */
    /* Ключи r, o*/
    std::string path;
    std::string regular;
    std::string key;

    /* Массив с заполненными аргументами 
       из командной строки */
    std::vector<std::string> filled;

    /* Указатель на класс Find_matches */
    Find_matches* fm;

public:
    user_interface(int argc, char** argv) {
        // Заполняем массив filled
        for (int i = 0; i < argc; i++) { filled.push_back(argv[i]); }

        // Находим в этом массиве путь и регулярное выражение //
        // Для регулярных выражений 
        std::regex regular_for_path_name_pattern(path_pattern_name);
        std::cmatch match_name;

        std::regex regular_for_path_pattern(path_pattern);
        std::cmatch match_path;

        // Поиск пути и регулярного выражения
        std::vector<std::string>::const_iterator c_iter;
        for (c_iter = filled.begin(); c_iter != filled.end(); ++c_iter)
        {
            std::string iter_consist = *c_iter;
            if  (std::regex_match(iter_consist.c_str(), match_name, regular_for_path_name_pattern)||
                 std::regex_match(iter_consist.c_str(), match_path, regular_for_path_pattern))
                    path = iter_consist;
            
            else if (iter_consist == keys.KEY_O ||
                     iter_consist == keys.KEY_R ||
                     iter_consist == keys.KEY_F)
            {
                key = iter_consist;
            }
            else {
                regular = iter_consist;
            }
        }

        fm = new Find_matches(path, regular);
    }
    ~user_interface() { delete fm; };
    /*
        Если в функции check_args вылетает false
        то программа аварийно завершает свою работу
    */
    bool check_args();
    /*
        Проверка ключей.
        И выбор режима работы из файла Find_matches.hpp
    */
    void check_keys();
};

bool user_interface::check_args()
{
    bool is_correct = false;
    if (check_syntax(filled)) is_correct = true;

    return is_correct;
}


void user_interface::check_keys() 
{
    if (check_args()) {
        std::vector<std::string>::const_iterator c_iter;

        for (c_iter = filled.begin(); c_iter != filled.end(); ++c_iter)
        {
            // Считает вхождения
            if (*c_iter == keys.KEY_F) fm->send_matches("a.txt");
            // Печатает вхождения
            if (*c_iter == keys.KEY_O) fm->print_matches();
            // Создает файл и туда печатает вхождения
            if (*c_iter == keys.KEY_R) 
                std::cout << fm->count_of_matches() << std::endl;
        }    
    }
    // Ошибка
    exit(-1);
}