#include "undocommands.h"
#include "cardstack.h"

/******************************************************************************
 * Hand Click Undo Command Implementation
 *****************************************************************************/
HandClickCommand::HandClickCommand(RandomStack *hand, RandomStack *wastePile)
    : mHand{hand}
    , mWastePile{wastePile}
{
}


void HandClickCommand::undo() {

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

void HandClickCommand::redo() {

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
 * Hand Dbl Click Undo Command Implementation
 *****************************************************************************/
EmptyHandClickCommand::EmptyHandClickCommand(RandomStack *hand, RandomStack *wastePile)
    : mHand{hand}
    , mWastePile{wastePile}
{
    this->setText("Reset hand from Waste pile");
}


void EmptyHandClickCommand::undo() {

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
 * @brief EmptyHandClickCommand::redo - moves entire waste pile back to the hand
 */
void EmptyHandClickCommand::redo() {

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


