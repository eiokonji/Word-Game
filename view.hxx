#pragma once

#include "model.hxx"
#include <vector>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const& model);

    /// Called by the framework to find out what to draw on the screen.
    void draw(ge211::Sprite_set& set);

    /// Called by the framework to find out how big to make the window.
    Dimensions initial_window_dimensions() const;


private:
    ///
    /// Data members (fields)
    ///

    // Model - stores the state of the game
    Model const& model_;

    // Sprites - images that are drawn on the screen
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Font sans50{"sans.ttf", 50};
    ge211::Text_sprite letter_sprites[5][6];
    ge211::Text_sprite score;
    ge211::Text_sprite ans;
    ge211::Text_sprite error_message;
    ge211::Text_sprite win_message;
    ge211::Text_sprite lose_message;

    ge211::Rectangle_sprite const white_square;
    ge211::Rectangle_sprite const green_square;
    ge211::Rectangle_sprite const yellow_square;
    ge211::Rectangle_sprite const gray_square;

    ///
    /// PRIVATE HELPER FUNCTIONS
    ///

    // Adds a square tile of the given color to the given set at the given
    // position.
    void add_color_sprite_(ge211::Sprite_set& set, char eval, Position pos)
    const;
};
