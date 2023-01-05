#include "model.hxx"
#include <catch.hxx>

struct Test_access
{
    Model model;

    explicit Test_access(Model&);

    bool is_guess_valid(Model mod, std::string guess) {
        return mod.is_guess_valid_(guess);
    }

    std::string evaluate_guess(Model mod, std::string guess, std::string
    answer) {
        return mod.evaluate_guess_(guess, answer);
    }
};

Test_access::Test_access(Model& model)
        : model(model)
{}

TEST_CASE("Guesses are all valid words")
/// 1) This test_case tests the helper function is_guess_valid_. It makes sure
/// that an attempted guess is either a real 5-letter word, or one of the
/// approved answers.
{
    Model model;
    Test_access access(model);

    // english word that isn't an answer (valid)
    CHECK(access.is_guess_valid(model, "STARE"));

    // potential answer that isn't an english word (valid)
    CHECK(access.is_guess_valid(model, "SRUTI"));

    // potential answer that is also an english word (valid)
    CHECK(access.is_guess_valid(model, "WHILE"));

    // actual answer (valid)
    CHECK(access.is_guess_valid(model, model.get_answer()));

    // non answer, non english word (not valid)
    CHECK(!access.is_guess_valid(model, "ABCDE"));

    // short word (not valid)
    CHECK(!access.is_guess_valid(model, "PLAY "));

    // empty word (not valid)
    CHECK(!access.is_guess_valid(model, "     "));
}

TEST_CASE("Guesses are evaluated correctly")
/// 2) This test_case tests the helper function evaluate_guess_. It makes sure
/// that a given guess is compared correctly with the answer so that the board
/// tiles turn into the proper colors.
{
    Model model;
    Test_access access(model);

    // standard guess
    CHECK(access.evaluate_guess(model, "pinch", "print") == "gyyxx");

    // completely correct guess
    CHECK(access.evaluate_guess(model, "print", "print") == "ggggg");

    // completely incorrect guess
    CHECK(access.evaluate_guess(model, "allow", "print") == "xxxxx");

    // double letter ('i') with two yellows
    CHECK(access.evaluate_guess(model, "civil", "while") == "xyxxy");

    // double letter ('e') with one green and one gray
    CHECK(access.evaluate_guess(model, "exile", "while") == "xxggg");
}

TEST_CASE("Board changes appropriately after guessing")
/// 3) This test_case ensures that the evals() function updates properly after
/// making a guess. As in, each guessed letter's tile will turn to the
/// correct color
{
    Model model;
    model.set_answer("PRINT");
    model.set_guess("PINCH"); // correct response: gyyxx
    model.play_move(13);
    CHECK(model.evals(0, 0) == 'g');
    CHECK(model.evals(1, 0) == 'y');
    CHECK(model.evals(2, 0) == 'y');
    CHECK(model.evals(3, 0) == 'x');
    CHECK(model.evals(4, 0) == 'x');

    // second guess (next row)
    model.set_guess("PRINT"); // correct response: ggggg
    model.play_move(13);
    CHECK(model.evals(0, 1) == 'g');
    CHECK(model.evals(1, 1) == 'g');
    CHECK(model.evals(2, 1) == 'g');
    CHECK(model.evals(3, 1) == 'g');
    CHECK(model.evals(4, 1) == 'g');
}

TEST_CASE("Keys have correct functionality")
/// 4) This test_case tests that the inputted key correctly update the guess in
/// play_move. This includes ensuring that both uppercase and lowercase letter
/// inputs are allowed, and that the two delete keys remove letters from the
/// guess.
{
    Model model;

    // standard lowercase guess
    model.play_move(115); // s
    model.play_move(116); // t
    model.play_move(97); // a
    model.play_move(114); // r
    model.play_move(101); // e
    CHECK(model.get_guess() == "STARE");
    model.play_move(13); // return (submits guess)

    // standard uppercase case
    model.play_move(83); // S
    model.play_move(84); // T
    model.play_move(65); // A
    model.play_move(82); // R
    model.play_move(69); // E
    CHECK(model.get_guess() == "STARE");
    model.play_move(13); // return

    // mixed uppercase and lowercase
    model.play_move(83); // S
    model.play_move(116); // t
    model.play_move(65); // A
    model.play_move(114); // r
    model.play_move(69); // E
    CHECK(model.get_guess() == "STARE");
    model.play_move(13); // return

    // backspace/delete keys updates guess correctly
    model.play_move(83); // S
    model.play_move(84); // T
    model.play_move(65); // A
    model.play_move(82); // R
    model.play_move(84); // T
    CHECK(model.get_guess() == "START");
    model.play_move(8); // backspace
    CHECK(model.get_guess() == "STAR ");
    model.play_move(83); // S
    CHECK(model.get_guess() == "STARS");
    model.play_move(127); // delete
    CHECK(model.get_guess() == "STAR ");
    model.play_move(101); // E
    CHECK(model.get_guess() == "STARE");
    model.play_move(13); // return

    // guesses do not update if they're already five letters long
    model.play_move(115); // s
    model.play_move(116); // t
    model.play_move(97); // a
    model.play_move(114); // r
    model.play_move(101); // e
    CHECK(model.get_guess() == "STARE");
    model.play_move(100); // d
    CHECK(model.get_guess() == "STARE");
    model.play_move(13); // return

    // keys don't work if the game is over
    model.set_guess(model.get_answer());
    model.play_move(13); // return
    model.play_move(115); // s
    model.play_move(116); // t
    model.play_move(97); // a
    model.play_move(114); // r
    model.play_move(101); // e
    CHECK(model.get_guess() != "STARE"); // guess should NOT update
}

TEST_CASE("Game ends correctly")
/// 5) This test_case ensures that the play_move function correctly sets the
/// status of the game after submitting a guess, and if applicable, whether the
/// player has won or lost.
{
    // incorrect guess, but you're not at round 6 yet = game is still active
    Model model;
    model.set_guess("STARE");
    model.play_move(13);
    CHECK(model.get_game_status() == 'a');

    // correct guess = you win
    Model model2;
    model2.set_guess(model2.get_answer());
    model2.play_move(13);
    CHECK(model2.get_game_status() == 'w');

    // incorrect guess and out of turns = you lose
    Model model3;
    model3.set_round(6);
    model3.set_guess("STARE");
    model3.play_move(13);
    CHECK(model3.get_game_status() == 'l');

    // final turn, but your guess isn't valid = game is still active
    Model model4;
    model4.set_round(6);
    model4.set_guess("ABCDE");
    model4.play_move(13);
    CHECK(model4.get_game_status() == 'a');

}