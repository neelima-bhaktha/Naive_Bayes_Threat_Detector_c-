#include <iostream>
#include <string>
#include "naive_bayes.h"

int main() {
    std::cout << "=== Naive Bayes Spam Detector ===" << std::endl;
    std::cout << std::endl;
    
    // Create Naive Bayes classifier
    NaiveBayes classifier;
    
    // Define paths to training data
    std::string spam_dir = "../data/spam/";
    std::string ham_dir = "../data/ham/";
    
    // Train the model
    classifier.train(spam_dir, ham_dir);
    
    // Interactive prediction loop
    std::cout << "Enter a message to classify (or 'quit' to exit):" << std::endl;
    std::cout << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        
        // Check for exit command
        if (input == "quit" || input == "exit" || input == "q") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        
        // Skip empty input
        if (input.empty()) {
            continue;
        }
        
        // Predict and display result
        bool is_spam = classifier.predict(input);
        
        // Get scores for more detailed output
        double spam_score, ham_score;
        classifier.predictWithScores(input, spam_score, ham_score);
        
        std::cout << std::endl;
        std::cout << "Prediction: ";
        if (is_spam) {
            std::cout << "SPAM" << std::endl;
        } else {
            std::cout << "NOT SPAM" << std::endl;
        }
        
        std::cout << "Spam score: " << spam_score << std::endl;
        std::cout << "Ham score: " << ham_score << std::endl;
        std::cout << std::endl;
    }
    
    return 0;
}
