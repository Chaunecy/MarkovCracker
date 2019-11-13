//
// Created by cw on 10/29/19.
//

#include "common.h"

shared_n_gram_map_ptr n_gram_p = nullptr;

void train(const char *training_set, int prefix_len, int from_len, int to_len);

shared_label_map_ptr generate_label_map();

int main(int argc, char *argv[]) {
    cmdline::parser parser;
    const char *training_set = "training-set", *n_gram = "n-gram",
            *trained_model = "trained-model", *from_len = "from-len", *to_len = "to-len";
    parser.add<std::string>(training_set, '\0', "the path of training set", true, "");
    parser.add<int>(n_gram, '\0', "the order of markov process", true, 4);
    parser.add<std::string>(trained_model, '\0', "the path to put trained model", true, "");
    parser.add<int>(from_len, '\0', "use passwords with length longer than from-len", false, 1);
    parser.add<int>(to_len, '\0', "use passwords with length shorter than to-len", false, 255);
    parser.parse_check(argc, argv);
    int from = parser.get<int>(from_len);
    int to = parser.get<int>(to_len);
    if (from > to) {
        std::cerr << "from-len should be less than or equal to to-len" << std::endl;
        std::exit(-1);
    }
    n_gram_p = std::make_shared<NGramMap>();
    train(parser.get<std::string>(training_set).c_str(), parser.get<int>(n_gram) - 1,
          parser.get<int>(from_len), parser.get<int>(to_len));
    shared_label_map_ptr label_map_p = generate_label_map();
    shared_markov_model_ptr markov_model_p = std::make_shared<MarkovModel>(MarkovModel{
            parser.get<int>(n_gram), label_map_p});
    save_grammar(markov_model_p, parser.get<std::string>(trained_model).c_str());
}

/**
 * count the n-gram in training_set,
 * password with length [from_len, to_len] will be counted; others will be discarded
 * @param training_set
 * @param prefix_len    n-gram - 1
 * @param trained_model
 * @param from_len
 * @param to_len
 */
void train(const char *training_set, const int prefix_len, int from_len, int to_len) {
    std::ifstream fin(training_set, std::ios::in);
    if (!fin.is_open()) {
        std::cerr << "can not open training_set: " << training_set << std::endl;
        return;
    }
    std::string line;
    while (getline(fin, line)) {
        const int line_len = line.size();
        // parse password with length between from-len and to-len
        if (line_len > to_len || line_len < from_len) {
            continue;
        }
        line += END_CHR;
        for (int i = 0, len = line.size(); i < len; i++) {
            std::string prefix = get_prefix(line, i, prefix_len);
            // can not find prefix in grammar
            if (n_gram_p->find(prefix) == n_gram_p->end()) {
                // create an entry
                shared_chr_cnt_map_ptr chr_cnt_map_p = std::make_shared<ChrCntMap>();
                shared_transition_ptr transition_p = std::make_shared<Transition>(Transition{0, chr_cnt_map_p});
                n_gram_p->insert(std::make_pair(prefix, transition_p));
            }
            // get the transition of prefix
            shared_transition_ptr transition_p = n_gram_p->at(prefix);
            char chr = get_chr(line, i);
            int cnt = 1;
            if (transition_p->chr_cnt_map_p->find(chr) == transition_p->chr_cnt_map_p->end()) {
                // create chr_cnt_map
                shared_count_ptr count_p = std::make_shared<Count>(Count{1, 0});
                transition_p->chr_cnt_map_p->insert(std::make_pair(chr, count_p));
            } else {
                transition_p->chr_cnt_map_p->at(chr)->freq += 1;
            }
            transition_p->total_freq += 1;
        }
    }

    for (auto &prefix_transition: *n_gram_p) {
        std::string prefix = prefix_transition.first;
        shared_transition_ptr transition_p = prefix_transition.second;
        for (auto &chr_cnt : *transition_p->chr_cnt_map_p) {
            shared_count_ptr count_p = chr_cnt.second;
            count_p->log_prob = log((double) (count_p->freq * 1.0 / transition_p->total_freq));
        }
    }
}

int calc_label(double log_prob) {
    int label = (int) lround(2 * (-log_prob) + .5);
    if (1 == label) {
        label = 2;
    }
    return label;
}

shared_label_map_ptr generate_label_map() {
    shared_label_map_ptr label_map_p = std::make_shared<LabelMap>();
    for (const auto &iter: *n_gram_p) {
        std::string prefix = iter.first;
        shared_transition_ptr transition_p = iter.second;
        for (const auto &it: *transition_p->chr_cnt_map_p) {
//            shared_count_ptr count_p = it.second;
            double log_prob = it.second->log_prob;
            int label = calc_label(log_prob);
            if (label_map_p->find(label) == label_map_p->end()) {
                label_map_p->insert(std::make_pair(label, std::make_shared<NGramMap>()));
            }
            shared_n_gram_map_ptr tmp_n_gram_map_p = label_map_p->at(label);
            // not exist, create a new entry
            if (tmp_n_gram_map_p->find(prefix) == tmp_n_gram_map_p->end()) {
                shared_chr_cnt_map_ptr tmp_chr_cnt_map_p = std::make_shared<ChrCntMap>();
                tmp_n_gram_map_p->insert(std::make_pair(prefix, std::make_shared<Transition>(Transition{
                        0, tmp_chr_cnt_map_p
                })));
            }
            shared_transition_ptr tmp_transition_p = tmp_n_gram_map_p->at(prefix);
            if (tmp_transition_p->chr_cnt_map_p->find(it.first) == tmp_transition_p->chr_cnt_map_p->end()) {
                tmp_transition_p->chr_cnt_map_p->insert(std::make_pair(it.first, it.second));
            }
        }
    }
    return label_map_p;
}

