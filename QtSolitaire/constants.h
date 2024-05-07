#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int GAME_WIDTH {800};
static const int GAME_HEIGHT {640};

static const int TOP_MARGIN {10};
static const int CARD_SPACING{20};

// Playing card aspect ratio is 3.5"/2.5" => 1.4 => Approx 14/10
static const int CARD_WIDTH {60};
static const int CARD_HEIGHT {CARD_WIDTH*14/10};    ///< Height of a card = 84
static const int SHDW {3};                          ///< Drop shadow offset
static const int CARD_OVERLAP {CARD_HEIGHT/6};      ///< overlap for Descending Stacks = 14

static const int NUM_PLAY_STACKS{7};

static const double SVG_SCALEF{0.078};            // SVG Scale Factor
static const double CARD_RADIUS{CARD_WIDTH/10.0};

static const char CARD_MIME_TYPE[] {"application/x-card"};

enum class DEBUG_LEVEL {
    NONE,
    NORMAL,
    VERBOSE
};
static const DEBUG_LEVEL debugLevel{DEBUG_LEVEL::VERBOSE};

#endif // CONSTANTS_H
