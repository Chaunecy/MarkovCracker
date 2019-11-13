//
// Created by cw on 11/6/19.
//

#ifndef MARKOV_COMMON_H
#define MARKOV_COMMON_H

#include "include/cereal/types/unordered_map.hpp"
#include "include/cereal/types/memory.hpp"
#include "include/cereal/archives/binary.hpp"
#include "include/cereal/types/string.hpp"
#include "include/cmdline.h"
#include <fstream>
#include <iostream>
#include <cmath>

struct Count {
    int freq;
    double log_prob;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(freq, log_prob);
    }
};

typedef std::shared_ptr<Count> shared_count_ptr;
typedef std::unordered_map<char, shared_count_ptr> ChrCntMap;
typedef std::shared_ptr<ChrCntMap> shared_chr_cnt_map_ptr;

struct Transition {
    int total_freq;
    shared_chr_cnt_map_ptr chr_cnt_map_p;

    template<class Archive>
    void save(Archive &ar) const {
        ar(total_freq, chr_cnt_map_p);
    }

    template<class Archive>
    void load(Archive &ar) {
        ar(total_freq, chr_cnt_map_p);
    }
};


typedef std::shared_ptr<Transition> shared_transition_ptr;
typedef std::unordered_map<std::string, shared_transition_ptr> NGramMap;
typedef std::shared_ptr<NGramMap> shared_n_gram_map_ptr;
typedef std::unordered_map<int, shared_n_gram_map_ptr> LabelMap;
typedef std::shared_ptr<LabelMap> shared_label_map_ptr;

struct MarkovModel {
    int n_gram;
    shared_label_map_ptr label_map_p;

    template<class Archive>
    void save(Archive &ar) const {
        ar(n_gram, label_map_p);
    }

    template<class Archive>
    void load(Archive &ar) {
        ar(n_gram, label_map_p);
    }
};

typedef std::shared_ptr<MarkovModel> shared_markov_model_ptr;
typedef std::vector<int> labels_t;
const char END_CHR = 3;

template<typename T>
void save_grammar(T t, const char *grammar_file) {
    std::ofstream os(grammar_file, std::ios::binary);
    cereal::BinaryOutputArchive save(os);
    save(t);
}

template<typename T>
T load_grammar(const char *grammar_file) {
    std::ifstream is(grammar_file, std::ios::binary);
    cereal::BinaryInputArchive load(is);
    T t;
    load(t);
    return t;
}

inline std::string get_prefix(std::string &str, int idx, int prefix_len) {
    int start = std::max(0, idx - prefix_len);
    int n = idx - start;
    return str.substr(start, n);
}

inline char get_chr(std::string &str, int idx) {
    return str.at(idx);
}

#endif //MARKOV_COMMON_H
