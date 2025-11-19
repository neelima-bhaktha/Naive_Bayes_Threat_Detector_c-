#ifndef NAIVE_BAYES_H
#define NAIVE_BAYES_H

#include <string>
#include <unordered_map>
#include <vector>

class NaiveBayes {
private:
    // Word frequency maps
    std::unordered_map<std::string, int> spam_word_count;
    std::unordered_map<std::string, int> ham_word_count;
    
    // Total word counts
    int total_spam_words;
    int total_ham_words;
    
    // Document counts
    int spam_doc_count;
    int ham_doc_count;
    
    // Vocabulary size (unique words across all documents)
    int vocabulary_size;
    
    // Laplace smoothing parameter (alpha)
    double alpha;
    
    // Helper functions
    std::vector<std::string> tokenize(const std::string& text);
    std::string toLowerCase(const std::string& str);
    std::string readFile(const std::string& filepath);
    
public:
    NaiveBayes();
    
    // Train the model with spam and ham directories
    void train(const std::string& spam_dir, const std::string& ham_dir);
    
    // Predict whether a text is spam or ham
    // Returns true for spam, false for ham
    bool predict(const std::string& text);
    
    // Get prediction with confidence scores
    void predictWithScores(const std::string& text, double& spam_score, double& ham_score);
};

#endif // NAIVE_BAYES_H