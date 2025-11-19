#include "naive_bayes.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cctype>
#include <dirent.h>  // For directory reading (cross-platform alternative)
#include <sys/stat.h>

NaiveBayes::NaiveBayes() 
    : total_spam_words(0), 
      total_ham_words(0), 
      spam_doc_count(0), 
      ham_doc_count(0),
      vocabulary_size(0),
      alpha(1.0)
{
}

std::string NaiveBayes::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<std::string> NaiveBayes::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string current_word;
    
    for (char c : text) {
        if (std::isalpha(c)) {
            current_word += c;
        } else {
            if (!current_word.empty()) {
                tokens.push_back(toLowerCase(current_word));
                current_word.clear();
            }
        }
    }
    
    if (!current_word.empty()) {
        tokens.push_back(toLowerCase(current_word));
    }
    
    return tokens;
}

std::string NaiveBayes::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open file " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper function to check if path ends with .txt
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void NaiveBayes::train(const std::string& spam_dir, const std::string& ham_dir) {
    std::cout << "Training Naive Bayes model..." << std::endl;
    
    spam_word_count.clear();
    ham_word_count.clear();
    total_spam_words = 0;
    total_ham_words = 0;
    spam_doc_count = 0;
    ham_doc_count = 0;
    
    // Process spam files
    std::cout << "Loading spam emails from: " << spam_dir << std::endl;
    DIR* dir = opendir(spam_dir.c_str());
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string filename = entry->d_name;
            if (endsWith(filename, ".txt")) {
                std::string filepath = spam_dir + filename;
                std::string content = readFile(filepath);
                std::vector<std::string> tokens = tokenize(content);
                
                for (const std::string& word : tokens) {
                    spam_word_count[word]++;
                    total_spam_words++;
                }
                spam_doc_count++;
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Warning: Could not open spam directory: " << spam_dir << std::endl;
    }
    
    // Process ham files
    std::cout << "Loading ham emails from: " << ham_dir << std::endl;
    dir = opendir(ham_dir.c_str());
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string filename = entry->d_name;
            if (endsWith(filename, ".txt")) {
                std::string filepath = ham_dir + filename;
                std::string content = readFile(filepath);
                std::vector<std::string> tokens = tokenize(content);
                
                for (const std::string& word : tokens) {
                    ham_word_count[word]++;
                    total_ham_words++;
                }
                ham_doc_count++;
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Warning: Could not open ham directory: " << ham_dir << std::endl;
    }
    
    // Calculate vocabulary size
    std::unordered_map<std::string, bool> unique_words;
    for (const auto& pair : spam_word_count) {
        unique_words[pair.first] = true;
    }
    for (const auto& pair : ham_word_count) {
        unique_words[pair.first] = true;
    }
    vocabulary_size = unique_words.size();
    
    std::cout << "Training complete!" << std::endl;
    std::cout << "Spam documents: " << spam_doc_count << std::endl;
    std::cout << "Ham documents: " << ham_doc_count << std::endl;
    std::cout << "Vocabulary size: " << vocabulary_size << std::endl;
    std::cout << "Total spam words: " << total_spam_words << std::endl;
    std::cout << "Total ham words: " << total_ham_words << std::endl;
    std::cout << std::endl;
}

void NaiveBayes::predictWithScores(const std::string& text, double& spam_score, double& ham_score) {
    std::vector<std::string> tokens = tokenize(text);
    
    int total_docs = spam_doc_count + ham_doc_count;
    double log_prior_spam = std::log(static_cast<double>(spam_doc_count) / total_docs);
    double log_prior_ham = std::log(static_cast<double>(ham_doc_count) / total_docs);
    
    spam_score = log_prior_spam;
    ham_score = log_prior_ham;
    
    for (const std::string& word : tokens) {
        int spam_count = spam_word_count.count(word) ? spam_word_count[word] : 0;
        double prob_word_given_spam = (spam_count + alpha) / 
                                       (total_spam_words + alpha * vocabulary_size);
        spam_score += std::log(prob_word_given_spam);
        
        int ham_count = ham_word_count.count(word) ? ham_word_count[word] : 0;
        double prob_word_given_ham = (ham_count + alpha) / 
                                      (total_ham_words + alpha * vocabulary_size);
        ham_score += std::log(prob_word_given_ham);
    }
}

bool NaiveBayes::predict(const std::string& text) {
    double spam_score, ham_score;
    predictWithScores(text, spam_score, ham_score);
    return spam_score > ham_score;
}