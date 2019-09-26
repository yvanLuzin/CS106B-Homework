// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "simpio.h"
#include "console.h"
#include "stack.h"
#include "queue.h"
#include "lexicon.h"
#include "filelib.h"

using namespace std;

string promptForWord(int order);
void printResults(Stack<string> s);

void getUserInput(string& word1, string& word2, Lexicon dict) {
    while(true) {
        word1 = promptForWord(1);
        if (word1.length() == 0) return;
        word2 = promptForWord(2);
        if (word2.length() == 0) return;

        if (word1.length() != word2.length()) {
            cout << "The two words must be the same length.\n" << endl;
        } else if (!(dict.contains(word1) && dict.contains(word2))) {
            cout << "The two words must be found in the dictionary.\n" << endl;
        } else if (word1 == word2) {
            cout << "The two words must be different.\n" << endl;
        } else {
            return;
        }
    }
}

int main() {
    cout << "Welcome to CS 106B Word Ladder.\n"
            "Please give me two English words, and I will change the\n"
            "first into the second by changing one letter at a time.\n" << endl;

    ifstream file;
    string word1;
    string word2;

    promptUserForFile(file, "Dictionary file name:", "Unable to open that file.  Try again.");
    Lexicon dictionary = file;

    while (true) {
        getUserInput(word1, word2, dictionary);

        if (word1.length() == 0 || word2.length() == 0) {
            break;
        }

        Queue<Stack<string>> ladders;
        Set<string> usedWords;
        ladders.enqueue({word1});
        bool isWordFound = false;

        while (!ladders.isEmpty() && !isWordFound) {
            Stack<string> s1 = ladders.dequeue();
            string word = s1.peek();
            string originalWord = word;

            for (int i = 0; i < word.length(); i++) {
                for (char c = 'a'; c <= 'z'; c++ ) {
                    word[i] = c;
                    if (dictionary.contains(word) && word != originalWord && !usedWords.contains(word)) {
                        Stack<string> s2;
                        s2 = s1;
                        s2.push(word);
                        ladders.enqueue(s2);
                        usedWords.add(word);

                        if (word == word2) {
                            isWordFound = true;
                            cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
                            printResults(s2);
                        }
                    }
                }
                word[i] = originalWord[i];
            }
        }

        if (ladders.isEmpty()) {
            cout << "No word ladder found from " << word2 << " back to " << word1 << ":" << endl;
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}

string promptForWord(int order) {
    string word;
    cout << "Word " << integerToString(order) << "(or Enter to quit): ";
    getline(cin, word);
    return toLowerCase(word);
}

void printResults(Stack<string> s) {
    while (!s.isEmpty()) {
        cout << s.pop() << " ";
    }
    cout << endl;
    cout << endl;
}
