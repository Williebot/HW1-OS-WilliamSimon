/*

  Word Count using dedicated lists

*/

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

*/

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount *word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64

/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 */
int num_words(FILE* infile) {
    int num_words = 0;
    bool in_word = false;
    int c;

    while ((c = fgetc(infile)) != EOF) {
        if (isalpha(c)) {
            if (!in_word) {
                num_words++;
                in_word = true;
            }
        } else {
            in_word = false;
        }
    }

    return num_words;
}


/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 */
void count_words(WordCount **wclist, FILE *infile) {
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int c;
    
    while ((c = fgetc(infile)) != EOF) {
        if (isalpha(c)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = tolower(c);
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                add_word(wclist, word);
                word_len = 0;
            }
        }
    }
}

/*
 * Comparator to sort list by frequency.
 */
static bool wordcount_less(const WordCount *wc1, const WordCount *wc2) {
    return wc1->count < wc2->count;
}

// Displays a helpful message.
static int display_help(void) {
    printf("Flags:\n"
           "--count (-c): Count the total amount of words in the file, or STDIN if a file is not specified.\n"
           "--frequency (-f): Count the frequency of each word in the file, or STDIN if a file is not specified.\n"
           "--help (-h): Displays this help message.\n");
    return 0;
}


/*
 * Handle command line flags and arguments.
 */
int main (int argc, char *argv[]) {
    printf("Address of main: %p\n", main);

    bool count_mode = true;
    bool freq_mode = false;
    int total_words = 0;
    FILE *infile = NULL;
    int i;
    static struct option long_options[] = {
        {"count", no_argument, 0, 'c'},
        {"frequency", no_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((i = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
        switch (i) {
            case 'c':
                count_mode = true;
                freq_mode = false;
                break;
            case 'f':
                count_mode = false;
                freq_mode = true;
                break;
            case 'h':
                return display_help();
        }
    }

    if (!count_mode && !freq_mode) {
        printf("Please specify a mode.\n");
        return display_help();
    }

    init_words(&word_counts);

    if ((argc - optind) < 1) {
        infile = stdin;
    } else {
        infile = fopen(argv[optind], "r");
        if (infile == NULL) {
            perror("Error opening file");
            return 1;
        }
    }

    if (count_mode) {
        total_words = num_words(infile);
        printf("The total number of words is: %i\n", total_words);
    } else {
        count_words(&word_counts, infile);
        wordcount_sort(&word_counts, wordcount_less);
        printf("The frequencies of each word are: \n");
        fprint_words(word_counts, stdout);
    }

    if (infile != stdin) {
        fclose(infile);
    }

    return 0;
}

