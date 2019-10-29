//
// Created by cw on 10/29/19.
//

#include "markov_guess.h"
#include "markov_train.h"
int main(int argc, char *argv[]) {

    std::ifstream is("out.cereal", std::ios::binary);
    cereal::BinaryInputArchive archive(is);
    int age = 0;
    std::string name;
    MyRecord rec{};
    rec.x = 1;
    rec.y = 2;
    rec.z = .0f;
    archive(age, name, rec);

    std::cout << "Age: " << age << ", Name: " << name << ", rec: " << rec.z << std::endl;
}