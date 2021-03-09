#include <stdio.h>
#include <ctype.h>

#define N 255

int check_letter_or_num_wss(char w, char custom) {
    if ((w == custom) || (custom == '\"' && isalpha(w)) ||
        (custom == '\'' && isdigit(w))) {
        return 1;
    }
    return 0;
}

int check_not_letter(char **word, char letter) { //~
    if (**word != letter && **word != '\0') {
        *word += 1;
        return 1;
    }
    return 0;
}

//ss: ' - 0-9; " - a-z, A-Z
int check_letter_or_num(char **word, char reg) { //13dsaf
    if (check_letter_or_num_wss(**word, reg)) {
        *word += 1;
        return 1;
    }
    return 0;
}

int check_many_entry_in_word(char **word, char *custom_letter, int n, int len) { //[|digit|*(xx)]
    for (int i = 0; i < n * len; ++i) {
        if (!check_letter_or_num_wss(**word, *custom_letter)) {
            return 0;
        }
        *word += 1;
        custom_letter = custom_letter + 1;
        if ((i + 1) % len == 0)
            custom_letter -= len;
    }
    return 1;
}

void check_reg_exp(char *word, int *point, char *reg) {
    for (int i = 1; *reg != '\0';) {
        if (isalnum(*reg) || *reg == '\\') {
            char checkable_char = *reg;
            if (*reg == '\\') {
                reg += 1;
                checkable_char = *reg == 'd' ? '\'' : '\"';
            }
            if (!check_letter_or_num(&word, checkable_char)) {
                *point = 1;
                return;
            }
        } else if (*reg == '[') {
            int times = 0;
            reg += 1;
            while (isdigit(*reg)) {
                times = (int) (times * 10 + *reg - '0');
                reg += 1;
            }
            char custom_word[64];
            reg += 2;
            int j = 0;
            while (isalpha(*reg) || *reg == '\\') {
                if (*reg == '\\') {
                    reg += 1;
                    custom_word[j] = *reg == 'd' ? '\'' : '\"';
                    j += 1;
                    reg += 1;
                } else {
                    custom_word[j] = *reg;
                    j += 1;
                    reg += 1;
                }
            }
            custom_word[j + 1] = '\0';
            if (!check_many_entry_in_word(&word, custom_word, times, j)) {
                *point = 1;
                return;
            }
        } else if (*reg == '~') {
            reg += 1;
            if (!check_not_letter(&word, *reg)) {
                *point = 1;
                return;
            }
        } else if (*reg == '<') {
            reg += 1;
            char custom_word[64];
            int j = 0;
            while (isalpha(*reg) || *reg == '\\') {
                if (*reg == '\\') {
                    reg += 1;
                    custom_word[j] = *reg == 'd' ? '\'' : '\"';
                    j += 1;
                    reg += 1;
                } else {
                    custom_word[j] = *reg;
                    j += 1;
                    reg += 1;
                }
            }
            while (check_many_entry_in_word(&word, custom_word, 1, j)){
//                check_many_entry_in_word(&word, custom_word, 1, j);
            }
            reg += 1;
        }
        reg += i;
    }
    if (*word != '\0') {
        *point = 1;
    }
}


int main() {
    char string[64];
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
