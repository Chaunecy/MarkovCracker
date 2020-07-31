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
    unsigned int freq;
    /**
     * This is a trick.
     * convert double to unsigned long long.
     * We can treat it as key of unordered_map
     * */
    unsigned long long minus_log2_prob;
    unsigned int label;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(freq, minus_log2_prob, label);
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
    std::shared_ptr<std::vector<std::string>> vec;
    std::string val;
public:

    explicit Prefix(const std::vector<std::string> &vec) {
        this->vec = std::make_shared<std::vector<std::string>>();
        for (const auto &ele: vec) {
            std::cout << ele << std::endl;
            this->vec->push_back(ele);
        }
    }

    std::string getVal() {
        return this->val;
    }

    std::shared_ptr<std::vector<std::string>> getVec() {
        return this->vec;
    }

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
typedef std::shared_ptr<NPrefixMap> SharedNPrefixMapPtr;
typedef long Label;
typedef std::unordered_map<Label, SharedNPrefixMapPtr> LabelMap;
#endif //MARKOV_FLYMARKOV_H
