#pragma once
#include <string>
#include <vector>

class func2client {
public:
    static std::string sha384(const std::string& message);

private:
    static std::vector<uint8_t> pad_message(const std::string& message);
    static void process_block(const uint8_t* block, uint64_t* H);
};
