// Copyright 2024 Daniel Dsouza
#include "RandWriter.hpp"
#include <vector>

RandWriter::RandWriter(const string& _text, size_t _k) : k(_k), text(_text) {
    if (text.length() < k) throw std::logic_error("Text length must be at least k");

    // Populating k-gram frequencies and next character frequencies
    for (size_t i = 0; i < text.length(); ++i) {
        // Extracting k-gram from text
        string kgram = text.substr(i, k);

        // Wrapping around if we reach the end of the text
        if (i + k > text.length()) {
            kgram += text.substr(0, (i + k) % text.length());
        }

        // Updating next character frequencies
        char nextChar = text[(i + k) % text.length()];
        frequencies[kgram][nextChar]++;
    }
}

int RandWriter::freq(const string &kgram) const {
    // Throwing exception if kgram is not of length k
    if (kgram.length() != k) throw std::invalid_argument("kgram must be of length k");

    // If the kgram is not found in the map, return 0
    if (frequencies.find(kgram) != frequencies.end()) {
        int freq = std::accumulate(frequencies.at(kgram).begin(), frequencies.at(kgram).end(), 0,
        [](int total, const auto& ent) { return total + ent.second; });
        return freq;
    }
    return 0;
}

int RandWriter::freq(const string &kgram, char c) const {
    // Throwing exception if kgram is not of length k
    if (kgram.length() != k) {
        throw std::invalid_argument("kgram must be of length k");
    }
    if (frequencies.find(kgram) == frequencies.end()) {
        throw std::invalid_argument("kgram not found in the model");
    }
    if (frequencies.at(kgram).find(c) ==  frequencies.at(kgram).end()) {
        return 0;
    }
    return frequencies.at(kgram).at(c);
}

char RandWriter::kRand(const string& kgram) {
    // Checking that the kgram is of length k and is part of the model
    if (kgram.length() != k) throw std::invalid_argument("kgram must be of length k");
    if (frequencies.find(kgram) ==  frequencies.end())
    throw std::invalid_argument("kgram not found in the model");


    // Getting the total frequency of all characters following the kgram
    const auto& charFreqs = frequencies.at(kgram);
    uint totalFreq = 0;
    std::vector<char> dist;
    std::vector<double> probabilities;

    for (const auto& ent : charFreqs) {
        totalFreq += ent.second;
        dist.push_back(ent.first);
    }

    for (const auto& ent : charFreqs) {
        probabilities.push_back(static_cast<double>(ent.second) / totalFreq);
    }

    // Generating a random character following the kgram based on the frequencies
    std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
    std::default_random_engine engine(std::random_device {}());

    return dist[distribution(engine)];
}

string RandWriter::generate(const string& kgram, size_t L) {
    // Checking that the kgram is of length k and is part of the model
    if (kgram.length() != k) throw std::invalid_argument("kgram must be of length k");
    if (frequencies.find(kgram) == frequencies.end())
    throw std::invalid_argument("kgram not found in the model");

    string res = kgram;

    for (size_t i = 0; i < L - k; i++) {
        string newKgram = res.substr(i, k);
        res += kRand(newKgram);
    }

    return res;
}

std::ostream& operator<<(std::ostream& os, const RandWriter& rw) {
    os << "Markov Model Order: " << rw.orderK() << endl;

    os << "Alphabet: ";
    std::set<char> alphabet;
    for (char c : rw.text) {
        alphabet.insert(c);
    }
    for (char c : alphabet) {
        os << c << " ";
    }
    os << std::endl;

    os << "Frequencies:" << std::endl;
    for (const auto& entry : rw.frequencies) {
        int totalFreq = 0;
        os << entry.first << ": ";
        for (const auto& innerEntry : entry.second) {
            os << innerEntry.first << "->" << innerEntry.second << ", ";
            totalFreq += innerEntry.second;
        }
        os << "| total: " << totalFreq << std::endl;
    }

    return os;
}
