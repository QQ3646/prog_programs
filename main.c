#include<stdio.h>
#include <ctype.h>

#define N 1024
#define R_S 64

int check_letter(char word, char *letter) {
    return (word == *letter || (*letter == '\\' && ((*(letter + 1) == 'd' && isdigit(word)) ||
                                                    (*(letter + 1) == 'D' && isalpha(word)))));
}

int check_entry_of_word(char *word, char *c_word) {
    while (*(c_word)) {
        if (!check_letter(*(word++), c_word))
            return 0;
        if (*(c_word++) == '\\')
            c_word++;
    }
    return 1;
}

int check_reg(char *word, char *reg) {
    while (*reg) {
        if (*reg == '[') {
            int count = 0;
            while (isdigit(*(++reg))) {
                count *= 10;
                count += *reg - '0';
            }
            reg++;
            char new_word[N] = {};
            int j = 0;
            int counter = 0;
            while (*(++reg) != ')') {
                new_word[j] = *reg;
                if (new_word[j++] == '\\')
                    counter++;
            }
            j -= counter; //отнимаем, пушто выражение "\d" или "\D" занимает два места в регулярных выражениях, но при этом отвечает всего за один символ в слове
            for (int i = 0; i < count; i++) {
                if (!check_entry_of_word(word, new_word))
                    return 0;
                word += j;
            }
            reg += 2;
        } else if (*reg == '~') {
            reg++;
            if (check_letter(*word, reg++) || *(word++) == '\0')
                return 0;
        } else if (*reg == '<') {
            char new_reg[N] = {};
            int j = 0;
            int counter = 0;
            while (*(++reg) != '>') {
                new_reg[j] = *reg;
                if (new_reg[j++] == '\\')
                    counter++;
            }
            j -= counter;
            reg += 2;
            /* Вариант 1, без бесконечного цикла
             * do {
                if (check_reg(word, reg))
                    return 1;
                word += j;
            } while (check_entry_of_word(word, new_reg));
            return check_reg(word, reg); //обработка случая, когда все символы из <>* уже вышли
            */

//          >Вариант 2, но с бесконечным циклом
            while (1) {
                if (check_reg(word, reg))
                    return 1;
                if (!check_entry_of_word(word, new_reg))
                    break;
                word += j;
            }
            return 0;
            //Не знаю что лучше из этих двух вариантов, оба вроде бы рабочие, но первый красивее
        } else if (isalnum(*reg) || *reg == '\\') {
            if (!check_letter(*(word++), reg))
                return 0;
            if (*(reg++) == '\\')
                reg++;
        }
    }
    return !*word;
}

int main() {
    char reg[R_S] = {};
    char word[N] = {};
    int n;
    scanf("%s", reg);
    scanf("%d", &n);
    int b = 0;
    for (int i = 0; i < n; i++) {
        scanf("%s", word);
        if (check_reg(word, reg)) {
            b = 1;
            printf("%d ", i);
        }
    }
    if (!b)
        printf("none");
}