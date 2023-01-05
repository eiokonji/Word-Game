#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>
#include <string>
#include <vector>

class Controller : public ge211::Abstract_game
{
public:
    Controller();

protected:
    /// Input handlers
    // Called by the framework when user inputs a key.
    void on_key(ge211::Key) override;

    // These delegate to the view:
    View::Dimensions initial_window_dimensions() const override;
    void draw(ge211::Sprite_set& set) override;


private:
    Model model_;
    View view_;
};
