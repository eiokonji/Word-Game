#include "model.hxx"

using namespace ge211;

Model::Model()
        : round_(1),
          is_game_over_(false),
          did_win_(false),
          error_(false),
          guess_("     "),
          answer_("     ")
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            board_[i][j] = ' ';
            eval_board_[i][j] = ' ';
        }
    }

    std::vector<std::string> answers = load_dictionary("answers_upper.dat");
    Random_source<int> range(0, 49);
    int choice = range.next();
    std::string ans = answers[choice];
    for (size_t i = 0; i < 5; i++) {
        answer_[i] = ans[i];
    }
}

std::vector<std::string>
Model::load_dictionary(std::string const& filename) const
{
    std::ifstream dictionary = ge211::open_resource_file(filename);
    std::vector<std::string> result;
    std::string buffer;

    while (std::getline(dictionary, buffer)) {
        result.push_back(buffer);
    }

    if (dictionary.bad()) {
        throw std::runtime_error("could not read dictionary: " + filename);
    }

    return result;
}

bool
Model::is_guess_valid_(std::string guess) const
{
    // is the answer 5 letters long?
    if (guess[4] != ' ') {
        for (std::string word : load_dictionary("english_words_upper.dat")) {
            // is it an english word?
            std::string w = "     ";
            for (size_t i = 0; i < 5; i++) {
                w[i] = word[i];
            }
            if (guess == w) {
                return true;
            }
        }

        for (std::string word : load_dictionary("answers_upper.dat")) {
            std::string w = "     ";
            for (size_t i = 0; i < 5; i++) {
                w[i] = word[i];
            }
            // is it an answer?
            if (guess == w) {
                return true;
            }
        }
        return false;
    }
    else {
        return false;
    }
}

std::string
Model::evaluate_guess_(std::string guess, std::string answer) const
{
    std::string evaluation = "00000";
    for (size_t i = 0; i < 5; i++) { // iterate through guess
        if (guess[i] == answer[i]) { // check if green
            evaluation[i] = 'g';
            answer[i] = '0'; // ensures the same letter from the answer isn't
                             // double counted
        }
    }
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) { // iterate through answer
            if (guess[i] == answer[j] && evaluation[i] != 'g') { // check if
                // yellow
                evaluation[i] = 'y';
                answer[j] = '0'; // ensures the same letter from the answer
                                 // isn't double counted
            }
        }
        if (evaluation[i] != 'g' && evaluation[i] != 'y') { // else, it is gray
            evaluation[i] = 'x';
        }
    }
    return evaluation;
}

void
Model::set_game_over_()
{
    if (guess_ == "ggggg") { // fully correct guess - you won
        did_win_ = true;
    }
    if (round_ == 6 || guess_ == "ggggg") { // lose/win - game ends either way
        is_game_over_ = true;
    }
}

void Model::play_move(int key)
{
    error_ = false; // no error message
    if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) // input letter
    {   bool check = true;
        for (int i = 0; i < 5; i++) {
            if (tiles(i, round_ - 1) == ' ' && check) { // find 1st empty slot
                board_[i][round_ - 1] = toupper(key);
                guess_[i] = toupper(key);
                check = false;
            }
        }
    }
    else if (key == 127 || key == 8) { // delete or backspace input
        bool check = true;
        for (int i = 4; i >= 0; i--) {
            if (tiles(i, round_ - 1) != ' ' && check) { // find last full slot
                board_[i][round_ - 1] = ' ';
                guess_[i] = ' ';
                check = false;
            }
        }
    }
    else if (key == 13) { // return
        if (is_guess_valid_(guess_)) {
            guess_ = evaluate_guess_(guess_, answer_);
            for (size_t i = 0; i < 5; i++) {
                eval_board_[i][round_-1] = guess_[i];
            }
            set_game_over_(); // changes game status if necessary
            round_++;
        }
        else { // not a valid word, so we need to display an error message
            error_ = true;
        }
    }
}