#pragma once

#include <ge211.hxx>
#include <vector>
#include <string>

class Model
{
public:
    Model();

    /// Returns letter stored in the given board slot
    char tiles(int i, int j) const {
        return board_[i][j];
    }

    /// Returns evaluated color of the letter in the given board slot (g/y/x)
    char evals(int i, int j) const {
        return eval_board_[i][j];
    }

    /// Returns the current guess for this round
    std::string get_guess() const {
        return guess_;
    }

    /// Returns the answer for the game
    std::string get_answer() const {
        return answer_;
    }

    /// Returns the current round
    int get_round() const{
        return round_;
    }

    /// Checks if there's an error. It is initialized as false and only
    /// returns true if the submitted guess is not valid.
    /// Works as a public version of "is_guess_valid()" below.
    bool get_error() const {
        return error_;
    }

    /// Returns the current status of the game. If the game is not over, it
    /// is active ('a'). If it is over because the user has won, the status is
    /// 'w'. If it is over because the user has lost, the status is 'l'.
    char get_game_status() const {
        if (!is_game_over_) {
            return 'a'; // active still
        }
        else if (did_win_) {
            return 'w'; // won
        }
        else {
            return 'l'; // lost
        }
    }

    /// Updates the guess_, board_ positions, error_ and round_ based on the
    /// typed key.
    ///
    /// If the typed key is a letter:
    /// It saves that letter to the current board position and updates the
    /// "guess_" string.
    ///
    /// If the typed key is the delete or backspace key:
    /// It replaces the most recently filled board position with a space (i.e.
    /// it deletes the last typed key).
    ///
    /// If the typed key is a return key:
    /// It checks if the guess is valid by calling "is_guess_valid()". If the
    /// guess is valid:
    ///     It calls "evaluate_guess_()" to check which characters in the
    ///     guess are correctly placed and in the answer, incorrectly placed
    ///     but in the answer or not in the answer.
    ///     Then it updates the game status (if necessary) by calling
    ///     "set_game_over_()".
    ///     Then it updates the round.
    /// If the guess is invalid:
    ///     It updates the "error_" variable to equal true.
    void play_move(int key);

    friend struct Test_access;

    ///
    /// HELPER FUNCTIONS FOR TESTING
    ///

    /// Sets the current guess
    void set_guess(std::string new_guess) {
        guess_ = new_guess;
    }

    /// Sets the answer for this game
    void set_answer(std::string answer) {
        answer_ = answer;
    }

    /// Sets the current round
    void set_round(int num) {
        round_ = num;
    }

    ///
    /// END OF HELPER FUNCTIONS FOR TESTING
    ///

private:
    char board_[5][6];
    char eval_board_[5][6];
    int round_;
    bool is_game_over_;
    bool did_win_;
    bool error_;
    std::string guess_;
    std::string answer_;


    ///
    /// PRIVATE HELPER FUNCTIONS
    ///


    /// Takes in a guess and returns a bool to determine whether it is valid. A
    /// valid word must be five letters long and must exist in either the
    /// list of english words or the answer list.
    ///
    /// Returns "true" if the guess is valid, and "false" otherwise.
    bool is_guess_valid_(std::string guess) const;


    /// Takes in a user guess and the answer to the puzzle. It compares each
    /// letter from the guess with the actual answer to determine whether that
    /// letter's tile should turn green, yellow, or gray. It returns a string
    /// of symbols: g, y, and x, to represent these colors respectively. Green
    /// (g) means the letter is in the correct spot, yellow (y) means the
    /// letter is in the answer, but is in the wrong spot, and gray (x) means
    /// the letter is not in the answer at all. So, if the answer is "print"
    /// and the user guesses "pinch", it should return "gyyxx".
    std::string evaluate_guess_(std::string guess, std::string answer) const;


    /// Checks if the game is over, and if so, ends it by setting
    /// is_game_over to be true. It also sets the variable did_win_ as either
    /// true or false depending if the player won or lost.
    void set_game_over_();


    /// Loads a text file as a vector of strings
    std::vector<std::string>
    load_dictionary(std::string const& filename) const;
};
