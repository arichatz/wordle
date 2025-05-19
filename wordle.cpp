#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <set>
#include <algorithm>
#include <iomanip>
#include <windows.h>

using namespace std;

// Struct to keep track of player's game statistics
struct Stats {
    int games_played = 0;
    int games_won = 0;
    int current_streak = 0;
    int max_streak = 0;
};

// Turns input string to all uppercase
string to_uppercase(const string& str) {
    string result = str;
    for (char& c : result) {
        if (c >= 'a' && c <= 'z') {
            c -= 32; // Convert to uppercase
        }
    }
    return result;
}

// Checks if the guess is exactly 5 letters long
bool is_valid_guess(const string& guess) {
    return guess.length() == 5;
}

// Generates color feedback for the guess (0=gray, 1=yellow, 2=green)
vector<int> get_feedback_colors(const string& guess, const string& target) {
    vector<int> colors(5, 0); // Initialize all to gray
    string target_copy = target;

    // First pass: mark correct letters in correct positions (green)
    for (size_t i = 0; i < 5; ++i) {
        if (guess[i] == target[i]) {
            colors[i] = 2;
            target_copy[i] = ' ';
        }
    }

    // Second pass: mark correct letters in wrong positions (yellow)
    for (size_t i = 0; i < 5; ++i) {
        if (colors[i] == 0) {
            if (target_copy.find(guess[i]) != string::npos) {
                colors[i] = 1;
                target_copy[target_copy.find(guess[i])] = ' ';
            }
        }
    }

    return colors;
}

// Prints the word with colored letters for feedback
void print_colored_word(const string& word, const vector<int>& colors, HANDLE hConsole) {
    cout << "   "; // Align left
    for (size_t i = 0; i < 5; ++i) {
        if (colors[i] == 2) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green for correct
        } else if (colors[i] == 1) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Yellow for misplaced
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY); // Gray for wrong
        }
        cout << word[i];
        if (i < 4) cout << " ";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
    cout << endl;
}

// Loads all 5-letter words from file
vector<string> load_words(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;
    while (getline(file, word)) {
        if (word.length() == 5) {
            words.push_back(to_uppercase(word));
        }
    }
    file.close();
    return words;
}

// Saves the word list back to file
void save_words(const string& filename, const vector<string>& words) {
    ofstream file(filename);
    for (const string& word : words) {
        file << word << endl;
    }
    file.close();
}

// Picks a random word from the list
string select_random_word(const vector<string>& words, mt19937& gen) {
    uniform_int_distribution<> dis(0, words.size() - 1);
    return words[dis(gen)];
}

// Menu for adding or removing words
void manage_words(vector<string>& words, HANDLE hConsole) {
    while (true) {
        system("cls");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n   *** Word Management ***\n\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "   1. Add a new word\n";
        cout << "   2. Delete a word\n";
        cout << "   3. Go back\n";
        cout << "\n   Your choice: ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            cout << "\n   Enter a 5-letter word: ";
            string word;
            getline(cin, word);
            word = to_uppercase(word);
            if (word.length() == 5 && find(words.begin(), words.end(), word) == words.end()) {
                words.push_back(word);
                save_words("words.txt", words);
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                cout << "\n   Word added!\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "\n   Invalid or duplicate word!\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            system("pause");
        } else if (choice == "2") {
            cout << "\n   Enter word to delete: ";
            string word;
            getline(cin, word);
            word = to_uppercase(word);
            auto it = find(words.begin(), words.end(), word);
            if (it != words.end()) {
                words.erase(it);
                save_words("words.txt", words);
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                cout << "\n   Word deleted!\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "\n   Word not found!\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            system("pause");
        } else if (choice == "3") {
            break;
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << "\n   Invalid option!\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            system("pause");
        }
    }
}

// Shows game statistics
void show_statistics(const Stats& stats, HANDLE hConsole) {
    system("cls");
    double win_percentage = (stats.games_played > 0) ? (static_cast<double>(stats.games_won) / stats.games_played) * 100 : 0;
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "\n   *** Your Stats ***\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "   Total Games: " << stats.games_played << "\n";
    cout << "   Wins: " << stats.games_won << "\n";
    cout << "   Win Rate: " << fixed << setprecision(1) << win_percentage << "%\n";
    cout << "   Current Streak: " << stats.current_streak << "\n";
    cout << "   Best Streak: " << stats.max_streak << "\n";
    cout << "\n   Press Enter to go back...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Displays the main menu
int show_menu(HANDLE hConsole) {
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "\n   *** WORDLE ***\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "   1. Start Game\n";
    cout << "   2. Manage Words\n";
    cout << "   3. View Stats\n";
    cout << "   4. Quit\n";
    cout << "\n   Choose an option: ";
    string choice;
    getline(cin, choice);
    if (choice == "1") return 1;
    if (choice == "2") return 2;
    if (choice == "3") return 3;
    if (choice == "4") return 4;
    return 0;
}

int main() {
    // Set up console for colored output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Load word list from file
    vector<string> words = load_words("words.txt");
    if (words.empty()) {
        cout << "Error: No words found in words.txt. Please add some words.\n";
        return 1;
    }

    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());

    set<string> used_targets;
    Stats stats;

    while (true) {
        int choice = show_menu(hConsole);
        if (choice == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << "\n   Invalid option!\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            system("pause");
            continue;
        }
        if (choice == 4) break;

        if (choice == 1) {
            system("cls");
            string target;
            do {
                target = select_random_word(words, gen);
            } while (used_targets.find(target) != used_targets.end());
            used_targets.insert(target);

            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "\n   *** WORDLE ***\n\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout << "   Green: Letter is correct and in right spot\n";
            cout << "   Yellow: Letter is in word but wrong spot\n";
            cout << "   Gray: Letter not in word\n";

            int attempts = 6;
            set<string> used_guesses;
            bool won = false;
            vector<pair<string, vector<int>>> history;

            while (attempts > 0 && !won) {
                cout << "\n   Attempt " << (7 - attempts) << "/6. Guess: ";
                string guess;
                getline(cin, guess);
                guess = to_uppercase(guess);

                if (!is_valid_guess(guess)) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    cout << "\n   Guess must be 5 letters!\n";
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    continue;
                }

                if (used_guesses.find(guess) != used_guesses.end()) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    cout << "\n   Already guessed that word!\n";
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    continue;
                }

                used_guesses.insert(guess);
                vector<int> colors = get_feedback_colors(guess, target);
                history.emplace_back(guess, colors);

                cout << "\n";
                for (const auto& entry : history) {
                    print_colored_word(entry.first, entry.second, hConsole);
                }

                if (guess == target) {
                    won = true;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                    cout << "\n   Nice job! The word was: " << target << endl;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }

                attempts--;
            }

            if (!won) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "\n   Out of tries! The word was: " << target << endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            stats.games_played++;
            if (won) {
                stats.games_won++;
                stats.current_streak++;
                stats.max_streak = max(stats.max_streak, stats.current_streak);
            } else {
                stats.current_streak = 0;
            }
            system("pause");
        } else if (choice == 2) {
            manage_words(words, hConsole);
        } else if (choice == 3) {
            show_statistics(stats, hConsole);
        }
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "\n   Thanks for playing Wordle! Come back soon!\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    system("pause");
    return 0;
}
