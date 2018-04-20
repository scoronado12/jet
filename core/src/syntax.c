/**
 * syntax.c
 * Implements functions for matching syntax
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <core/attribute.h>
#include <core/syntax.h>
#include <core/regex.h>
#include <core/line.h>

typedef struct _rule {
    char *s;
    int len;
    enum attr_type type;
} rule;

static bool syntax_enabled = false;

/*
 * rule arrays:
 *  - key is for keywords, those are simply literal matches
 *  - reg is for a regular expression to match
 *  - enc is encapsulations. enc_b stores the beginning rules while enc_e stores
 *      the corresponding end rules
 */
static rule *key, *reg, *enc_b, *enc_e;
static int key_len, reg_len, enc_len;

/* sets up syntax  */
void syntax_init(buffer *b) {
    // if we don't have a name, no syntax for you
    if (b->name == NULL) {
        return;
    }

    // grab the extension
    const char *ext = strchr(b->name, '.');
    if (!ext || ext == b->name || *(ext + 1) == '\0') { // did we get it?
        return;
    }
    ext++;

    // if we don't support the extension, no syntax for you
    if (!(strcmp(ext, "c") == 0 || strcmp(ext, "h") == 0)) {
        return;
    }

    // hoo boy we're in the clear, let's go
    // keywords first
    key_len = 1;
    key = malloc(sizeof(rule));
    key[0].len = 3;
    key[0].s = malloc(4);
    strcpy(key[0].s, "int");
    key[0].type = COLOR1;

    // regular expressions
    reg_len = 0;

    // encapsulations
    enc_len = 0;

    syntax_enabled = true;
}

/* clears all syntax rules */
void syntax_end() {
    for (int i = 0; i < key_len; i++) {
        free(key[i].s);
    }
    free(key);
    key_len = 0;
}

/* generate syntax attributes for the given buffer */
void gen_syntax(buffer *b) {
    // only proceed if syntax is enabled
    if (!syntax_enabled) {
        return;
    }
    
    int y, x;
    line *prev, *curr, *next;

    // iterate over each line
    for (y = 0; y < b->len; y++) {
        x = 0;
        curr = b->lines[y];

        // only regenerate if needed
        if (!curr->needs_update) {
            continue;
        }

        lclrattrs(curr);

        // check each x for matches to any rule
        do {
            // keywords
            for (int i = 0; i < key_len; i++) {
                rule r = key[i];

                if (re_match(r.s, curr->s + x) == 0) {
                    // create attrs
                    attribute *beg;
                    attribute *end;

                    beg->enabled = true;
                    beg->type = COLOR1;

                    end->enabled = false;
                    end->type = COLOR1;
                    
                    // update x


                    // break
                }
            }

            x++;
        } while (x < curr->len);
    }
}

