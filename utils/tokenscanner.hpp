#include <vector>
#include <string>

class TokenScanner {
private:
    std::vector<std::string> tokens;
    size_t cursor = 0;

public:
    TokenScanner(std::vector<std::string> t) : tokens(t) {}

    std::string peek() {        // Check current
        if (is_at_end()) return "";
        return tokens[cursor];
    }

    std::string peek_next() {   // Check next
        if (cursor + 1 >= tokens.size()) return "";
        return tokens[cursor + 1];
    }

    std::string next() {        // Go to next
        if (is_at_end()) return "";
        return tokens[cursor++];
    }

    bool expect(std::string expected) { // Return 0 if expected doesn't match what's there
        if (peek() == expected) {
            cursor++;
            return true;
        }
        return false;
    }

    bool is_at_end() {
        return cursor >= tokens.size();
    }
};