#include "markov.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

int main() {
    srand(time(0));

    const int MAX_WORDS = 5000;
    string words[MAX_WORDS];
    string prefixes[MAX_WORDS];
    string suffixes[MAX_WORDS];

    string filename;
    int order;
    int requestedWords;

    cout << "Enter input filename: ";
    getline(cin, filename);

    while (true) {
        cout << "Enter order (1, 2, or 3): ";

        if (cin >> order && order >= 1 && order <= 3) {
            break;
        }

        cout << "Please enter 1, 2, or 3." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), 10);
    }

    while (true) {
        cout << "Enter maximum number of words to generate: ";

        if (cin >> requestedWords && requestedWords >= order) {
            break;
        }

        cout << "Please enter a number that is at least " << order << "." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), 10);
    }

    int numWords = readWordsFromFile(filename, words, MAX_WORDS);

    if (numWords == -1) {
        cout << "Error: Could not open that file." << endl;
        return 1;
    }

    if (numWords <= order) {
        cout << "Error: The file needs at least " << order + 1
             << " words for this order." << endl;
        return 1;
    }

    if (numWords == MAX_WORDS) {
        cout << "Note: Only the first " << MAX_WORDS
             << " words were used." << endl;
    }

    int chainSize = buildMarkovChain(words, numWords, order,
                                     prefixes, suffixes, MAX_WORDS);

    if (chainSize <= 0) {
        cout << "Error: The Markov chain could not be built." << endl;
        return 1;
    }

    string output = generateText(prefixes, suffixes, chainSize,
                                 order, requestedWords);

    int actualWords = 0;
    string oneWord;
    istringstream counter(output);

    while (counter >> oneWord) {
        actualWords++;
    }

    cout << endl;
    cout << "Generated text:" << endl;
    cout << output << endl << endl;
    cout << "Generated " << actualWords << " of at most "
         << requestedWords << " words." << endl;

    if (actualWords < requestedWords) {
        cout << "Stopped early because the current prefix had no successor."
             << endl;
    }

    return 0;
}
