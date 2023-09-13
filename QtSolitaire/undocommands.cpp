#include "undocommands.h"
#include "cardstack.h"

/******************************************************************************
 * Hand Click Undo Command Implementation
 *****************************************************************************/
HandToWasteCommand::HandToWasteCommand(RandomStack *hand, RandomStack *wastePile)
    : mHand{hand}
    , mWastePile{wastePile}
{
}


/**
 * @brief undo move single card from hand to waste pile
 */
void HandToWasteCommand::undo() {

    Card *topCard{nullptr};
    if (!mHand || !mWastePile) {
        qDebug() << "Bad parameter in HandClickUndoCommand";
        return;
    }
    if (mWastePile->isEmpty()) {
        return;
    }
    topCard = mWastePile->takeTop();
    qDebug() << "Undo" << text();
    topCard->setFaceUp(false);
    mHand->addCard(topCard);
}

/**
 * @brief redo move single card from hand to waste pile
 */
void HandToWasteCommand::redo() {

    Card *topCard{nullptr};
    if (!mHand || !mWastePile) {
        qDebug() << "Bad parameter in HandClickUndoCommand";
        return;
    }
    if (mHand->isEmpty()) {
        return;
    }
    topCard = mHand->takeTop();
    if (this->text().isEmpty()) {
        QString text = "Moved " + topCard->getText() + "to Waste Pile";
        this->setText(text);
    }

    qDebug() << "Redo " << text();

    topCard->setFaceUp(true);
    mWastePile->addCard(topCard);    
}

/******************************************************************************
 * Reset Hand Command Implementation - Move Waste Pile back to Hand
 *****************************************************************************/
ResetHandCommand::ResetHandCommand(RandomStack *hand, RandomStack *wastePile)
    : mHand{hand}
    , mWastePile{wastePile}
{
    this->setText("Reset hand from Waste pile");
}

/**
 * @brief undo move entire waste pile back to the hand
 */
void ResetHandCommand::undo() {

    Card *card{nullptr};
    if (!mHand || !mWastePile) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mHand->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    while (!mHand->isEmpty()) {
        card = mHand->takeTop();
        card->setFaceUp(true);             // Ensure new top of stack face down
        mWastePile->addCard(card);
    }
}

/**
 * @brief redo move entire waste pile back to the hand
 */
void ResetHandCommand::redo() {

    Card *card{nullptr};

    if (!mHand || !mWastePile) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    qDebug() << "Redo " << text();
    while (!mWastePile->isEmpty()) {
        card = mWastePile->takeTop();
        card->setFaceUp(false);             // Ensure new top of stack face down
        mHand->addCard(card);
    }
}


/******************************************************************************
 * Move to Playfield Undo Command Implementation
 *****************************************************************************/
MoveToPlayfieldCommand::MoveToPlayfieldCommand(RandomStack *wastePile, DescendingStack *dStack)
    : mWastePile{wastePile}
    , mDStack{dStack}
{
}


/**
 * @brief undo moves entire waste pile back to the hand
 */
void MoveToPlayfieldCommand::undo() {

    Card *card{nullptr};
    if (!mWastePile || !mDStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mDStack->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    card = mDStack->takeTop();
    mWastePile->addCard(card);
}

/**
 * @brief redo moves entire waste pile back to the hand
 */
void MoveToPlayfieldCommand::redo() {

    Card *card{nullptr};

    if (!mWastePile || !mDStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mWastePile->takeTop();
    if (card && text().isEmpty()) {
        QString text = "Move " + card->getText() + "to playfield";
        setText(text);
    }
    qDebug() << "Redo " << text();

    mDStack->addCard(card);
}

/******************************************************************************
 * Waste Pile to Foundation Command Implementation
 *****************************************************************************/
WasteToFoundationCommand::WasteToFoundationCommand(RandomStack *wastePile, SortedStack *sStack)
    : mWastePile{wastePile}
    , mSStack{sStack}
{
}

/**
 * @brief undo move single card from waste pile to foundation
 */
void WasteToFoundationCommand::undo() {

    Card *card{nullptr};
    if (!mWastePile || !mSStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mSStack->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    card = mSStack->takeTop();
    mWastePile->addCard(card);
}

/**
 * @brief redo move single card from waste pile to foundation
 */
void WasteToFoundationCommand::redo() {

    Card *card{nullptr};

    if (!mWastePile || !mSStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mWastePile->takeTop();
    if (card && text().isEmpty()) {
        QString text = "Move" + card->getText() + "to foundation";
        setText(text);
    }
    qDebug() << "Redo " << text();
    mSStack->addCard(card);
}

/******************************************************************************
 * Playfield to Foundation Command Implementation
 *****************************************************************************/
PlayfieldToFoundationCommand::PlayfieldToFoundationCommand(DescendingStack *playfield, SortedStack *sStack)
    : mPlayfieldStack{playfield}
    , mSStack{sStack}
{
}

/**
 * @brief undo move single card from waste pile to foundation
 */
void PlayfieldToFoundationCommand::undo() {

    Card *card{nullptr};
    if (!mPlayfieldStack || !mSStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mSStack->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    card = mSStack->takeTop();
    mPlayfieldStack->addCard(card);
}

/**
 * @brief redo move single card from waste pile to foundation
 */
void PlayfieldToFoundationCommand::redo() {

    Card *card{nullptr};

    if (!mPlayfieldStack || !mSStack) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mPlayfieldStack->takeTop();
    if (card && text().isEmpty()) {
        QString text = "Move" + card->getText() + "to foundation";
        setText(text);
    }
    qDebug() << "Redo " << text();

    mSStack->addCard(card);
}

/******************************************************************************
 * Playfield to Playfield Undo Command Implementation
 *****************************************************************************/
PlayfieldToPlayfieldCommand::PlayfieldToPlayfieldCommand(DescendingStack *playfieldFrom, DescendingStack *playfieldTo)
    : mPlayfieldFrom{playfieldFrom}
    , mPlayfieldTo{playfieldTo}
{
}

/**
 * @brief Undo click to move single card from one playfield column to another
 */
void PlayfieldToPlayfieldCommand::undo() {

    Card *card{nullptr};
    if (!mPlayfieldFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mPlayfieldTo->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    card = mPlayfieldTo->takeTop();
    mPlayfieldFrom->addCard(card);
}

/**
 * @brief Redo click to move single card from one playfield column to another
 */
void PlayfieldToPlayfieldCommand::redo() {

    Card *card{nullptr};

    if (!mPlayfieldFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mPlayfieldFrom->takeTop();
    if (card && text().isEmpty()) {
        QString text = "Move" + card->getText() + "to foundation";
        setText(text);
    }
    qDebug() << "Redo " << text();
    mPlayfieldTo->addCard(card);
}
