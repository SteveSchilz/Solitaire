#ifndef ENUMITERATOR_H
#define ENUMITERATOR_H

/**
  * @brief Enum iterator Allows iterating over enums
  *
  *   See https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
  */

#include <type_traits>
template < typename C, C beginVal, C endVal>
class enumIterator {
    typedef typename std::underlying_type<C>::type val_t;
    int val;
public:
    enumIterator(const C & f) : val(static_cast<val_t>(f)) {}
    enumIterator() : val(static_cast<val_t>(beginVal)) {}
    enumIterator operator++() {
        ++val;
        return *this;
    }
    C operator*() { return static_cast<C>(val); }
    enumIterator begin() { return *this; } //default ctor is good
    enumIterator end() {
        static const enumIterator endIter=++enumIterator(endVal); // cache it
        return endIter;
    }
    bool operator!=(const enumIterator& i) { return val != i.val; }
};

#endif // ENUMITERATOR_H
