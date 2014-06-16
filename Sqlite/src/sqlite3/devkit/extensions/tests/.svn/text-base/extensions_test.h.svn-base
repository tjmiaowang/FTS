#ifndef COLLATIONS_TEST_H
#define COLLATIONS_TEST_H

#include "../utils/nds_bool.h"

// macro to make defining char constants by hex numbers simple and warning-free
#define C(x) static_cast<char>(static_cast<unsigned>(x))

// macro to convert a char into a unichar (this conversion makes sense only for ASCII)
#define U(x) static_cast<unichar>(static_cast<unsigned char>(x))

void markFailure(const char * file, unsigned line, const char * function);

template<typename T, typename U>
void expectEq(T expected, U received, const char* file, unsigned line, const char* function)
{
    if (expected != received)
    {
        markFailure(file, line, function);
    }
}

template<>
inline void expectEq(nds_bool expected, bool received, const char* file, unsigned line, const char* function)
{
    expectEq(expected != NDS_FALSE, received, file, line, function);
}

template<>
inline void expectEq(bool expected, nds_bool received, const char* file, unsigned line, const char* function)
{
    expectEq(expected, received != NDS_FALSE, file, line, function);
}

#ifdef _MSC_VER
    // MSVC
    #define EXPECT_EQ(expected, received) expectEq(expected, received, __FILE__, __LINE__, __FUNCTION__)
#else
    // assume C99
    #define EXPECT_EQ(expected, received) expectEq(expected, received, __FILE__, __LINE__, __func__)
#endif

#endif // COLLATIONS_TEST_H
