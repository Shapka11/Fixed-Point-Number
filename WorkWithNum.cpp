#include "WorkWithNum.h"


void Rounding::SetNum(const uint64_t &num) {
    num_ = num;
}

void Rounding::SetAAndB(const size_t &a, const size_t &b) {
    a_ = a;
    b_ = b;
}

void Rounding::SetNumberDigitsAfterB(const uint64_t &number_digits_after_b) {
    number_digits_after_b_ = number_digits_after_b;
}

void Rounding::SetSign(const bool &sign) {
    is_negative_ = sign;
}


uint64_t Rounding::GetResult(const int &type) {
    if (number_digits_after_b_ == 0) {
        return num_;
    }
    switch (type) {
    case 0:
        TowardZero();
        break;
    case 1:
        TowardNearestEven();
        break;
    case 2:
        TowardPosInfinity();
        break;
    case 3:
        TowardNegInfinity();
        break;
    default:
        break;
    }
    return num_;
}


void Rounding::TowardZero() {
    num_ >>= number_digits_after_b_;
}

void Rounding::TowardNearestEven() {
    uint64_t discarded_part = num_ << (64 - number_digits_after_b_ + 1);

    if (num_ << (64 - number_digits_after_b_) == 0) {
        num_ >>= number_digits_after_b_;
        return;
    }

    bool last_bit_b =  num_ << (64 - number_digits_after_b_ - 1) >> 63;

    if (number_digits_after_b_ == 1) {
        discarded_part = 0;
    }

    bool fisrt_bit_after_b = number_digits_after_b_ == 1 ? num_ & 1 : num_ << (64 - number_digits_after_b_) >> 63;
    if (fisrt_bit_after_b == 0) {
        num_ >>= number_digits_after_b_;
    } else {

        if (discarded_part == 0) {
            num_ >>= number_digits_after_b_;
            if (last_bit_b == 1) {
                ++num_;
            }
        } else {
            num_ >>= number_digits_after_b_;
            ++num_;
        }
    }

}

void Rounding::TowardPosInfinity() {
    uint64_t discarded_part = num_ << (64 - number_digits_after_b_);
    num_ >>= number_digits_after_b_;
    if (discarded_part != 0 && !is_negative_) {
        ++num_;
    }
}

void Rounding::TowardNegInfinity() {
    uint64_t discarded_part = num_ << (64 - number_digits_after_b_);
    num_ >>= number_digits_after_b_;
    if (discarded_part != 0 && is_negative_) {
        ++num_;
    }
}



void PrintNum(const uint32_t &num, const int &a, const int &b, const int &type_round) {
    std::string ans = "";
    std::string frac_ans = "";
    bool sign = num >> (a + b - 1);
    OutputNumber out_num;
    out_num.SetAAndB(a, b);
    out_num.SetSign(sign);

    if (sign == 1) {
        uint32_t int_part = (-num) << (32 - a - b) >> (32 - a);
        uint32_t frac_part = b == 0 ? 0 : (-num) << (32 - b) >> (32 - b);

        out_num.SetFracPart(frac_part);
        frac_part = out_num.GetResult(type_round) % PowNum(out_num.GetCntInsignificantZero(), 10);
        if (SizeNum(frac_part) == 4) {
            frac_part = 0;
            ++int_part;
        }

        ans +=  '-' + std::to_string(int_part) + ".";
        switch (out_num.GetCntInsignificantZero()) {
        case 1:
            frac_ans += "00" + std::to_string(frac_part);
            break;
        case 2:
            frac_ans += "0" + std::to_string(frac_part);
            break;
        default:
            frac_ans += std::to_string(frac_part);
            break;
        }
        AddZero(frac_ans);
        ans += frac_ans;
        RepairMinusZero(ans);
    } else {
        uint32_t int_part = num >> b;
        uint32_t frac_part = b == 0 ? 0 : num << (32 - b) >> (32 - b);

        out_num.SetFracPart(frac_part);
        frac_part = out_num.GetResult(type_round) % PowNum(out_num.GetCntInsignificantZero(), 10);
        if (SizeNum(frac_part) == 4) {
            frac_part = 0;
            ++int_part;
        }

        ans += std::to_string(int_part) + ".";

        switch (out_num.GetCntInsignificantZero()) {
        case 1:
            frac_ans += "00" + std::to_string(frac_part);
            break;
        case 2:
            frac_ans += "0" + std::to_string(frac_part);
            break;
        default:
            frac_ans += std::to_string(frac_part);
            break;
        }
        AddZero(frac_ans);
        ans += frac_ans;

    }
    std::cout << ans << std::endl;

}

uint64_t PowNum(const int &degree, const int &num) {
    uint64_t res = 1;
    for (int i = 0; i < degree; ++i) {
        res *= num;
    }
    return res;
}

bool DetermineSign(Data *data, const uint32_t &num) {
    return (num >> (data->a + data->b - 1));
}

size_t SizeNum(uint64_t num) {
    size_t res = 0;
    while (num > 0) {
        ++res;
        num /= 10;
    }
    return res;
}



void Operation::Sum(Data *data) {
    result_ = data->num1 + data->num2;
    result_ = result_ << (64 - data->a - data->b) >> (64 - data->a - data->b);
}

void Operation::Minus(Data *data) {
    result_ = data->num1 - data->num2;
    result_ = result_ << (64 - data->a - data->b) >> (64 - data->a - data->b);
}

void Operation::Multiplication(Data *data) {
    bool sign_num1 = DetermineSign(data, data->num1);
    bool sign_num2 = DetermineSign(data, data->num2);

    sign_res_ = sign_num1 ^ sign_num2;
    if (sign_num1) {
        data->num1 = -data->num1;
        data->num1 <<= (32 - data->a - data->b);
        data->num1 >>= (32 - data->a - data->b);
    }
    if (sign_num2) {
        data->num2 = -data->num2;
        data->num2 <<= (32 - data->a - data->b);
        data->num2 >>= (32 - data->a - data->b);
    }
    result_ = (uint64_t)data->num1 * data->num2;

    round.SetAAndB(data->a, data->b);
    round.SetNum(result_);
    round.SetNumberDigitsAfterB(data->b);
    round.SetSign(sign_res_);
    result_ = round.GetResult(data->type_rounding);

    if (sign_res_) {
        result_ = -result_;
    }

    result_ = result_ << (64 - data->a - data->b) >> (64 - data->a - data->b);
}

void Operation::Division(Data *data) {
    bool sign_num1 = DetermineSign(data, data->num1);
    bool sign_num2 = DetermineSign(data, data->num2);

    sign_res_ = sign_num1 ^ sign_num2;
    if (sign_num1) {
        data->num1 = -data->num1;
        data->num1 <<= (32 - data->a - data->b);
        data->num1 >>= (32 - data->a - data->b);
    }
    if (sign_num2) {
        data->num2 = -data->num2;
        data->num2 <<= (32 - data->a - data->b);
        data->num2 >>= (32 - data->a - data->b);
    }

    if (data->num2 == 0) {
        std::cout << "div_by_zero" << std::endl;
        exit(EXIT_SUCCESS);
    }

    uint64_t dividend = data->num1;
    dividend <<= (64 - data->a - data->b);
    result_ = (uint64_t)dividend / data->num2;

    //std::cout << "res " << result_ << std::endl;

    round.SetAAndB(data->a, data->b);
    round.SetNum(result_);
    round.SetNumberDigitsAfterB(64 - data->a - 2 * data->b);
    round.SetSign(sign_res_);
    result_ = round.GetResult(data->type_rounding);

    //std::cout << "res " << result_ << std::endl;

    if (sign_res_) {
        result_ = -result_;
    }
    //std::cout << "res " << result_ << std::endl;
    result_ = result_ << (64 - data->a - data->b) >> (64 - data->a - data->b);
    //std::cout << "res " << result_ << std::endl;
}



bool Operation::GetSignResult() {
    return sign_res_;
}

uint64_t Operation::GetResult(Data *data) {
    if (data->operation == '+') {
        Sum(data);
    } else if (data->operation == '-') {
        Minus(data);
    } else if (data->operation == '*') {
        Multiplication(data);
    } else {
        Division(data);
    }
    return result_;
}



void OutputNumber::SetFracPart(const uint32_t &frac_part) {
    frac_part_base_2_ = frac_part;
}

void OutputNumber::SetAAndB(const size_t &a, const size_t &b) {
    a_ = a;
    b_ = b;
}

void OutputNumber::SetSign(const bool &sign) {
    sign_ = sign;
}


uint32_t OutputNumber::GetResult(const int &type_round) {
    ConvertToBase10();
    size_num_ = SizeNum(frac_part_base_10_);
    cnt_round_digit_ = 3 - std::min(b_, kMaxSizeNumBase10_) + size_num_;
    bool disc_part_to_nearest_even = discarded_part_ + frac_part_base_10_ % PowNum(size_num_ - cnt_round_digit_ - 1, 10);
    discarded_part_ += frac_part_base_10_ % PowNum(size_num_ - cnt_round_digit_, 10);
    if (cnt_round_digit_ <= 0 && discarded_part_ == 0) {
        return 0;
    }

    switch (type_round) {
    case 0:
        TowardZero();
        break;
    case 1:
        TowardNearestEven(disc_part_to_nearest_even);
        break;
    case 2:
        TowardPosInfinity();
        break;
    case 3:
        TowardNegInfinity();
        break;
    default:
        break;
    }
    return frac_part_base_10_;
}

size_t OutputNumber::GetCntInsignificantZero() {
    return cnt_round_digit_;
}

void OutputNumber::ConvertToBase10() {
    if (b_ <= kMaxSizeNumBase10_) {
        frac_part_base_10_ = frac_part_base_2_ * PowNum(b_, 5);
    } else {
        frac_part_base_10_ = (frac_part_base_2_ >> (b_ - kMaxSizeNumBase10_)) * PowNum(kMaxSizeNumBase10_, 5);
        discarded_part_ = frac_part_base_2_ << (32  - (b_ - kMaxSizeNumBase10_));
    }
}



void OutputNumber::TowardZero() {
    frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
}

void OutputNumber::TowardNearestEven(bool disc_part) {
    if (size_num_ <= cnt_round_digit_) {
        return;
    }

    int last_sign_num = frac_part_base_10_ / PowNum(size_num_ - cnt_round_digit_, 10) % 10;
    int first_disc_digit = frac_part_base_10_ / PowNum(size_num_- cnt_round_digit_ - 1, 10) % 10;
    size_t size_before_round;
    if (first_disc_digit == 5 && !disc_part) {

        if (last_sign_num % 2 == 0) {
            frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
            size_before_round = SizeNum(frac_part_base_10_);
        } else {
            frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
            size_before_round = SizeNum(frac_part_base_10_);
            ++frac_part_base_10_;
        }

    } else if (first_disc_digit > 5 || (first_disc_digit == 5 && disc_part)) {
        frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
        size_before_round = SizeNum(frac_part_base_10_);
        ++frac_part_base_10_;
    } else {
        frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
        size_before_round = SizeNum(frac_part_base_10_);
    }

    if (size_before_round < SizeNum(frac_part_base_10_)) {
        ++cnt_round_digit_;
    }
}

void OutputNumber::TowardPosInfinity() {
    frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
    size_t size_before_round = SizeNum(frac_part_base_10_);
    if (discarded_part_ && sign_ == 0) {
        ++frac_part_base_10_;
    }
    if (size_before_round < SizeNum(frac_part_base_10_)) {
        ++cnt_round_digit_;
    }
}

void OutputNumber::TowardNegInfinity() {
    frac_part_base_10_ /= PowNum(size_num_ - cnt_round_digit_, 10);
    size_t size_before_round = SizeNum(frac_part_base_10_);
    if (discarded_part_ && sign_) {
        ++frac_part_base_10_;
    }
    if (size_before_round < SizeNum(frac_part_base_10_)) {
        ++cnt_round_digit_;
    }
}



void AddZero(std::string &num) {
    switch (num.size()) {
    case 0:
        num += "000";
        break;
    case 1:
        num += "00";
        break;
    case 2:
        num += "0";
        break;
    default:
        break;
    }
}

void RepairMinusZero(std::string &str) {
    bool is_null = true;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] >= '1' && str[i] <= '9') {
            is_null = false;
        }
    }
    if (is_null) {
        str = str.substr(1, str.size() - 1);
    }
}

