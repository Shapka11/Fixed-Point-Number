#include "Parsing.h"


Parser::Parser(Data *data) : data_(data) {
}

void Parser::CheckCorrectRounding(const std::string &argc) {
    if (argc == "0" || argc == "1" || argc == "2" || argc == "3") {
        return;
    }
    std::cerr << "Error, incorrect rounding type" << std::endl;
    exit(EXIT_FAILURE);
}

void Parser::ParseRounding(const std::string &argc) {
    data_->type_rounding = std::stoi(argc);
}


void Parser::CheckCorrectOperation(const std::string &argc) {
    if (argc == "/" || argc == "-" || argc == "+" || argc == "*") {
        return;
    }
    std::cerr << "Error, incorrect operation" << std::endl;
    exit(EXIT_FAILURE);
}

void Parser::ParseOperation(const std::string &argc) {
    data_->operation = argc[0];
}


void Parser::CheckCorrectNumber(const std::string &argc) {
    if (argc.substr(0, 2) != "0x") {
        std::cerr << "Error, incorrect notation" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i < argc.size(); ++i) {
        if (!(argc[i] >= 'a' && argc[i] <= 'f' || argc[i] >= 'A' && argc[i] <= 'F' || argc[i] >= '0' && argc[i] <= '9')) {
            std::cerr << "Error, incorrect number" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    try {
        uint32_t num = std::stoul(argc, nullptr, 16);
    } catch (std::out_of_range) {
        std::cerr << "Error, too many number" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Parser::ParseNum(const std::string &argc, uint32_t &num) {
    num = std::stoul(argc, nullptr, 16);
    size_t size_num = CountBitsNum(num);
    if (size_num > data_->a + data_->b) {
        int shift = 32 - (data_->a + data_->b);
        num <<= shift;
        num >>= shift;
    }
}

int Parser::CountBitsNum(uint32_t num) {
    int size_num = 0;
    while (num != 0) {
        num = num >> 1;
        ++size_num;
    }
    return size_num;
}

bool Parser::IsNum(const std::string &num) {
    for (int i = 0; i < num.size(); ++i) {
        if (!(num[i] >= '0' && num[i] <= '9')) {
            return false;
        }
    }
    return true;
}


void Parser::CheckCorrectFormat(const std::string &argc) {
    if (argc.find('.') == argc.npos) {
        std::cerr << "Error, no separation of parts" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        std::string int_part = argc.substr(0, argc.find('.'));
        std::string frac_part = argc.substr(argc.find('.') + 1);

        if (!(IsNum(int_part) && IsNum(frac_part))) {
            std::cerr << "Error, incorrect format" << std::endl;
            exit(EXIT_FAILURE);
        }

        int int_num = std::stoi(int_part);
        int frac_num = std::stoi(frac_part);

        if (int_num + frac_num > 32) {
            std::cerr << "Error, A+B > 32" << std::endl;
            exit(EXIT_FAILURE);
        }
    } catch (std::invalid_argument) {
        std::cerr << "Error, incorrect format" << std::endl;
        exit(EXIT_FAILURE);
    } catch (std::out_of_range) {
        std::cerr << "Error, too big number" << std::endl;
        exit(EXIT_FAILURE);
    }

}

void Parser::ParseFormat(const std::string &argc) {
    std::string int_part = argc.substr(0, argc.find('.'));
    std::string frac_part = argc.substr(argc.find('.') + 1);
    data_->a = std::stoi(int_part);
    data_->b = std::stoi(frac_part);
}


void Parser::ParseFirstVariant(char **argc) {
    CheckCorrectFormat(argc[1]);
    ParseFormat(argc[1]);

    CheckCorrectRounding(argc[2]);
    ParseRounding(argc[2]);

    CheckCorrectNumber(argc[3]);
    ParseNum(argc[3], data_->num1);

}

void Parser::ParseSecondVariant(char **argc) {
    CheckCorrectFormat(argc[1]);
    ParseFormat(argc[1]);

    CheckCorrectRounding(argc[2]);
    ParseRounding(argc[2]);

    CheckCorrectOperation(argc[3]);
    ParseOperation(argc[3]);

    CheckCorrectNumber(argc[4]);
    ParseNum(argc[4], data_->num1);

    CheckCorrectNumber(argc[5]);
    ParseNum(argc[5], data_->num2);

}
