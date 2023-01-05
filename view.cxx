#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// Initializing the grid and box sizes (data)
static int const grid_size = 75;
static int const box_size = 9 * grid_size / 10;

View::View(Model const& model)
        : model_(model),
          white_square({box_size, box_size}, Color::white()),
          green_square({box_size, box_size}, Color::medium_green()),
          yellow_square({box_size, box_size}, Color::medium_yellow()),
          gray_square({box_size, box_size}, {128, 128, 128})

{ }

void
View::draw(ge211::Sprite_set& set)
{
    ge211::Text_sprite::Builder message_builder(sans30);

    /// Displays all tiles and letters.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            char letter = model_.tiles(i, j);
            char eval = model_.evals(i, j);
            Position posn = Position(i*grid_size+4,j*grid_size+3);
            add_color_sprite_(set, eval, posn);
            ge211::Text_sprite::Builder letter_builder(sans50);
            letter_builder.color(Color::black()) << letter;
            letter_sprites[i][j].reconfigure(letter_builder);

            set.add_sprite(white_square, posn, 1);

            // Centers the letter 'I' since it is less wide.
            if (letter == 'I') {
                set.add_sprite(letter_sprites[i][j],
                               Position(i * grid_size + 30, j * grid_size - 5),
                               3);
            }
            // Centers the letters 'M' and 'W' since they are too wide.
            else if (letter == 'M' || letter == 'W') {
                set.add_sprite(letter_sprites[i][j],
                               Position(i*grid_size+15,j*grid_size-5),3);
            }
            // Ensures regular positioning for all other letters.
            else {
                    set.add_sprite(letter_sprites[i][j],
                                   Position(i*grid_size+20,j*grid_size-5),3);
            }
        }
    }

    /// Checks to see if error_ == true and if so, adds an error message.
    if (model_.get_error()) {
        message_builder.color(Color::white()) << "Not a valid guess";
        error_message.reconfigure(message_builder);
        set.add_sprite(error_message, Position(70,460), 1);
    }


    /// Checks the game status and prints a corresponding message.
    // Adds a winning message & score if the player wins.
    if (model_.get_game_status() == 'w') {
        message_builder.color(Color::white()) << "You win! Score: ";
        win_message.reconfigure(message_builder);
        set.add_sprite(win_message, Position(70,460), 1);
        message_builder.color(Color::white()) << model_.get_round()-1; // score
        score.reconfigure(message_builder);
        set.add_sprite(score, Position(70,460), 1);
    }

    // Adds a losing message & answer if the player loses
    if (model_.get_game_status() == 'l') {
        message_builder.color(Color::white()) << "You lose! Answer: ";
        lose_message.reconfigure(message_builder);
        set.add_sprite(lose_message, Position(10,460), 1);
        message_builder.color(Color::white()) << model_.get_answer(); // answer
        ans.reconfigure(message_builder);
        set.add_sprite(ans, Position(10,460), 1);
    }

    /// Given an evaluated guess, it checks each letter in it and changes the
    /// color of the tile as appropriate.
    std::string guess = model_.get_guess();
    for (size_t i = 0; i < 5; i++) {
        Position posn = Position(i*grid_size+4, (model_.get_round()-2)
                                                    *grid_size+3);
        if (guess[i] == 'g') { // green
            set.add_sprite(green_square, posn,2);
        }
        if (guess[i] == 'y') { // yellow
            set.add_sprite(yellow_square, Position
            (i*grid_size+4,(model_.get_round()-2)*grid_size+3), 2);
        }
        if (guess[i] == 'x') { // gray
            set.add_sprite(gray_square, Position
            (i*grid_size+4,(model_.get_round()-2)*grid_size+3), 2);
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return grid_size * Dimensions{5, 7};
}

void
View::add_color_sprite_(Sprite_set& set, char eval, Position pos) const
{
    if (eval == 'g') {
        set.add_sprite(green_square, pos,2);
    }
    else if (eval == 'y') {
        set.add_sprite(yellow_square, pos,2);
    }
    else if (eval == 'x') {
        set.add_sprite(gray_square, pos,2);
    }
}