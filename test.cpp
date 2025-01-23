// Copyright 2024 Daniel Dsouza
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "RandWriter.hpp"

BOOST_AUTO_TEST_CASE(k_zero) {
    std::string text = "abcdefghijklmnopqrstuvwxyz";
    size_t k = 0;  // Model will be 0th order Markov chain
    RandWriter rw(text, k);

    // Checking correct order is stored
    BOOST_CHECK_EQUAL(rw.orderK(), 0);

    // Checking that correct frequency for characters is stored
    BOOST_CHECK_EQUAL(rw.freq("", 'a'), 1);

    // Checking that total k-gram frequency is correct
    BOOST_CHECK_EQUAL(rw.freq(""), text.length());

    // Checking that exception is thrown for invalid k-gram
    BOOST_REQUIRE_THROW(rw.freq("a"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(no_error_checking) {
    // Initializing RandWriter with a sample text and a valid k value
    std::string text = "This is a test text for RandWriter.";
    size_t k = 5;  // Model will be 5th order Markov chain
    RandWriter rw(text, k);

    // Testing freq with a valid k-gram that exists in the text
    std::string kgram = "This ";
    BOOST_CHECK_NO_THROW({
        int freq = rw.freq(kgram);
        BOOST_CHECK_GE(freq, 1);  // Ensure the frequency is greater than or equal to 1
    });

    // Testing freq with a valid k-gram and following character that exists
    char followingChar = 'i';
    BOOST_CHECK_NO_THROW({
        int freq = rw.freq(kgram, followingChar);
        BOOST_CHECK_GE(freq, 1);  // Ensure the frequency is greater than or equal to 1
    });
}

BOOST_AUTO_TEST_CASE(invalid_kgram) {
    // Initializing RandWriter with a sample text and a valid k value
    std::string text = "This is a test text for RandWriter.";
    size_t k = 5;
    RandWriter rw(text, k);

    std::string kgram = "This ";

    // Testing generate with a valid k-gram and length
    size_t genLength = 20;  // Length of the string to generate
    BOOST_CHECK_NO_THROW({
        std::string generatedText = rw.generate(kgram, genLength);
        BOOST_CHECK_EQUAL(generatedText.size(), genLength);
        BOOST_CHECK_EQUAL(generatedText.substr(0, k), kgram);  // Ensure it starts with kgram
    });
}

BOOST_AUTO_TEST_CASE(krand_works) {
    // Initializing RandWriter with a sample text and a valid k value
    std::string text = "This is a test text for RandWriter.";
    size_t k = 5;
    RandWriter rw(text, k);

    std::string kgram = "This ";

    // Checking orderK() returns the correct k value
    BOOST_CHECK_EQUAL(rw.orderK(), k);

    // Testing kRand with a valid k-gram
    BOOST_CHECK_NO_THROW({
        char randomChar = rw.kRand(kgram);

        // Since we can't predict the random outcome, just check if it's a char
        BOOST_CHECK(isalpha(randomChar) || ispunct(randomChar) || isspace(randomChar));
    });
}
