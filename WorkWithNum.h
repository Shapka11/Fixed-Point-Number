#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>

#include "Parsing.h"


class Rounding {
public:
    void SetNum(const uint64_t &num);
    void SetAAndB(const size_t &a, const size_t &b);
    void SetNumberDigitsAfterB(const uint64_t &number_digits_after_b);
    void SetSign(const bool &sign);
    uint64_t GetResult(const int &type);

private:
    void TowardZero();
    void TowardNearestEven();
    void TowardPosInfinity();
    void TowardNegInfinity();

    uint64_t num_;
    size_t a_;
    size_t b_;
    bool is_negative_;
    uint64_t number_digits_after_b_;

};

void PrintNum(const uint32_t &num, const int &a, const int &b, const int &type_round);
uint64_t PowNum(const int &degree, const int &num);
bool DetermineSign(Data *data, const uint32_t &num);
size_t SizeNum(uint64_t num);
void AddZero(std::string &num);
void RepairMinusZero(std::string &str);


class Operation {
public:
    uint64_t GetResult(Data *data);
    bool GetSignResult();

private:
    Rounding round;

    void Sum(Data *data);
    void Minus(Data *data);
    void Multiplication(Data *data);
    void Division(Data *data);

    uint64_t result_;
    bool sign_res_;
};



class OutputNumber {
public:
    void SetFracPart(const uint32_t &frac_part);
    void SetAAndB(const size_t &a, const size_t &b);
    void SetSign(const bool &sign);

    uint32_t GetResult(const int &type_round);
    size_t GetCntInsignificantZero();


private:
    bool discarded_part_ = 0;
    uint32_t frac_part_base_2_;
    uint64_t frac_part_base_10_;
    size_t a_;
    size_t b_;
    bool sign_;
    size_t cnt_round_digit_;
    size_t size_num_;

    void TowardZero();
    void TowardNearestEven(bool disc_part);
    void TowardPosInfinity();
    void TowardNegInfinity();

    void ConvertToBase10();

    const size_t kMaxSizeNumBase10_ = 19;

};
