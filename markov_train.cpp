//
// Created by cw on 10/29/19.
//

#include "markov_train.h"

int main(int argc, char *argv[]) {
    markov_grammar = std::make_shared<NGramMap>();
    shared_count_ptr cnt_p = std::make_shared<Count>(Count{1, .1});
    shared_chr_cnt_map_ptr chr_cnt_map_p = std::make_shared<ChrCntMap>();
    chr_cnt_map_p->insert(std::make_pair('1', cnt_p));
    markov_grammar->insert(std::make_pair("hello", chr_cnt_map_p));
    shared_chr_cnt_map_ptr tmp = markov_grammar->at("hello");
    save_grammar<shared_n_gram_map_ptr>(markov_grammar, "out.cereal");
    shared_n_gram_map_ptr n_gram_map_p = load_grammar<shared_n_gram_map_ptr>("out.cereal");
    std::cout << "hello" << std::endl;
    std::cout << n_gram_map_p->at("hello") << std::endl;
    std::cout << n_gram_map_p->at("hello")->at('1')->freq << std::endl;
    std::cout << "world" << std::endl;
//    grammar.insert(std::pair())
    /*{
        std::ofstream os("out.cereal", std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        int age = 24;
        std::string name = "cw";
        MyRecord rec{
        };
        rec.x = 1;
        rec.y = 2;
        rec.z = 3.4f;
        archive(CEREAL_NVP(age), CEREAL_NVP(name), rec);
    }*/

}

template<typename T>
void save_grammar(T t, const char *grammar_file) {
    std::ofstream os(grammar_file, std::ios::binary);
    cereal::BinaryOutputArchive save(os);
    save(t);
//    std::cout << t->at("hello")->at('1')->freq << std::endl;
}

template<typename T>
T load_grammar(const char *grammar_file) {
    std::ifstream is(grammar_file, std::ios::binary);
    cereal::BinaryInputArchive load(is);
    T t;
    load(t);
    return t;
}
