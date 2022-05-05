#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "document_analyzer.h"

#define LINE_LENGTH (100)
#define ASCII_LEN (127)

static char**** s_pa_doc_tree = NULL;
static char* s_pa_doc_str = NULL;

static char* load_doc_malloc(FILE* fstream) /* runtime memory allocation for variable length of document */
{
    char line[LINE_LENGTH];
    char* pa_line = NULL;
    size_t realloc_size = 0;

    pa_line = malloc(LINE_LENGTH);
    *pa_line = '\0';
    while (fgets(line, LINE_LENGTH, fstream) != NULL) {
        realloc_size = strlen(pa_line) + strlen(line) + 1;
        pa_line = realloc(pa_line, realloc_size);
        strcat(pa_line, line);
    }

    return pa_line;
}

static char* tokenize_only_by_delim(char* str_or_null, const char* delim)
{
    static char* s_word_start;
    char* p_str;
    char* ret;

    unsigned char is_delim[127];

    memset(is_delim, 0, 127);
    while (*delim != '\0') { /* set ascii validation */
        is_delim[(int)*delim] = 1;
        ++delim;
    }

    if (str_or_null != NULL) {
        s_word_start = str_or_null;
    }
    p_str = s_word_start;

    while (is_delim[(int)*p_str] && *p_str != '\0') { /* preset starting point */
        *p_str = '\0';
        ++s_word_start;
        ++p_str;
    }

    while (!is_delim[(int)*p_str] && *p_str != '\0') {
        ++p_str;
    }

    if (*p_str == '\0') {
        s_word_start = NULL;
        return NULL;
    }

    while (is_delim[(int)*p_str] && *p_str != '\0') {
        *p_str = '\0';
        ++p_str;
    }

    ret = p_str - (p_str - s_word_start);
    s_word_start = p_str;

    return ret;
}

static char** tokenize_malloc(char* str, const char* delim, char* (*tokenizer)(char* str_or_null, const char* delim))
{
    char** pa_str_list = NULL;
    char* token;
    size_t num_paragraph = 0;

    token = tokenizer(str, delim);
    while (token != NULL) {
        pa_str_list = realloc(pa_str_list, (num_paragraph + 1) * sizeof(char*));
        pa_str_list[num_paragraph++] = token;
        token = tokenizer(NULL, delim);
    }

    pa_str_list = realloc(pa_str_list, (num_paragraph + 1) * sizeof(char*));
    pa_str_list[num_paragraph] = NULL;

    return pa_str_list;
}

static int get_len(char* arr[]) /* get length of string array. */
{
    char** p_arr = arr;

    while (*p_arr != NULL) {
        ++p_arr;
    }

    return p_arr - arr;
}


int load_document(const char* document)
{
    char** pa_paragraph_strs;
    FILE* fstream;
    size_t paragraph_count;
    size_t i = 0;
    size_t j = 0;
    
    if (s_pa_doc_tree != NULL) {
        dispose();
    }

    fstream = fopen(document, "rb");
    if (fstream == NULL) {
        return FALSE;
    }

    s_pa_doc_str = load_doc_malloc(fstream);
    pa_paragraph_strs = tokenize_malloc(s_pa_doc_str, "\r\n", strtok);
    paragraph_count = get_len(pa_paragraph_strs);
    if (paragraph_count == 0) {
        goto end;
    }

    s_pa_doc_tree = malloc((paragraph_count + 1) * sizeof(char***));
    s_pa_doc_tree[paragraph_count] = NULL;

    for (i = 0; pa_paragraph_strs[i] != NULL; ++i) {
        char** pa_sentence_strs;
        char*** paragraph_tree = NULL;
        size_t sentence_count;

        pa_sentence_strs = tokenize_malloc(pa_paragraph_strs[i], ".!?", tokenize_only_by_delim);

        sentence_count = get_len(pa_sentence_strs);
        paragraph_tree = malloc((sentence_count + 1) * sizeof(char**));
        paragraph_tree[sentence_count] = NULL;
        s_pa_doc_tree[i] = paragraph_tree;
        
        for (j = 0; pa_sentence_strs[j] != NULL; ++j) {
            char** pa_words;
            pa_words = tokenize_malloc(pa_sentence_strs[j], " ,", strtok);
            s_pa_doc_tree[i][j] = pa_words;
        }

        free(pa_sentence_strs);
    }

end:
    free(pa_paragraph_strs);
    fclose(fstream);

    return TRUE;
}

void dispose(void)
{
    char**** pppp = s_pa_doc_tree;

    if (s_pa_doc_tree == NULL) {
        return;
    }

    while (*pppp != NULL) {
        char*** ppp = *pppp;
        while (*ppp != NULL) {
            free(*ppp);

            ++ppp;
        }
        free(*pppp);

        ++pppp;
    }

    free(s_pa_doc_tree);
    free(s_pa_doc_str);
    s_pa_doc_tree = NULL;
}

size_t get_total_word_count(void)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t word_count = 0;

    if (s_pa_doc_tree == NULL) {
        return 0;
    }

    for (i = 0; s_pa_doc_tree[i] != NULL; ++i) {
        for (j = 0; s_pa_doc_tree[i][j] != NULL; ++j) {
            for (k = 0; s_pa_doc_tree[i][j][k] != NULL; ++k) {
                ++word_count;
            }
        }
    }

    return word_count;
}

size_t get_total_sentence_count(void)
{
    size_t i = 0;
    size_t j = 0;
    size_t sentence_count = 0;

    if (s_pa_doc_tree == NULL) {
        return 0;
    }

    for (i = 0; s_pa_doc_tree[i] != NULL; ++i) {
        for (j = 0; s_pa_doc_tree[i][j] != NULL; ++j) {
            ++sentence_count;
        }
    }

    return sentence_count;
}

size_t get_total_paragraph_count(void)
{
    size_t i = 0;
    size_t paragraph_count = 0;

    if (s_pa_doc_tree == NULL) {
        return 0;
    }

    for (i = 0; s_pa_doc_tree[i] != NULL; ++i) {
        ++paragraph_count;
    }

    return paragraph_count;
}

const char*** get_paragraph_or_null(const size_t paragraph_index)
{
    if (paragraph_index + 1 > get_total_paragraph_count()) {
        return NULL;
    }

    return (const char***)s_pa_doc_tree[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    const char*** p_paragraph = paragraph;
    size_t i = 0;
    size_t j = 0;
    size_t count = 0;

    if (paragraph == NULL) {
        return 0;
    }

    for (i = 0; p_paragraph[i] != NULL; ++i) {
        for (j = 0; p_paragraph[i][j] != NULL; ++j) {
            ++count;
        }
    }

    return count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t i = 0;
    size_t count = 0;

    if (paragraph == NULL) {
        return 0;
    }

    for (i = 0; paragraph[i] != NULL; ++i) {
        ++count;
    }

    return count;
}

const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index)
{
    const char*** paragraph = get_paragraph_or_null(paragraph_index);

    if (paragraph == NULL) {
        return NULL;
    }

    if (sentence_index + 1 > get_paragraph_sentence_count(paragraph)) {
        return NULL;
    }

    return (const char**)s_pa_doc_tree[paragraph_index][sentence_index];
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t i = 0;
    size_t count = 0;

    if (sentence == NULL) {
        return 0;
    }

    for (i = 0; sentence[i] != NULL; ++i) {
        ++count;
    }

    return count;
}

int print_as_tree(const char* filename)
{
    FILE* fstream;
    int i = 0;
    int j = 0;
    int k = 0;

    if (s_pa_doc_tree == NULL) {
        return FALSE;
    }

    fstream = fopen(filename, "wb");
    if (fstream == NULL) {
        return FALSE;
    }

    for (i = 0; s_pa_doc_tree[i] != NULL; ++i) {
        fprintf(fstream, "Paragraph %d:\n", i);
        for (j = 0; s_pa_doc_tree[i][j] != NULL; ++j) {
            fprintf(fstream, "    Sentence %d:\n", j);
            for (k = 0; s_pa_doc_tree[i][j][k] != NULL; ++k) {
                if (s_pa_doc_tree[i + 1] == NULL && s_pa_doc_tree[i][j + 1] == NULL && s_pa_doc_tree[i][j][k + 1] == NULL) {
                    fprintf(fstream, "        %s", s_pa_doc_tree[i][j][k]);
                } else {
                    fprintf(fstream, "        %s\n", s_pa_doc_tree[i][j][k]);
                }
            }
        }

        if (s_pa_doc_tree[i + 1] != NULL) {
            fprintf(fstream, "\n");
        }        
    }

    fclose(fstream);

    return TRUE;
}
