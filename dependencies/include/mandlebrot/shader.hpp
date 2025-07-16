#pragma once

#include "main.h"

std::string readFile(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
