#pragma once

#include <vector>
#include <string>

std::vector<std::string> split_lines(std::string data) {
    std::vector<std::string> out;
    out.push_back("");

    size_t idx = 0;
    for (char& c : data) {
        if (c != '\n') {
            out[idx] += c;
        }
        else {
            out.push_back("");
            idx++;
        }
    }
    return out;
}
