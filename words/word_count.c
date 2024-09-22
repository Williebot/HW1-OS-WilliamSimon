/*

Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/
#include "word_count.h"

/* Basic utilities */

char *new_string(char *str) {
    return strcpy((char *)malloc(strlen(str) + 1), str);
}

void init_words(WordCount **wclist) {
    *wclist = NULL;
}

size_t len_words(WordCount *wchead) {
    size_t len = 0;
    while (wchead != NULL) {
        len++;
        wchead = wchead->next;
    }
    return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
    while (wchead != NULL) {
        if (strcmp(wchead->word, word) == 0) {
            return wchead;
        }
        wchead = wchead->next;
    }
    return NULL;
}

void add_word(WordCount **wclist, char *word) {
    WordCount *wc = find_word(*wclist, word);
    if (wc != NULL) {
        wc->count++;
    } else {
        WordCount *new_wc = malloc(sizeof(WordCount));
        new_wc->word = new_string(word);
        new_wc->count = 1;
        new_wc->next = *wclist;
        *wclist = new_wc;
    }
}

void fprint_words(WordCount *wchead, FILE *ofile) {
    WordCount *wc;
    for (wc = wchead; wc != NULL; wc = wc->next) {
        fprintf(ofile, "%d\t%s\n", wc->count, wc->word);
    }
}
void wordcount_sort(WordCount **wclist, bool less(const WordCount *, const WordCount *)) {
    // Sort the word count list using a sorting algorithm like bubble sort or insertion sort.
    WordCount *sorted = NULL;
    WordCount *current = *wclist;
    WordCount *next;

    while (current != NULL) {
        next = current->next;
        wordcount_insert_ordered(&sorted, current, less);
        current = next;
    }

    *wclist = sorted;
}
void wordcount_insert_ordered(WordCount **wclist, WordCount *elem, bool less(const WordCount *, const WordCount *)) {
    WordCount **current = wclist;
    
    while (*current != NULL && !less(elem, *current)) {
        current = &(*current)->next;
    }
    
    elem->next = *current;
    *current = elem;
}

