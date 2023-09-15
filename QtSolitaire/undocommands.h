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
    bool mTopFlipped;
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
    bool mTopFlipped;
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
    bool mTopFlipped;
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
    bool mTopFlipped;
    DescendingStack *mPlayfieldFrom;
    DescendingStack *mPlayfieldTo;
};

class DragPlayfieldToPlayfieldCommand : public QUndoCommand
{
public:
    DragPlayfieldToPlayfieldCommand(Card *droppedCard, DescendingStack *playfieldFrom, DescendingStack *playfieldTo);

    void undo() override;
    void redo() override;

private:
    bool mTopFlipped;
    Card *mDroppedCard;
    DescendingStack *mPlayfieldFrom;
    DescendingStack *mPlayfieldTo;
};

class DragWasteToPlayfieldCommand : public QUndoCommand
{
public:
    DragWasteToPlayfieldCommand(RandomStack *mWasteFrom, DescendingStack *playfieldTo);

    void undo() override;
    void redo() override;

private:
    RandomStack *mWasteFrom;
    DescendingStack *mPlayfieldTo;
};


class DragFoundationToPlayfieldCommand : public QUndoCommand
{
public:
    DragFoundationToPlayfieldCommand(SortedStack *foundationFrom,DescendingStack *playfieldTo);

    void undo() override;
    void redo() override;

private:
    bool mTopFlipped;
    DescendingStack *mPlayfieldTo;
    SortedStack *mFoundationFrom;
};

#if 0
class DragPlayfieldToFoundation : public QUndoCommand
{
public:
    DragPlayfieldToFoundation(DescendingStack *playfieldFrom, SortedStack *foundationTo);

    void undo() override;
    void redo() override;

private:
    bool mTopFlipped;
    DescendingStack *mPlayfieldFrom;
    SortedStack *mFoundationTo;
};
#endif //0

#endif // CARDUNDOCOMMAND_H
