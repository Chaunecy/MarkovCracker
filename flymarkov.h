//
// Created by cw on 7/31/20.
//

#ifndef MARKOV_FLYMARKOV_H
#define MARKOV_FLYMARKOV_H

#include <fstream>
#include <iostream>
#include <cmath>
#include "include/cereal/types/unordered_map.hpp"
#include "include/cereal/types/memory.hpp"
#include "include/cereal/archives/binary.hpp"
#include "include/cereal/types/string.hpp"
#include "include/cmdline.h"

struct Count {
    int freq;
    double log_prob;
    int label;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(freq, log_prob, label);
    }
};

struct Seg {
    std::string val;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(val);
    }
};

class Prefix {
private:
    std::vector<std::string> vec;
    std::string val;
public:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(vec, val);
    }

    bool operator==(const Prefix &p) {
        return this->val == p.val;
    }
};


typedef std::shared_ptr<Count> SharedCntPtr;
typedef std::shared_ptr<Seg> SharedSegPtr;
typedef std::unordered_map<SharedSegPtr, SharedCntPtr> SegCntMap;
typedef std::shared_ptr<SegCntMap> SharedSegCntMapPtr;

struct Transition {
    int total_freq;
    SharedSegCntMapPtr sharedSegCntMapPtr;

    template<class Archive>
    void save(Archive &ar) const {
        ar(total_freq, sharedSegCntMapPtr);
    }

    template<class Archive>
    void load(Archive &ar) {
        ar(total_freq, sharedSegCntMapPtr);
    }
};

typedef std::shared_ptr<Prefix> SharedPrefixPtr;
typedef std::unordered_map<SharedPrefixPtr, SharedSegCntMapPtr> NPrefixMap;
#endif //MARKOV_FLYMARKOV_H
