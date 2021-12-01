#ifndef INCLUDED_TEST
#define INCLUDED_TEST

#ifdef NDEBUG
    #define assert(a)
#else
    #include <stdio.h>
    #include <stdlib.h>

    inline static int succeeded_message(const char* file, int line, const char* value) {
        printf("%s:%d \033[32mAssertion succeeded: %s\033[m\n" , file, line, value);
        return 1;
    }

    inline static int failed_message(const char* file, int line, const char* value) {
        printf("%s:%d \033[31mAssertion failed: %s\033[m\n" , file, line, value);
        exit(1);
        return 0;
    }

    #define assert(a) a ? succeeded_message(__FILE__, __LINE__, #a) : failed_message(__FILE__, __LINE__, #a)

    #define skip_assert(a)  printf("%s:%d \033[33mAssertion skipped: %s\033[m\n" , __FILE__, __LINE__, #a)

    #define skip_assert_eq(a, b)  printf("%s:%d \033[33mAssertion skipped: %s == %s\033[m\n" , __FILE__, __LINE__, #a, #b)

    #define number_eq(a, b) assert(a == b)

    #define assert_eq(a, b) _Generic((a), \
        signed char:            _Generic((b), signed char:              number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        signed short int:       _Generic((b), signed short int:         number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        signed long int:        _Generic((b), signed long int:          number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        signed long long int:   _Generic((b), signed long long int:     number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        unsigned char:          _Generic((b), unsigned char:            number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        unsigned short int:     _Generic((b), unsigned short int:       number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        unsigned long int:      _Generic((b), unsigned long int:        number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        unsigned long long int: _Generic((b), unsigned long long int:   number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        unsigned int:           _Generic((b), unsigned int:             number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        char:                   _Generic((b), char:                     number_eq(a, b), int: number_eq((int)a, b), default: skip_assert_eq(a, b)), \
        int:                    _Generic((b), int:                      number_eq(a, b), default: skip_assert_eq(a, b)), \
        default:                skip_assert_eq(a, b) \
    )
#endif

#endif
