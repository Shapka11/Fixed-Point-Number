#include <iostream>

#include "Parsing.h"
#include "WorkWithNum.h"


int main(int argv, char **argc) {
    Data *data = new Data();
    Parser parser(data);
    if (argv == 4) {
        parser.ParseFirstVariant(argc);
        PrintNum(data->num1, data->a, data->b, data->type_rounding);
    } else if (argv == 6) {
        parser.ParseSecondVariant(argc);
        Operation op;
        uint32_t result = op.GetResult(data);
        PrintNum(result, data->a, data->b, data->type_rounding);
    } else {
        std::cerr << "Error, not enough arguments" << std::endl;
        return 0;
    }

    delete data;
    return 0;
}
