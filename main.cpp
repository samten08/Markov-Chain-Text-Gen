#include "markov.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

int checkNumber(string answer) {
    if (answer == "") {
        return -1;
    }

    int number = 0;
    int length = answer.length();

    for (int i = 0; i < length; i++) {
        if (answer[i] < '0' || answer[i] > '9') {
            return -1;
        }
        number = number * 10 + (answer[i] - '0');
    }

    return number;
}

int main() {
    srand(time(0));

    const int MAX_WORDS = 5000;
    string words[MAX_WORDS];
    string prefixes[MAX_WORDS];
    string suffixes[MAX_WORDS];

    string filename;
    int order;
    int requestedWords;
    string answer;

    cout << "File: ";
    getline(cin, filename);

    while (true) {
        cout << "Order (1-3): ";
        cin >> answer;
        order = checkNumber(answer);

        if (order >= 1 && order <= 3) {
            break;
        }

        cout << "Enter 1, 2, or 3." << endl;
    }

    while (true) {
        cout << "Enter max words: ";
        cin >> answer;
        requestedWords = checkNumber(answer);

        if (requestedWords >= order) {
            break;
        }

        cout << "Enter at least " << order << "." << endl;
    }

    int numWords = readWordsFromFile(filename, words, MAX_WORDS);

    if (numWords == -1) {
        cout << "Could not open file." << endl;
        return 1;
    }

    if (numWords <= order) {
        cout << "File needs at least " << order + 1
             << " words." << endl;
        return 1;
    }

    if (numWords == MAX_WORDS) {
        cout << "Only first " << MAX_WORDS
             << " words were used." << endl;
    }

    int chainSize = buildMarkovChain(words, numWords, order,
                                     prefixes, suffixes, MAX_WORDS);

    if (chainSize <= 0) {
        cout << "Could not build chain." << endl;
        return 1;
    }

    string output = generateText(prefixes, suffixes, chainSize,
                                 order, requestedWords);

    int actualWords = 0;

    if (output != "") {
        actualWords = 1;
        int outputLength = output.length();

        for (int i = 0; i < outputLength; i++) {
            if (output[i] == ' ') {
                actualWords++;
            }
        }
    }

    cout << endl;
    cout << "Generated text:" << endl;
    cout << output << endl << endl;
    cout << "Generated " << actualWords << " of "
         << requestedWords << " words." << endl;

    if (actualWords < requestedWords) {
        cout << "Stopped early, no next word."
             << endl;
    }

    return 0;
}