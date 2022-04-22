#include "../string.h"

/**
  * @param [in] originalString1 比較対象1
  * @param [in] originalString2 比較対象2
  * @return 比較対象が等しいならば0を返す
  *
  * ```c
  * #include "string.h"
  *
  * int main() {
  *     if( strcmp("a",  "a" ) != 0 ) return 1;
  *     if( strcmp("a",  "b" ) == 0 ) return 1;
  *     if( strcmp("aa", "a" ) == 0 ) return 1;
  *     if( strcmp("a",  "aa") == 0 ) return 1;
  *     return 0;
  * }
  * ```
  */
int strcmp(const char* originalString1, const char* originalString2) {
    char* string1 = (char*)originalString1;
    char* string2 = (char*)originalString2;

    for (;*string1 == *string2; string1++, string2++) {
        if(*string1 == '\0') return 0;
    }
    return *string1 - *string2;
}

// /**
//   * @brief 指定した文字列の中から指定した文字が最初に出現する位置を特定する
//   * @param [in] string          検索対象の文字列
//   * @param [in] searchTarget    検索文字
//   * @return 指定した文字が最初に出現する位置
//   **/
// char* strchr(const char* string, int searchTarget) {
//     char character = (char)searchTarget;
//     while (*string) {
//         if(*string == character) return (char *)string;
//         string++;
//     }
//     return NULL;
// }

// char *strncpy(char *s1, const char *s2, size_t n) {
//     while (*s1++ = *s2++ && n--);
//     return s1;
// }
