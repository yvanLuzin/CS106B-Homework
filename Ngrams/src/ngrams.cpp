// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "queue.h"
#include "map.h"
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "random.h"

using namespace std;

/* Function prototypes */
string generateRandomText(int numOfWords, Map<Queue<string>, Vector<string>> & map);
Map<Queue<string>, Vector<string>> buildMap(int nValue, ifstream &input);

/* Main function */
int main() {
    cout << "Welcome to CS 106B Random Writer (\"N-Grams\").\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n" << endl;

    ifstream input;
    string filename = promptUserForFile(input, "Input file name?", "No such file, try again");

    int nValue;
    while(true) {
        nValue = getInteger("Value of N?");
        if (nValue <= 1) {
            cout << "Value of N should be greater than 1." << endl;
        } else {
            break;
        }
    };

    Map<Queue<string>, Vector<string>> map = buildMap(nValue, input);

    int numOfWords;
    while(true) {
        numOfWords = getInteger("# of random words to generate (0 to quit)?");
        if (numOfWords == 0) {
            break;
        } else if (numOfWords < nValue ) {
            cout << "Number of words should be at least "  << nValue << endl;
        } else {
            cout << generateRandomText(numOfWords, map) << endl;
        }
    }

    cout << "Exiting." << endl;
    return 0;
}

/* Utility functions */

/*
 * function: buildMap(n, stream)
 * ----------------------------
 * Build and returns map of n-grams
 * from stream, and closes the stream.
 */
Map<Queue<string>, Vector<string>> buildMap(int nValue, ifstream &input) {
    Queue<string> window;
    Map<Queue<string>, Vector<string>> map;
    Vector<string> vector;

    string line;
    while (input >> line) {
        vector.add(line);
    }

    vector += vector.subList(0, nValue - 1);

    for (string word:vector) {
        if (window.size() == nValue - 1) {
            Vector<string> tempV = map.get(window);
            tempV.add(word);
            map.add(window, tempV);
            window.dequeue();
        }
        window.enqueue(word);
    }

    input.close();
    return map;
}

/*
 * function: generateRandomText(numberOfWords, map)
 * ----------------------------
 * Generates and returns random text
 * with numberOfWords length, using
 * map of n-grams as a dictionary.
 */
string generateRandomText(int numOfWords, Map<Queue<string>, Vector<string>> & map) {
    string result = "...";
    int random = randomInteger(0, map.size());
    Queue<string> entry = map.keys()[random];

    for (int i = 0; i < numOfWords; ++i) {
        Vector<string> nextVector = map.get(entry);
        int randomVElement = randomInteger(0, nextVector.size()-1);
        string nextWord = nextVector[randomVElement];
        entry.enqueue(nextWord);

        result += entry.dequeue();
        result += " ";
    }

    return result + "...";
}

