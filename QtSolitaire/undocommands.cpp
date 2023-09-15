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
    mHand->addCard(topCard, false);
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
        QString text = "Move " + topCard->getText() + " to Waste Pile";
        this->setText(text);
    }

    qDebug() << "Redo " << text();

    topCard->setFaceUp(true);
    mWastePile->addCard(topCard, false);
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
        mWastePile->addCard(card, false);
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
        mHand->addCard(card, false);
    }
}


/******************************************************************************
 * Move to Playfield Undo Command Implementation
 *****************************************************************************/
MoveToPlayfieldCommand::MoveToPlayfieldCommand(RandomStack *wastePile, DescendingStack *dStack)
    : mTopFlipped{false}
    , mWastePile{wastePile}
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
    mWastePile->addCard(card, mWastePile->isTopFlipped());
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
        QString text = "Move " + card->getText() + " to playfield";
        setText(text);
    }
    qDebug() << "Redo " << text();

    mDStack->addCard(card, false);
}

/******************************************************************************
 * Waste Pile to Foundation Command Implementation
 *****************************************************************************/
WasteToFoundationCommand::WasteToFoundationCommand(RandomStack *wastePile, SortedStack *sStack)
    : mTopFlipped{false}
    , mWastePile{wastePile}
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
    mWastePile->addCard(card, mWastePile->isTopFlipped());
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
        QString text = "Move " + card->getText() + " to foundation";
        setText(text);
    }
    qDebug() << "Redo " << text();
    mSStack->addCard(card, false);
}

/******************************************************************************
 * Playfield to Foundation Command Implementation
 *****************************************************************************/
PlayfieldToFoundationCommand::PlayfieldToFoundationCommand(DescendingStack *playfield, SortedStack *sStack)
    : mTopFlipped{false}
    , mPlayfieldStack{playfield}
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
    mPlayfieldStack->addCard(card, mPlayfieldStack->isTopFlipped());
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
        QString text = "Move " + card->getText() + " to foundation";
        setText(text);
    }
    qDebug() << "Redo " << text();

    mSStack->addCard(card, false);
}

/******************************************************************************
 * Playfield to Playfield Undo Command Implementation
 *****************************************************************************/
PlayfieldToPlayfieldCommand::PlayfieldToPlayfieldCommand(DescendingStack *playfieldFrom, DescendingStack *playfieldTo)
    : mTopFlipped{false}
    , mPlayfieldFrom{playfieldFrom}
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

    if (!mPlayfieldFrom->isEmpty() && mTopFlipped) {
    }
    mPlayfieldFrom->addCard(card, mPlayfieldFrom->isTopFlipped());
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
        QString text = "Move " + card->getText() + " to foundation";
        setText(text);
        mTopFlipped = mPlayfieldFrom->isTopFlipped();
    }
    qDebug() << "Redo " << text();
    mPlayfieldTo->addCard(card, false);
}

/******************************************************************************
 * Drag Playfield to Playfield Undo Command Implementation
 *****************************************************************************/
DragPlayfieldToPlayfieldCommand::DragPlayfieldToPlayfieldCommand(Card *droppedCard, DescendingStack *playfieldFrom, DescendingStack *playfieldTo)
    : mTopFlipped{false}
    , mDroppedCard(droppedCard)
    , mPlayfieldFrom{playfieldFrom}
    , mPlayfieldTo{playfieldTo}
{
}

/**
 * @brief Undo Drag move one or more cards from one playfield column to another
 */
void DragPlayfieldToPlayfieldCommand::undo() {

    Card *card{nullptr};
    QList<Card*> tempCards;
    bool firstCard = true;
    bool setTopFlipped = false;

    if (!mDroppedCard || !mPlayfieldFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mPlayfieldTo->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();

    tempCards = mPlayfieldTo->takeCards(*mDroppedCard);
    while (!tempCards.isEmpty()) {
        card = tempCards.takeFirst();
        if (firstCard) {
            setTopFlipped = mPlayfieldFrom->isTopFlipped();
            firstCard = false;
        } else {
            setTopFlipped = false;
        }

        mPlayfieldFrom->addCard(card, setTopFlipped);
        card->update();
    }
}

/**
 * @brief Redo Drag move one or more cards from one playfield column to another
 */
void DragPlayfieldToPlayfieldCommand::redo() {

    Card *card{nullptr};
    QList<Card*> tempCards;
    int numDragged{0};

    if (!mDroppedCard || !mPlayfieldFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    tempCards = mPlayfieldFrom->takeCards(*mDroppedCard);
    while (!tempCards.isEmpty()) {
        card = tempCards.takeFirst();
        mPlayfieldTo->addCard(card, false);
        card->update();
        numDragged++;
    }
    if (text().isEmpty()) {
        QString text = QString("Drag %1 card%2 starting with %3 to other stack")
                           .arg(numDragged)
                           .arg( (numDragged > 1) ? "s" : "")
                           .arg(mDroppedCard->getText());
        setText(text);
        mTopFlipped = mPlayfieldFrom->isTopFlipped();
    }

    qDebug() << "Redo " << text();
}

/******************************************************************************
 * Drag Waste to Playfield Undo Command Implementation
 *
 * NOTE: really does not care about top flipped, because the stacks involved
 *       always have the cards above the movement already face up
 *****************************************************************************/
DragWasteToPlayfieldCommand::DragWasteToPlayfieldCommand(RandomStack *wasteFrom, DescendingStack *playfieldTo)
    : mWasteFrom{wasteFrom}
    , mPlayfieldTo{playfieldTo}
{
}

/**
 * @brief Undo Drag move one or more cards from one playfield column to another
 */
void DragWasteToPlayfieldCommand::undo() {

    Card *card{nullptr};

    if (!mWasteFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mPlayfieldTo->isEmpty()) {
        return;
    }

    qDebug() << "Undo" << text();
    card = mPlayfieldTo->takeTop();
    mWasteFrom->addCard(card, false);
}

/**
 * @brief Redo Drag move one or more cards from one playfield column to another
 */
void DragWasteToPlayfieldCommand::redo() {

    Card *card{nullptr};

    if (!mWasteFrom || !mPlayfieldTo) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mWasteFrom->takeTop();
    if (text().isEmpty()) {
        QString text = QString("Drag  %1 to playfield")
                           .arg(card->getText());
        setText(text);
    }
    mPlayfieldTo->addCard(card, false);

    qDebug() << "Redo " << text();
}

/******************************************************************************
 * Drag Playfield to Foundation Undo Command Implementation
 *****************************************************************************/
DragFoundationToPlayfieldCommand::DragFoundationToPlayfieldCommand(SortedStack *foundationFrom, DescendingStack *playfieldTo )
    : mTopFlipped{false}
    , mFoundationFrom{foundationFrom}
    , mPlayfieldTo{playfieldTo}
{
}

/**
 * @brief Undo click to move single card from one playfield column to another
 */
void DragFoundationToPlayfieldCommand::undo() {

    Card *card{nullptr};

    if (!mPlayfieldTo|| !mFoundationFrom) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }
    if (mPlayfieldTo->isEmpty()) {
        return;
    }
    qDebug() << "Undo" << text();
    card = mPlayfieldTo->takeTop();
    mFoundationFrom->addCard(card, mTopFlipped);
}

/**
 * @brief Redo click to move single card from one playfield column to another
 */
void DragFoundationToPlayfieldCommand::redo() {

    Card *card{nullptr};

    if (!mPlayfieldTo || !mFoundationFrom) {
        qDebug() << "Bad parameter in " << __FUNCTION__;
        return;
    }

    card = mFoundationFrom->takeTop();
    if (card && text().isEmpty()) {
        QString text = "Drag " + card->getText() + " from foundation to playfield";
        setText(text);
        if (!mPlayfieldTo->isEmpty()) {
            mTopFlipped = mPlayfieldTo->isTopFlipped();
        }
    }
    qDebug() << "Redo " << text();
    mPlayfieldTo->addCard(card, false);
}

