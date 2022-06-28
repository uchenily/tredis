#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 相比fgets()或scanf(), readline()在命令行输入处理上更合适.
#include <readline/readline.h>

#include "htable.h"

/*
 * 实现redis的set/get/del命令
 */
int main() {
    htable_t *htable;
    char *line;
    char *command;
    char *key;
    char *value;
    int ret;

    // line是字符串指针, value是字符串指针
    htable = ht_create();
    assert(htable);
    line = NULL;

    while (1) {
        if (line) {
            free(line);
        }

        line = readline("==> ");
        // 输入ctrl+d(EOF)的情况
        if (!line) {
            printf("EOF\n");
            break;
        }
        command = strtok(line, " \t\n");
        if (!command) {
            continue;
        } else if (!strcmp(command, "q") || !strcmp(command, "quit")) {
            break;
        } else if (!strcmp(command, "get")) {
            key = strtok(NULL, " \t\n");
            if (key) {
                value = ht_get(htable, key);
                if (value) {
                    printf("%s\n", value);
                } else {
                    printf("(nil)\n");
                }
            }
        } else if (!strcmp(command, "set")) {
            key = strtok(NULL, " \t\n");
            if (key) {
                // value中可以包含空格, tab等
                value = strtok(NULL, "\n");
                if (value) {
                    value = strdup(value);
                    ret = ht_set(htable, key, value);
                    assert(ret == 0);
                    printf("OK\n");
                }
            }
        } else if (!strcmp(command, "del")) {
            key = strtok(NULL, " \t\n");
            if (key) {
                ht_del(htable, key);
            }
        } else if (!strcmp(command, "help")) {
            printf("Tiny redis works in memory.\n\n"
                   "USAGE:\n"
                   "  get [key]\n"
                   "  set [key] [value]\n"
                   "  del [key]\n"
                   "  quit\n");
        } else {
            printf("Unknown command: %s\n", command);
            printf("Type help for more information\n");
        }
    }

#if 0
    printf("hashtable length: %d\n", ht_length(htable));

    printf("%s\t%s\n", "Key", "Value");
    hti entry;
    ht_for_each(entry, htable) {
        printf("%s\t%s\n", entry.key, (char *)entry.value);
    }
#endif

    ht_destroy(htable);
    return 0;
}
