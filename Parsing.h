#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>


struct Data {
    int a;
    int b;
    int type_rounding;
    char operation;
    uint32_t num1 = 0;
    uint32_t num2 = 0;
};

class Parser {
public:
    Parser(Data *data);
    void ParseFirstVariant(char **argc);
    void ParseSecondVariant(char **argc);

private:
    Data *data_;

    void CheckCorrectRounding(const std::string &argc);
    void CheckCorrectOperation(const std::string &argc);
    void CheckCorrectNumber(const std::string &argc);
    void CheckCorrectFormat(const std::string &argc);

    void ParseFormat(const std::string &argc);
    void ParseOperation(const std::string &argc);
    void ParseRounding(const std::string &argc);
    void ParseNum(const std::string &argc, uint32_t &num);

    int CountBitsNum(uint32_t num);
    bool IsNum(const std::string &num);

};
