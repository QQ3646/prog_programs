#include <stdio.h>
#include <ctype.h>

#define N 255 //размер строки
#define R_S 64 //размер регул. выражения

//ss: ' - 0-9; " - a-z, A-Z
int check_letter_or_num(char w, char custom) {
    if ((w == custom) || (custom == '\"' && isalpha(w)) ||
        (custom == '\'' && isdigit(w))) {
        return 1;
    }
    return 0;
}

int check_not_letter(char **word, char letter) { //~
    if (**word != letter && **word != '\0') {
        (*word)++;
        return 1;
    }
    return 0;
}



int check_many_entry_in_word(char **word, char *custom_letter, int n, int len) { //[|digit|*(xx)]
    for (int i = 0; i < n * len; ++i) {
        if (!check_letter_or_num(**word, *custom_letter)) {
            return 0;
        }
        (*word)++;
        custom_letter = custom_letter + 1;
        if ((i + 1) % len == 0)
            custom_letter -= len;
    }
    return 1;
}

void check_reg_exp(char *word, int *point, char *reg) {
    while (*reg != '\0') {
        if (isalnum(*reg) || *reg == '\\') {
            char checkable_char = *reg;
            if (*reg == '\\') {
                reg++;
                checkable_char = *reg == 'd' ? '\'' : '\"';
            }
            if (!check_letter_or_num(*word, checkable_char)) {
                *point = 1;
                return;
            } else {
                word++;
                *point = 0;
            }
        } else if (*reg == '[') {
            int times = 0;
            reg++;
            while (isdigit(*reg)) {
                times = (int) (times * 10 + *reg - '0');
                reg++;
            }
            char custom_word[R_S];
            reg += 2;
            int j = 0;
            while (isalpha(*reg) || *reg == '\\') {
                if (*reg == '\\') {
                    reg++;
                    custom_word[j] = *reg == 'd' ? '\'' : '\"';
                    j += 1;
                    reg++;
                } else {
                    custom_word[j] = *reg;
                    j += 1;
                    reg++;
                }
            }
            custom_word[j + 1] = '\0';
            if (!check_many_entry_in_word(&word, custom_word, times, j)) {
                *point = 1;
                return;
            } else
                *point = 0;
        } else if (*reg == '~') {
            reg++;
            if (!check_not_letter(&word, *reg)) {
                *point = 1;
                return;
            } else
                *point = 0;
        } else if (*reg == '<') {
            reg++;
            char custom_word[R_S];
            int j = 0;
            while (isalpha(*reg) || *reg == '\\') {
                if (*reg == '\\') {
                    reg++;
                    custom_word[j] = *reg == 'd' ? '\'' : '\"';
                    j++;
                    reg++;
                } else {
                    custom_word[j] = *reg;
                    j++;
                    reg++;
                }
            }
            reg+=2;
            do {
                check_reg_exp(word, point, reg);
                if (*point == 0)
                    return;
            } while (check_many_entry_in_word(&word, custom_word, 1, j));
            return;
        }
        reg++;
    }
    if (*word != '\0') {
        *point = 1;
    }
}


int main() {
    char string[R_S];
    int n;
    int valid_str[N] = {};
    char word[1024];
    scanf("%s", string);
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%s", word);
        check_reg_exp(word, &valid_str[i], string);
    }
    int b = 0;
    for (int i = 0; i < n; ++i) {
        if (!valid_str[i]) {
            printf("%i ", i);
            b = 1;
        }
    }
    if (b == 0) {
        printf("none");
    }
}
