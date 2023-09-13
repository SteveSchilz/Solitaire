#ifndef CARDUNDOCOMMAND_H
#define CARDUNDOCOMMAND_H

#include <QUndoCommand>

class Card;
class RandomStack;
class DescendingStack;
class SortedStack;

QT_FORWARD_DECLARE_CLASS(QGraphicsItem);
/**
 * @brief The HandClickUndoCommand - Undo command for clicking on the hand pile
 */
class HandToWasteCommand : public QUndoCommand
{
public:
    HandToWasteCommand(RandomStack *hand, RandomStack *wastePile);

    void undo() override;
    void redo() override;

private:
    RandomStack *mHand;
    RandomStack *mWastePile;
};

class ResetHandCommand : public QUndoCommand
{
public:
    ResetHandCommand(RandomStack *hand, RandomStack *wastePile);

    void undo() override;
    void redo() override;

private:
    RandomStack *mHand;
    RandomStack *mWastePile;
};


class MoveToPlayfieldCommand : public QUndoCommand
{
public:
    MoveToPlayfieldCommand(RandomStack *wastePile, DescendingStack *dStack);

    void undo() override;
    void redo() override;

private:
    RandomStack *mWastePile;
    DescendingStack *mDStack;
};


class WasteToFoundationCommand : public QUndoCommand
{
public:
    WasteToFoundationCommand(RandomStack *wastePile, SortedStack *sStack);

    void undo() override;
    void redo() override;

private:
    RandomStack *mWastePile;
    SortedStack *mSStack;
};

class PlayfieldToFoundationCommand : public QUndoCommand
{
public:
    PlayfieldToFoundationCommand(DescendingStack *playfield, SortedStack *sStack);

    void undo() override;
    void redo() override;

private:
    DescendingStack *mPlayfieldStack;
    SortedStack *mSStack;
};


class PlayfieldToPlayfieldCommand : public QUndoCommand
{
public:
    PlayfieldToPlayfieldCommand(DescendingStack *playfieldFrom, DescendingStack *playfieldTo);

    void undo() override;
    void redo() override;

private:
    DescendingStack *mPlayfieldFrom;
    DescendingStack *mPlayfieldTo;
};


#endif // CARDUNDOCOMMAND_H
