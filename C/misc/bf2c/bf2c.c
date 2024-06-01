
#include <stdio.h>
#include <stdlib.h>

/* NOTE: Can't calculate the heap size when parsing since we would have to
 * account for the loops and that basically means running the brainfuck from
 * here. */
#define HEAP_SZ 4096

#define INDENT_STEP 4

static int indentation = 0;

/*----------------------------------------------------------------------------*/

static char* parseInput(void) {
    size_t i = 0, buf_sz = 100;
    char* buf = calloc(buf_sz, sizeof(char));

    int c;
    while ((c = getchar()) != EOF) {
        if (i >= buf_sz) {
            buf_sz += 100;
            buf = realloc(buf, buf_sz);
        }

        switch (c) {
            case '+':
            case '-':
            case '<':
            case '>':
            case '[':
            case ']':
            case '.':
            case ',':
                /* Valid chars */
                buf[i++] = c;
                break;
            default:
                /* Ignore invalid chars and comments */
                break;
        }
    }

    buf[i] = '\0';
    return buf;
}

static inline void printIndented(const char* str) {
    for (int i = 0; i < indentation; i++)
        putchar(' ');

    printf("%s\n", str);
}

/*----------------------------------------------------------------------------*/

int main(void) {
    /* Get the input from the user */
    char* input = parseInput();

    /* Print the header comment */
    printf("/************************************\n"
           " * Generated by bf2c                *\n"
           " * https://github.com/8dcc/scratch  *\n"
           " ************************************/\n\n");

    /* We at least need the putchar() and getchar() functions */
    printf("#include <stdio.h>\n\n");

    /* Declare the heap as a global byte array */
    printf("static char heap[%d];\n\n", HEAP_SZ);

    /* Declare the main function, and the local variables */
    printIndented("int main(void) {");

    indentation += INDENT_STEP;
    printIndented("char* p = heap;\n");

    for (int i = 0; input[i] != '\0'; i++) {
        switch (input[i]) {
            case '+':
                printIndented("(*p)++;");
                break;
            case '-':
                printIndented("(*p)--;");
                break;
            case '>':
                printIndented("p++;");
                break;
            case '<':
                printIndented("p--;");
                break;
            case '[':
                putchar('\n');
                printIndented("while (*p != 0) {");
                indentation += INDENT_STEP;
                break;
            case ']':
                indentation -= INDENT_STEP;
                printIndented("}");
                putchar('\n');
                break;
            case '.':
                printIndented("putchar(*p);");
                break;
            case ',':
                printIndented("*p = getchar();");
                break;
            default:
                fprintf(stderr, "Should not reach default case.\n");
                break;
        }
    }

    /* Close the main function */
    printIndented("return 0;");
    indentation -= INDENT_STEP;
    printIndented("}");

    free(input);
    return 0;
}
