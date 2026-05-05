#include "string_poly.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

const size_t POLY_STRING_BUFFER_SIZE = 4096;
const size_t POLY_STRING_ARRAY_INITIAL_CAPACITY = 10;

int is_ascii_only(const char* str) {
    if (!str) return 1;
    while (*str) {
        if ((unsigned char)*str > 127) return 0;
        str++;
    }
    return 1;
}

char* wchar_to_utf8(const wchar_t* wstr) {
    if (!wstr) return NULL;
    size_t len = wcstombs(NULL, wstr, 0);
    if (len == (size_t)-1) return NULL;
    char* utf8 = malloc(len + 1);
    if (!utf8) return NULL;
    wcstombs(utf8, wstr, len + 1);
    return utf8;
}

wchar_t* utf8_to_wchar(const char* utf8) {
    if (!utf8) return NULL;
    size_t len = mbstowcs(NULL, utf8, 0);
    if (len == (size_t)-1) return NULL;
    wchar_t* wstr = malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    mbstowcs(wstr, utf8, len + 1);
    return wstr;
}

void ascii_print_to_buffer(const void* data, char* buffer, size_t buffer_size) {
    if (!data || !buffer || buffer_size == 0) return;
    const char* str = (const char*)data;
    strncpy(buffer, str, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

char* ascii_to_utf8(const void* data) {
    if (!data) return NULL;
    const char* str = (const char*)data;
    char* result = malloc(strlen(str) + 1);
    if (result) strcpy(result, str);
    return result;
}

PolyString* ascii_concat(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    size_t len_a = strlen((const char*)a->data);
    size_t len_b = strlen((const char*)b->data);
    char* result = malloc(len_a + len_b + 1);
    if (!result) return NULL;
    strcpy(result, (const char*)a->data);
    strcat(result, (const char*)b->data);
    return create_poly_string_ascii(result);
}

PolyString* ascii_substring(const PolyString* str, int start, int end) {
    if (!str || !str->data) return NULL;
    const char* data = (const char*)str->data;
    size_t len = strlen(data);
    if (start < 0) start = 0;
    if (end < 0 || end >= (int)len) end = len - 1;
    if (start > end) return NULL;
    size_t sub_len = end - start + 1;
    char* result = malloc(sub_len + 1);
    if (!result) return NULL;
    strncpy(result, data + start, sub_len);
    result[sub_len] = '\0';
    return create_poly_string_ascii(result);
}

int ascii_find(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->data || !pattern->data) return -1;
    const char* t = (const char*)text->data;
    const char* p = (const char*)pattern->data;
    size_t tlen = strlen(t);
    size_t plen = strlen(p);
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    for (size_t i = 0; i <= tlen - plen; i++) {
        int found = 1;
        for (size_t j = 0; j < plen; j++) {
            char tc = t[i + j];
            char pc = p[j];
            if (!sensitive) {
                if (tc >= 'A' && tc <= 'Z') tc = tc - 'A' + 'a';
                if (pc >= 'A' && pc <= 'Z') pc = pc - 'A' + 'a';
            }
            if (tc != pc) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    return -1;
}

size_t ascii_length(const void* data) {
    if (!data) return 0;
    return strlen((const char*)data);
}

void ascii_free(void* data) {
    free(data);
}

void wide_print_to_buffer(const void* data, char* buffer, size_t buffer_size) {
    if (!data || !buffer || buffer_size == 0) return;
    char* utf8 = wchar_to_utf8((const wchar_t*)data);
    if (utf8) {
        strncpy(buffer, utf8, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
        free(utf8);
    } else {
        buffer[0] = '\0';
    }
}

char* wide_to_utf8(const void* data) {
    if (!data) return NULL;
    return wchar_to_utf8((const wchar_t*)data);
}

PolyString* wide_concat(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    const wchar_t* wa = (const wchar_t*)a->data;
    const wchar_t* wb = (const wchar_t*)b->data;
    size_t len_a = wcslen(wa);
    size_t len_b = wcslen(wb);
    wchar_t* result = malloc((len_a + len_b + 1) * sizeof(wchar_t));
    if (!result) return NULL;
    wcscpy(result, wa);
    wcscat(result, wb);
    return create_poly_string_wide(result);
}

PolyString* wide_substring(const PolyString* str, int start, int end) {
    if (!str || !str->data) return NULL;
    const wchar_t* data = (const wchar_t*)str->data;
    size_t len = wcslen(data);
    if (start < 0) start = 0;
    if (end < 0 || end >= (int)len) end = len - 1;
    if (start > end) return NULL;
    size_t sub_len = end - start + 1;
    wchar_t* result = malloc((sub_len + 1) * sizeof(wchar_t));
    if (!result) return NULL;
    wcsncpy(result, data + start, sub_len);
    result[sub_len] = L'\0';
    return create_poly_string_wide(result);
}

int wide_find(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->data || !pattern->data) return -1;
    const wchar_t* t = (const wchar_t*)text->data;
    const wchar_t* p = (const wchar_t*)pattern->data;
    size_t tlen = wcslen(t);
    size_t plen = wcslen(p);
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    for (size_t i = 0; i <= tlen - plen; i++) {
        int found = 1;
        for (size_t j = 0; j < plen; j++) {
            wchar_t tc = t[i + j];
            wchar_t pc = p[j];
            if (!sensitive) {
                if (tc >= L'A' && tc <= L'Z') tc = tc - L'A' + L'a';
                if (pc >= L'A' && pc <= L'Z') pc = pc - L'A' + L'a';
            }
            if (tc != pc) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    return -1;
}

size_t wide_length(const void* data) {
    if (!data) return 0;
    return wcslen((const wchar_t*)data);
}

void wide_free(void* data) {
    free(data);
}

const StringTypeInfo AsciiType = {
    .print_to_buffer = ascii_print_to_buffer,
    .concat = ascii_concat,
    .substring = ascii_substring,
    .find = ascii_find,
    .length = ascii_length,
    .free = ascii_free,
    .to_utf8 = ascii_to_utf8
};

const StringTypeInfo WideType = {
    .print_to_buffer = wide_print_to_buffer,
    .concat = wide_concat,
    .substring = wide_substring,
    .find = wide_find,
    .length = wide_length,
    .free = wide_free,
    .to_utf8 = wide_to_utf8
};

PolyString* create_poly_string_ascii(const char* str) {
    if (!str) return NULL;
    PolyString* poly = malloc(sizeof(PolyString));
    if (!poly) return NULL;
    poly->data = malloc(strlen(str) + 1);
    if (!poly->data) {
        free(poly);
        return NULL;
    }
    strcpy((char*)poly->data, str);
    poly->type = &AsciiType;
    return poly;
}

PolyString* create_poly_string_wide(const wchar_t* str) {
    if (!str) return NULL;
    PolyString* poly = malloc(sizeof(PolyString));
    if (!poly) return NULL;
    poly->data = malloc((wcslen(str) + 1) * sizeof(wchar_t));
    if (!poly->data) {
        free(poly);
        return NULL;
    }
    wcscpy((wchar_t*)poly->data, str);
    poly->type = &WideType;
    return poly;
}

PolyString* create_poly_string_wide_from_utf8(const char* utf8_str) {
    if (!utf8_str) return NULL;
    wchar_t* wstr = utf8_to_wchar(utf8_str);
    if (!wstr) return NULL;
    PolyString* poly = create_poly_string_wide(wstr);
    free(wstr);
    return poly;
}

void free_poly_string(PolyString* str) {
    if (str) {
        if (str->type && str->type->free) {
            str->type->free(str->data);
        }
        free(str);
    }
}

void print_poly_string_to_buffer(const PolyString* str, char* buffer, size_t buffer_size) {
    if (!str || !str->type || !str->type->print_to_buffer) {
        if (buffer && buffer_size > 0) buffer[0] = '\0';
        return;
    }
    str->type->print_to_buffer(str->data, buffer, buffer_size);
}

PolyString* concat_poly_strings(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->type || !b->type || a->type != b->type) {
        return NULL;
    }
    return a->type->concat(a, b);
}

PolyString* substring_poly_string(const PolyString* str, int start, int end) {
    if (!str || !str->type || !str->type->substring) {
        return NULL;
    }
    return str->type->substring(str, start, end);
}

int find_in_poly_string(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->type || !pattern->type || 
        text->type != pattern->type || !text->type->find) {
        return -1;
    }
    return text->type->find(text, pattern, sensitive);
}

size_t poly_string_length(const PolyString* str) {
    if (!str || !str->type || !str->type->length) return 0;
    return str->type->length(str->data);
}

char* poly_string_to_utf8(const PolyString* str) {
    if (!str || !str->type || !str->type->to_utf8) return NULL;
    return str->type->to_utf8(str->data);
}