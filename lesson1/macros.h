// See this: https://gcc.gnu.org/onlinedocs/cpp/index.html#SEC_Content

#ifndef FOO_H
#define FOO_H

// object like macro
#define N 100

// function like macro
#define print() printf("MY PRINT\n");

// multi line macro

// problem with i.e if (0) foo(n) --> will always execute second n++
// #define foo(n) \
// n++; \
// n++

// problem with if () foo(n); else --> compile time error, cannot have ; after if (  ) { } before else
// #define foo(n) \
// { \
//     n++; \
//     n++; \
// } \

// GOOD. Problem with getting value from it
#define foo(n) \
do { \
    n++; \
    n++; \
} while (0)

// ONLY GNU THIS IS NOT IN STD C
// #define foo(n) \
// ({ \
//     n++; \
//     n++; \
//     n; \
// })

// PREDEFINED MACROS
// __FILE__ // char* in .rodata (current file (can have absolute path))
// __LINE__ // int (current LINE)
// __DATE__ // char* in .rodata (date when preprocessor is being run like Dec 6 2022)
// __TIME__ // char* in .rodata (hour when preprocessor is being run like 20:35:02 )
// __cplusplus // C++ (1) C (0)
// __func__ // char* in .rodata function name (since C99)

// GNU PREDEFINED MACROS
// __GNUC__ // Compiler supports gnuc (1), does not support (0)
// __COUNTER__ // This macro expands to sequential integral values starting from 0. (Each file has own value (starting from 0 again))
// __FILE_NAME__ // Like __FILE__ but without absolute path

// STD C
// #error "ERROR"
// compiler extension
// #warning "WARNING"
// compiler extension
// #pragma message ("MESSAGE")

#ifdef SOME_OPTION_ENABLED
    #if SOME_OPTION_ENABLED > 2
        #error "ERROR from ifdef if > 2"
    #endif
#else
    // #error "ERROR from else"
#endif

// is something is not defined, then if assuming value 0
#if defined(SOME_OPTION_ENABLED) && SOME_OPTION_ENABLED > 1
    #error "ERROR from if defined && if > 1"
#else
    // #error "ERROR from else 2"
#endif

#if SOME_OPTION_ENABLED > 1
    #error "ERROR from if > 1"
#else
    // #error "ERROR from else 3"
#endif

#if SOME_OPTION_ENABLED > 1
    #error "ERROR from if > 1"
#elif  SOME_OPTION_ENABLED == 1
    #error "ERROR from elif == 1"
#endif

// to check if compiler supports attributes
#if defined(__has_attribute)
// to check if compiler supports nonnull attribute
    #if __has_attribute(nonnull)
        #define ATTR_NONNULL __attribute__ ((nonnull))
    #endif
#endif

// to check if compiler supports builtins
#if defined(__has_builtin)
// to check if compiler supports popcount builtin
    #if __has_builtin(__builtin_popcount)
        #define popcount(n) __builtin_popcount((unsigned)n)
    #endif
#endif

#define TOKEN_TO_STRING(token)      TOKEN_TO_STRING_PRIV(token)
#define TOKEN_TO_STRING_PRIV(token) #token

#define TOKEN_CONCAT(token1, token2)      TOKEN_CONCAT_PRIV(token1, token2)
#define TOKEN_CONCAT_PRIV(token1, token2) token1 ## token2

// Since C99
#define COMPILER_MESSAGE(str) _Pragma(TOKEN_TO_STRING(message str))

// COMPILER_MESSAGE("MESSAGE FROM MACRO")

// Since C99

// ... cannot be empty
#define myprintf(...) printf(__VA_ARGS__)


#endif
