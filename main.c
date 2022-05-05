#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "document_analyzer.h"

static void test_get_count(void)
{
    load_document("./test/input1.txt");

    assert(get_total_word_count() == 62);
    assert(get_total_sentence_count() == 6);
    assert(get_total_paragraph_count() == 3);



    dispose();
}

static void test_get_paragraph(void)
{
    const char*** paragraph;
    load_document("./test/input1.txt");

    paragraph = get_paragraph_or_null(2);

    assert(strcmp(paragraph[0][0], "Egestas") == 0);
    assert(strcmp(paragraph[0][7], "viverra") == 0);
    assert(strcmp(paragraph[1][0], "Nunc") == 0);
    assert(strcmp(paragraph[1][6], "dui") == 0);

    dispose();
}

static void test_get_paragraph_word_count(void)
{
    const char*** paragraph;

    load_document("./test/input1.txt");

    paragraph = get_paragraph_or_null(2);

    assert(get_paragraph_word_count(paragraph) == 15);

    dispose();
}

static void test_get_paragraph_sentence_count(void)
{
    const char*** paragraph;

    load_document("./test/input1.txt");

    paragraph = get_paragraph_or_null(2);

    assert(get_paragraph_sentence_count(paragraph) == 2);

    dispose();
}

static void test_get_sentence(void)
{
    const char** sentence;
    load_document("./test/input1.txt");

    sentence = get_sentence_or_null(1, 1);

    assert(strcmp(sentence[0], "Nisl") == 0);
    assert(strcmp(sentence[8], "risus") == 0);

    dispose();
}

static void test_get_sentence_word_count(void)
{
    const char** sentence;

    load_document("./test/input1.txt");

    sentence = get_sentence_or_null(1, 1);

    assert(get_sentence_word_count(sentence) == 9);

    dispose();
}

static void test_description()
{
    const char** sentence = NULL;
    const char*** paragraph = NULL;
    size_t i = 0U;
    size_t j = 0U;

    assert(load_document("./test/doesntexist.txt") == FALSE);
    assert(load_document("./test/input.txt") == TRUE);

    assert(get_total_word_count() == 58U);
    assert(get_total_sentence_count() == 9U);
    assert(get_total_paragraph_count() == 3U);

    assert(get_paragraph_or_null(3) == NULL);

    paragraph = get_paragraph_or_null(1);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 14);
    assert(get_paragraph_sentence_count(paragraph) == 2);

    assert(strcmp(paragraph[i][j++], "This") == 0);
    assert(strcmp(paragraph[i][j++], "is") == 0);
    assert(strcmp(paragraph[i][j++], "the") == 0);
    assert(strcmp(paragraph[i][j++], "second") == 0);
    assert(strcmp(paragraph[i++][j++], "paragraph") == 0);

    j = 0U;

    assert(strcmp(paragraph[i][j++], "I") == 0);
    assert(strcmp(paragraph[i][j++], "wonder") == 0);
    assert(strcmp(paragraph[i][j++], "how") == 0);
    assert(strcmp(paragraph[i][j++], "many") == 0);
    assert(strcmp(paragraph[i][j++], "sentences") == 0);
    assert(strcmp(paragraph[i][j++], "you") == 0);
    assert(strcmp(paragraph[i][j++], "will") == 0);
    assert(strcmp(paragraph[i][j++], "see") == 0);
    assert(strcmp(paragraph[i][j++], "here") == 0);

    assert(get_sentence_or_null(0, 10) == NULL);

    sentence = get_sentence_or_null(0, 2);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 4);

    i = 0U;

    assert(strcmp(sentence[i++], "Is") == 0);
    assert(strcmp(sentence[i++], "this") == 0);
    assert(strcmp(sentence[i++], "too") == 0);
    assert(strcmp(sentence[i++], "easy") == 0);

    assert(print_as_tree("./test/output.txt") == TRUE);

    dispose();
}

/* wiki test */
void test_empty(void)
{
    assert(load_document("./test/input_empty.txt") == TRUE);
    assert(print_as_tree("./test/output_empty.txt") == FALSE);
    assert(get_total_word_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_paragraph_count() == 0U);

    dispose();
}

void test_document(void)
{
    const char** sentence = NULL;
    const char*** paragraph = NULL;
    assert(get_paragraph_or_null(0) == NULL);
    assert(get_sentence_or_null(0, 0) == NULL);
    assert(get_total_word_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_paragraph_count() == 0U);
    assert(print_as_tree("./test/output1.txt") == FALSE);

    assert(load_document("./test/input1.txt") == TRUE);

    assert(get_total_word_count() == 62U);
    assert(get_total_sentence_count() == 6U);
    assert(get_total_paragraph_count() == 3U);

    paragraph = get_paragraph_or_null(0);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 19);
    assert(get_paragraph_sentence_count(paragraph) == 1);
    sentence = get_sentence_or_null(0, 0);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 19);
    assert(get_sentence_or_null(0, 1) == NULL);

    paragraph = get_paragraph_or_null(1);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 28);
    assert(get_paragraph_sentence_count(paragraph) == 3);
    sentence = get_sentence_or_null(1, 0);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 10);
    sentence = get_sentence_or_null(1, 1);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 9);
    sentence = get_sentence_or_null(1, 2);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 9);
    assert(get_sentence_or_null(1, 3) == NULL);

    paragraph = get_paragraph_or_null(2);
    assert(paragraph != NULL);
    assert(get_paragraph_word_count(paragraph) == 15);
    assert(get_paragraph_sentence_count(paragraph) == 2);
    sentence = get_sentence_or_null(2, 0);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 8);
    sentence = get_sentence_or_null(2, 1);
    assert(sentence != NULL);
    assert(get_sentence_word_count(sentence) == 7);
    assert(get_sentence_or_null(2, 2) == NULL);

    assert(get_paragraph_or_null(3) == NULL);

    assert(print_as_tree("./test/output1.txt") == TRUE);

    dispose();
}

static void test_before_load(void)
{
    dispose();
    assert(get_total_word_count() == 0U);
    assert(get_total_sentence_count() == 0U);
    assert(get_total_paragraph_count() == 0U);

    assert(get_paragraph_or_null(0) == NULL);
    assert(get_paragraph_word_count(NULL) == 0);
    assert(get_paragraph_sentence_count(NULL) == 0);

    assert(get_sentence_or_null(0, 0) == NULL);
    assert(get_sentence_word_count(NULL) == 0);
    
    assert(print_as_tree("./test/output1.txt") == FALSE);
}

static void test_bot1(void)
{
    assert(load_document("./test/input2.txt") == TRUE);
    assert(load_document("./test/input2.txt") == TRUE);
    
    assert(get_total_paragraph_count() == 3);
    assert(get_total_sentence_count() == 10);
    assert(get_total_word_count() == 45);

    assert(print_as_tree("./test/output2.txt") == TRUE); 

    dispose();
}

static void test_empty2(void) 
{
    /* F00_DocNotLoadedPrintAsTree */
    assert(load_document("./test/input2.txt") == TRUE);
    assert(load_document("./test/input_empty.txt") == TRUE);
    dispose();
    assert(print_as_tree("./test/output_empty") == FALSE);

    /* F01_EmptyDocPrintAsTree */
    assert(load_document("./test/input_empty.txt") == TRUE);
    assert(print_as_tree("./test/output_empty") == FALSE);

    dispose();
}



int main(void)
{
    test_get_count();
    test_get_paragraph();
    test_get_paragraph_word_count();
    test_get_paragraph_sentence_count();
    test_get_sentence();
    test_get_sentence_word_count();
    test_description();

    /* wiki test */
    test_empty();
    test_document();
    test_before_load();

    /* build bot test */
    test_bot1();
    test_empty2();

    return 0;
}
