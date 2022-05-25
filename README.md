## 주요 기능
- `.txt` 파일을 분석 후 파일의 정보(문단 개수, 문장 개수, 단어 개수 등)를 제공합니다

## 테스트
- `main.c` 에 있는 테스트 코드 참고
- 프로젝트의 root 디랙토리로 이동해서 컴파일 합니다  
`clang -std=c89 -W -Wall -pedantic-errors *.c`
- 실행합니다  
`./a.out`

## 기능 목록
##### 문서 로딩하기
- `int load_document(const char* document)`
- 매개변수로 받은 이름에 해당하는 파일을 한줄씩 읽어서 메모리에 올린다
- 문단 -> 문장 -> 단어 로 이어지는 tree구조의 자료형을 메모리에 저장한다

##### 메모리 해제하기
- `void dispose(void)`
- 로드된 tree구조의 데이터 및 동적할당 된 메모리를 해제한다

##### 전체 단어 수 가져오기
- `size_t get_total_word_count(void)` 
- 문서에 있는 전체 단어의 개수를 반환한다

##### 전체 문장 수 가져오기
- `size_t get_total_sentence_count(void)`
- 문서에 있는 전체 문장의 개수를 반환한다

##### 전체 문단 수 가져오기
- `size_t get_total_paragraph_count(void)`
- 문서에 있는 전체 문단의 개수를 반환한다

##### 지정된 문단 가져오기
- `const char*** get_paragraph_or_null(const size_t paragraph_index)`
- 매개변수로 받은 index에 해당하는 문단의 배열을 가져옵니다
- 아래와 같은 포인터 배열을 반환을 반환합니다.  
`[ [ "Egestas", "diam", "in", "arcu", "cursus", "euismod", "quis", "viverra" ], [ "Nunc", "non", "blandit", "massa", "enim", "nec", "dui" ] ]`

##### 지정된 문장 가져오기
- `const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index)`
- 매개변수로 받은 `paragraph_index` 해당하는 문단에 `sentence_index`에 해당하는 문장을 가져옵니다
- 아래와 같은 포인터 배열을 반환합니다.  
`[ "Nisl", "purus", "in", "mollis", "nunc", "sed", "id", "semper", "risus" ]`

##### 문단에 있는 단어 개수 가져오기
- `size_t get_paragraph_word_count(const char*** paragraph)`
- 매개변수로 받은 문단 배열에서 단어의 총 개수를 반환합니다.

##### 문단에 있는 문장의 개수 가져오기
- `size_t get_paragraph_sentence_count(const char*** paragraph)`
- 매개 변수로 받은 문단 배열에서 문장의 개수를 반환합니다.

##### 문장에 있는 단어의 개수 가져오기
- `size_t get_sentence_word_count(const char** sentence)`
- 매개변수로 받은 만장 배열에서 단어의 개수를 반환합니다.

##### 구조 출력하기
- `int print_as_tree(const char* filename)`
- `filename` 에 해당하는 파일에 다음과 같은 트리 구조의 데이터를 출력합니다  
```
Paragraph 0:
    Sentence 0:
        You
        better
        watch
        out
    Sentence 1:
        You
        better
        not
        cry
    Sentence 2:
        You
        better
        not
        pout
    Sentence 3:
        I'm
        telling
        you
        why

Paragraph 1:
    Sentence 0:
        Santa
        Clause
        is
        coming
        to
        town
        for
        reals
    Sentence 1:
        It's
        no
        Joke

Paragraph 2:
    Sentence 0:
        He's
        making
        a
        list
    Sentence 1:
        He's
        checking
        it
        twice
    Sentence 2:
        Gonna
        find
        out
        who's
        naughty
        and
        nice
    Sentence 3:
        So
        be
        nice
```

## 이슈 로그
### 상황
- delimi
