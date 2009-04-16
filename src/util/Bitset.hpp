//----------------------------------------------------------------------------
/** @file Bitset.hpp
 */
//----------------------------------------------------------------------------

#ifndef BITSET_HPP
#define BITSET_HPP

#include <cassert>
#include <bitset>
#include <set>
#include <string>
#include <vector>

#include "Types.hpp"

//----------------------------------------------------------------------------

/** Maximum size of a bitset. 
    Very important. Only mess with this if you know what you are
    doing!
*/
#if defined(SUPPORT_19x19)

/** Actually need only 361+7 for 19x19. */
static const int BITSETSIZE = 384;

#elif defined(SUPPORT_14x14)

/** Actually need only 196+7 for 14x14. */
static const int BITSETSIZE = 224;

#elif defined(SUPPORT_13x13)

/** Actually need only 169+7 for 13x13. */
static const int BITSETSIZE = 192;

#else

/** Fits 11x11 exactly. */
static const int BITSETSIZE = 128;

#endif

//----------------------------------------------------------------------------

/** Makes IsLessThan() and IsSubsetOf() faster. */
#define OPTIMIZED_BITSET 1

//----------------------------------------------------------------------------

/** Standard-sized bitset. */
typedef std::bitset<BITSETSIZE> bitset_t;

/** Global empty bitset. */
static const bitset_t EMPTY_BITSET;

//----------------------------------------------------------------------------

/** Utilities on bitsets. */
namespace BitsetUtil 
{
    /** Converts the bottom numbits of b into a byte stream. */
    void BitsetToBytes(const bitset_t& b, byte* out, int numbits);
    
    /** Converts a byte stream into a bitset. */
    bitset_t BytesToBitset(const byte* bytes, int numbits);

    /** Converts a bitset into a string of hex symbols. */
    std::string BitsetToHex(const bitset_t& b, int numbits);

    /** Converts a string of hex symbols into a bitset. */
    bitset_t HexToBitset(const std::string& str);

    //------------------------------------------------------------------------

    /** Subtracts b2 from b1. */
    bitset_t Subtract(const bitset_t& b1, const bitset_t& b2);

    /** If removeFrom - remove is not empty, stores that value in
        removeFrom and returns true.  Otherwise, removeFrom is not
        changed and returns false. */
    bool SubtractIfLeavesAny(bitset_t& removeFrom, const bitset_t& remove);


    /** Returns true if b1 is a subset of b2. */
    bool IsSubsetOf(const bitset_t& b1, const bitset_t& b2);
    
    /** Returns true if b1 comes before b2 in some consistent order
        (any well defined ordering, not necessarily lexicographic). */
    bool IsLessThan(const bitset_t& b1, const bitset_t& b2);

    //------------------------------------------------------------------------

    /** Stores indices of set bits in b in indices. 
        @note INT must be convertible to an int.
    */
    template<typename INT>
    void BitsetToVector(const bitset_t& b, std::vector<INT>& indices);

    /** Converts of set of indices into a bitset with those bits set. 
        @note INT must be convertible to an int. 
     */
    template<typename INT>
    bitset_t SetToBitset(const std::set<INT>& indices);
    
    //------------------------------------------------------------------------

    /** Returns the bit that is set in b. */
    int FindSetBit(const bitset_t& b);

    /** Returns least-significant set bit in b. */
    int FirstSetBit(const bitset_t& b);
}

//----------------------------------------------------------------------------

template<typename INT>
void BitsetUtil::BitsetToVector(const bitset_t& b, 
                                std::vector<INT>& indices)
{
    indices.clear();
    for (int i=0; i<BITSETSIZE; i++) {
        if (b.test(i))
            indices.push_back(static_cast<INT>(i));
    }
    assert(b.count() == indices.size());
}

template<typename INT>
bitset_t BitsetUtil::SetToBitset(const std::set<INT>& indices)
{
    bitset_t ret;
    typedef typename std::set<INT>::const_iterator const_iterator;
    for (const_iterator it=indices.begin(); 
         it != indices.end();
         ++it) 
    {
        ret.set(static_cast<int>(*it));
    }
    return ret;
}

//----------------------------------------------------------------------------

inline bool BitsetUtil::IsSubsetOf(const bitset_t& b1, const bitset_t& b2)
{
#if OPTIMIZED_BITSET
    const void* vb1 = &b1;
    const void* vb2 = &b2;
    const int* ib1 = (const int*)vb1;
    const int* ib2 = (const int*)vb2;
    static const int num = sizeof(bitset_t)/sizeof(int);
    for (int i=0; i<num; ++i, ++ib1, ++ib2) {
        if (*ib1 & ~(*ib2))
            return false;
    }
    return true;
#else
    return ((b1 & b2) == b1);
#endif
}

inline bool BitsetUtil::IsLessThan(const bitset_t& b1, const bitset_t& b2)
{
#if OPTIMIZED_BITSET
    const void* vb1 = &b1;
    const void* vb2 = &b2;
    const int* ib1 = (const int*)vb1;
    const int* ib2 = (const int*)vb2;
    const static int num = sizeof(bitset_t)/sizeof(int);
    for (int i=0; i<num; ++i, ++ib1, ++ib2) {
        if (*ib1 != *ib2) 
            return (*ib1 < *ib2);
    }
    return false;
#else 
    for (int i=0; i<BITSETSIZE; i++) {
        if (b1[i] != b2[i]) 
            return !b1[i];
    }
    return false;
#endif
}

//----------------------------------------------------------------------------

/** Extends the standard binary '-' operator for bitsets. */
bitset_t operator-(const bitset_t& b1, const bitset_t& b2);

//----------------------------------------------------------------------------

#endif // BITSET_HPP
