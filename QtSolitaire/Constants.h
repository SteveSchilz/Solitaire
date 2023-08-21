#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int GAME_WIDTH {800};
static const int GAME_HEIGHT {640};

static const int TOP_MARGIN {10};
static const int CARD_SPACING{20};

// Playing card aspect ratio is 3.5"/2.5" => 1.4 => Approx 14/10
static const int CARD_WIDTH {60};
static const int CARD_HEIGHT {CARD_WIDTH*14/10};
static const int SHDW {3};                      // Drop shadow offset
static const double SVG_SCALEF{0.075};            // SVG Scale Factor

#endif // CONSTANTS_H
