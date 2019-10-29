//
// Created by cw on 10/29/19.
//

#ifndef MARKOV_MARKOV_TRAIN_H
#define MARKOV_MARKOV_TRAIN_H

#include "include/cereal/types/unordered_map.hpp"
#include "include/cereal/types/memory.hpp"
#include "include/cereal/archives/binary.hpp"
#include "include/cereal/types/string.hpp"
#include <fstream>
#include <iostream>

struct MyRecord {
    uint8_t x, y;
    float z;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(x, y, z);
    }
};

struct Count {
    long freq;
    float log_prob;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(freq, log_prob);
    }
};

typedef std::shared_ptr<Count> shared_count_ptr;
typedef std::unordered_map<char, shared_count_ptr> ChrCntMap;
typedef std::shared_ptr<ChrCntMap> shared_chr_cnt_map_ptr;
typedef std::unordered_map<std::string, shared_chr_cnt_map_ptr> NGramMap;
typedef std::shared_ptr<NGramMap> shared_n_gram_map_ptr;
shared_n_gram_map_ptr markov_grammar;

template<typename T>
void save_grammar(T t, const char *grammar_file);

template<typename T>
T load_grammar(const char *grammar_file);

#endif //MARKOV_MARKOV_TRAIN_H
