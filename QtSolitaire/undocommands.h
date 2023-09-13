#ifndef CARDUNDOCOMMAND_H
#define CARDUNDOCOMMAND_H

#include <QUndoCommand>

class Card;
class RandomStack;

QT_FORWARD_DECLARE_CLASS(QGraphicsItem);
/**
 * @brief The HandClickUndoCommand - Undo command for clicking on the hand pile
 */
class HandClickCommand : public QUndoCommand
{
public:
    HandClickCommand(RandomStack *hand, RandomStack *wastePile);

    void undo() override;
    void redo() override;

private:
    RandomStack *mHand;
    RandomStack *mWastePile;
};

class EmptyHandClickCommand : public QUndoCommand
{
public:
    EmptyHandClickCommand(RandomStack *hand, RandomStack *wastePile);

    void undo() override;
    void redo() override;

private:
    RandomStack *mHand;
    RandomStack *mWastePile;
};


#endif // CARDUNDOCOMMAND_H
