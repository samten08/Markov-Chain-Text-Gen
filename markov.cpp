#include "markov.h"

#include <cstdlib>
#include <fstream>

using namespace std;

string joinWords(const string words[], int startIndex, int count) {
    string result = "";

    for (int i = 0; i < count; i++) {
        result += words[startIndex + i];

        if (i < count - 1) {
            result += " ";
        }
    }

    return result;
}

int readWordsFromFile(string filename, string words[], int maxWords) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return -1;
    }

    int count = 0;

    while (count < maxWords && inputFile >> words[count]) {
        count++;
    }

    inputFile.close();
    return count;
}

int buildMarkovChain(const string words[], int numWords, int order,
                     string prefixes[], string suffixes[],
                     int maxChainSize) {
    if (order < 1 || order > 3 || numWords <= order || maxChainSize <= 0) {
        return 0;
    }

    int count = 0;

    for (int i = 0; i < numWords - order && count < maxChainSize; i++) {
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order];
        count++;
    }

    return count;
}

string getRandomSuffix(const string prefixes[], const string suffixes[],
                       int chainSize, string currentPrefix) {
    if (chainSize <= 0) {
        return "";
    }

    int matchCount = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            matchCount++;
        }
    }

    if (matchCount == 0) {
        return "";
    }

    int pick = rand() % matchCount;
    int currentMatch = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (currentMatch == pick) {
                return suffixes[i];
            }
            currentMatch++;
        }
    }

    return "";
}

string getRandomPrefix(const string prefixes[], int chainSize) {
    if (chainSize <= 0) {
        return "";
    }

    int index = rand() % chainSize;
    return prefixes[index];
}

string generateText(const string prefixes[], const string suffixes[],
                    int chainSize, int order, int numWords) {
    if (chainSize <= 0 || order < 1 || order > 3 || numWords < order) {
        return "";
    }

    string currentPrefix = getRandomPrefix(prefixes, chainSize);
    string result = currentPrefix;
    string currentWords[3];

    int wordIndex = 0;
    string temp = "";

    for (int i = 0; i < static_cast<int>(currentPrefix.length()); i++) {
        if (currentPrefix[i] == ' ') {
            currentWords[wordIndex] = temp;
            wordIndex++;
            temp = "";
        } else {
            temp += currentPrefix[i];
        }
    }

    currentWords[wordIndex] = temp;

    for (int i = 0; i < numWords - order; i++) {
        string newWord = getRandomSuffix(prefixes, suffixes,
                                         chainSize, currentPrefix);

        if (newWord == "") {
            break;
        }

        result += " " + newWord;

        for (int j = 0; j < order - 1; j++) {
            currentWords[j] = currentWords[j + 1];
        }

        currentWords[order - 1] = newWord;
        currentPrefix = joinWords(currentWords, 0, order);
    }

    return result;
}
