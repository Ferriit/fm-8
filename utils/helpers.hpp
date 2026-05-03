#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace utils {
    enum Base {
        BIN,
        OCT,
        DEC,
        HEX
    };

    struct NumType {
        Base base;
        std::string num;
    };
};

namespace helpers {
    bool is_numeric(const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    }

    bool is_alpha(const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha);
    }
    
    bool is_alnum(const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isalnum);
    }
};

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

std::vector<std::string> split_whitespace(std::string data) {
    std::vector<std::string> out;
    out.push_back("");

    size_t idx = 0;
    for (char& c : data) {
        if ((c != '\n') && (c != ' ') && (c != '\t')) {
            out[idx] += c;
        }
        else {
            out.push_back("");
            idx++;
        }
    }
    return out;
}

std::vector<std::string> split_tokens(std::string data) {
    std::vector<std::string> out;
    out.push_back("");

    size_t idx = 0;
    for (char& c : data) {
        if ((c != '\n') && (c != ' ') && (c != '\t') && (c != ',')) {
            out[idx] += c;
        }
        else {
            out.push_back("");
            idx++;
        }
    }
    return out;
}


utils::NumType get_base(const std::string str) {
    if (str.empty()) {
        return {utils::Base::DEC, "0"};
    }

    utils::Base base = utils::Base::DEC;
    std::string num = str;
    size_t len = str.length();

    if (len >= 3 && str[0] == '0') {
        char prefix = std::tolower(static_cast<unsigned char>(str[1]));
        
        if (prefix == 'x') {
            base = utils::Base::HEX;
            num = str.substr(2);
        }
        else if (prefix == 'b') {
            base = utils::Base::BIN;
            num = str.substr(2);
        }
        else if (prefix == 'o') {
            base = utils::Base::OCT;
            num = str.substr(2);
        }
    }
    else if (len >= 2 && std::toupper(static_cast<unsigned char>(str[len - 1])) == 'H') {
        base = utils::Base::HEX;
        num = str.substr(0, len - 1);
    }

    return {base, num};
}

uint16_t to_int(utils::NumType data) {
    int base_int = 10;
    
    switch (data.base) {
        case utils::Base::HEX: base_int = 16; break;
        case utils::Base::BIN: base_int = 2;  break;
        case utils::Base::OCT: base_int = 8;  break;
        case utils::Base::DEC: base_int = 10; break;
    }

    try {
        return static_cast<uint16_t>(std::stoul(data.num, nullptr, base_int));
    } 
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid number format for " << data.num << std::endl;
        return 0;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: Number too large for 16-bit: " << data.num << std::endl;
        return 0;
    }
}
