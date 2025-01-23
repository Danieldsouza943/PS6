// Copyright 2024 Daniel Dsouza
#include <iostream>
#include <memory>
#include <algorithm>
#include <set>
#include <string>
#include <map>
#include <random>

using std::string, std::cout, std::cin, std::endl;

class RandWriter {
 private:
    std::map<string, std::map<char, int>> frequencies;
    size_t k;
    string text;

 public:
    // Create a Markov model of order k from given text
    // Assume that text has length at least k.
    RandWriter(const string& text, size_t k);

    size_t orderK() const { return k; }  // Order k of Markov model

    // Number of occurences of kgram in text
    // Throw an exception if kgram is not length k

    int freq(const string& kgram) const;
    // Number of times that character c follows kgram
    // if order=0, return num of times that char c appears
    // (throw an exception if kgram is not of length k)
    int freq(const string& kgram, char c) const;

    // Random character following given kgram
    // (throw an exception if kgram is not of length k)
    // (throw an exception if no such kgram)
    char kRand(const string& kgram);

    // Generate a string of length L characters by simulating a trajectory
    // through the corresponding Markov chain. The first k characters of
    // the newly generated string should be the argument kgram.
    // Throw an exception if kgram is not of length k.
    // Assume that L is at least k
    string generate(const string& kgram, size_t L);

    friend std::ostream& operator<<(std::ostream& os, const RandWriter& rw);
};
// Overload the stream insertion operator << and display the internal state
// of the Markov model. Print out the order, alphabet, and the frequencies
// of the k-grams and k+1-grams
