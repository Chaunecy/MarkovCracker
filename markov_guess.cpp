//
// Created by cw on 10/29/19.
//

#include "common.h"

const int MAX_STACK_SIZE = 64;
struct Stack {
    int sum{};
    int ptr{};
    int arr[MAX_STACK_SIZE]{};
};
int counter = 0;
int step = 0;

void dfs(int target, const labels_t &labels, Stack &stack);

int main(int argc, char *argv[]) {
    cmdline::parser parser;
    const char *trained_model = "trained-model";
    parser.add<std::string>(trained_model, '\0', "the path of trained model", true, "");
    parser.parse_check(argc, argv);
    shared_label_map_ptr label_map_p = load_grammar<shared_label_map_ptr>(
            parser.get<std::string>(trained_model).c_str());
    for (const auto &entry: *label_map_p) {
        std::cout << entry.first << std::endl;
        shared_n_gram_map_ptr n_gram_map_p = entry.second;
        if (n_gram_map_p->find("") != n_gram_map_p->end() &&
            n_gram_map_p->at("")->chr_cnt_map_p->find('h') != n_gram_map_p->at("")->chr_cnt_map_p->end()) {
            std::cout << n_gram_map_p->at("")->chr_cnt_map_p->at('h') << std::endl;
        }
    }
//    double max = -100;
//    std::string max_str = "default-max";
//    char max_chr = '\0';
//    double min = 100;
//    std::string min_str = "default-min";
//    char min_chr = '8';
//    int counters[20] = {0};
//    for (const auto &prefix_transition: *markov_grammar_p) {
//        std::string prefix = prefix_transition.first;
//        shared_transition_ptr transition_p = prefix_transition.second;
//        for (auto &chr_cnt: *transition_p->chr_cnt_map_p) {
//            char chr = chr_cnt.first;
//            double log_prob = chr_cnt.second->log_prob;
//            counters[lround(-log_prob)] += 1;
//            if (log_prob > max) {
//                max = log_prob;
//                max_str = prefix;
//                max_chr = chr;
//            }
//            if (log_prob < min) {
//                min = log_prob;
//                min_str = prefix;
//                min_chr = chr;
//            }
//        }
//    }
//    for (int i = 0; i < 20; i++) {
//        std::cout << "i: " << i << ", num: " << counters[i] << std::endl;
//    }
//    std::cout << "max: " << max << ", n-gram: " << max_str << "-" << max_chr
//              << ", min: " << min << ", n-gram: " << min_str << "-" << min_chr << "." << std::endl;

    labels_t labels;
    for (int i = 0; i < 25; i++) {
        labels.push_back(i + 1);
    }
    Stack stack;
    dfs(10, labels, stack);

}

int generate_pwd(Stack &stack, const shared_label_map_ptr &label_map_p) {
    for (int i = 0, len = stack.ptr; i < len; i++) {

    }
    return 0;
}

/**
 *
 * @param target
 * @param labels ascending sorted
 * @param stack
 */

void dfs(int target, const labels_t &labels, Stack &stack) {
    if (stack.sum == target) {
        if (counter == 100) {
            counter = 0;
            step += 1;
            std::cout << "step: " << step << std::endl;
        }
        counter += 1;
        return;
    }
    if (stack.ptr >= MAX_STACK_SIZE) {
        return;
    }
    int diff = target - stack.sum;
    for (int label : labels) {
        if (label > diff) {
            return;
        } else {
            stack.arr[stack.ptr] = label;
            stack.ptr += 1;
            stack.sum += label;
            dfs(target, labels, stack);
            stack.ptr -= 1;
            stack.sum -= label;
        }
    }
}

