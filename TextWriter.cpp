// Copyright 2024 Daniel Dsouza
#include "RandWriter.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <order> <length> < input_file.txt" << std::endl;
        return 1;
    }

    int order = std::stoi(argv[1]);
    int length = std::stoi(argv[2]);

    std::string inputText;
    std::string line;
    while (std::getline(std::cin, line)) {
        inputText += line;
    }

    RandWriter rw(inputText, order);

    string res = rw.generate(inputText.substr(0, order), length);

    cout << res << endl;

    return 0;
}
