//
// Created by cw on 10/29/19.
//

#include "common.h"

int main(int argc, char *argv[]) {
    cmdline::parser parser;
    const char *trained_model = "trained-model";
    parser.add<std::string>(trained_model, '\0', "the path of trained model", true, "");
    parser.parse_check(argc, argv);
    shared_n_gram_map_ptr markov_grammar_p = load_grammar<shared_n_gram_map_ptr>(
            parser.get<std::string>(trained_model).c_str());
    for (auto &prefix_transition: *markov_grammar_p) {
        std::string prefix = prefix_transition.first;
        shared_transition_ptr transition_p = prefix_transition.second;
        std::cout << prefix << std::endl;
        std::cout << transition_p->total_freq << std::endl;
    }

}


