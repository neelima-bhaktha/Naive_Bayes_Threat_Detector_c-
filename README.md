# Naive Bayes Spam Detector

Simple Naive Bayes text classifier for spam detection.

## Project layout
- src/
  - main.cpp
  - naive_bayes.cpp
  - naive_bayes.h
- data/
  - spam/   (text files, one email per .txt)
  - ham/    (text files, one email per .txt)

## Requirements
- g++ (MinGW) or another C++17-capable compiler
- Windows (commands below use Windows paths)

## Build (from project root)
Open a Developer Command Prompt, PowerShell or cmd and run:
```bash
g++ -std=c++17 [main.cpp](http://_vscodecontentref_/0) [naive_bayes.cpp](http://_vscodecontentref_/1) -o nb.exe