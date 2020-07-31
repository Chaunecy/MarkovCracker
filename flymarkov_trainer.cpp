//
// Created by cw on 7/31/20.
//

#include "flymarkov.h"

int main(int args, char **argv) {
    SharedNPrefixMapPtr snpMapPtr = std::make_shared<NPrefixMap>();
    std::vector<std::string> hh;
    std::cout << "hello" << std::endl;
    hh.emplace_back("ac");
    std::cout << "hello" << std::endl;
    SharedPrefixPtr prefixPtr = std::make_shared<Prefix>(hh);
    auto kk = prefixPtr->getVec();

    return 0;
}