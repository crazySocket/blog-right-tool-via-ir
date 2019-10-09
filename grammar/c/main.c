#include <ctype.h>
#include <stdio.h>

enum symbol
{
    SYMBOL_START,
    SYMBOL_MESSAGE,
    SYMBOL_LINE,
    SYMBOL_KEY,
    SYMBOL_VALUE,
    SYMBOL_TEXT,
    SYMBOL_CHARACTER,
    SYMBOL_CHARACTER_MORE,
    SYMBOL_CHARACTER_COLON,
    SYMBOL_CHARACTER_NL,
};

const char * symbol_to_string(enum symbol symbol)
{
#define SYMBOL_CASE(x) case x: return #x;
    switch (symbol)
    {
        SYMBOL_CASE(SYMBOL_START)
        SYMBOL_CASE(SYMBOL_MESSAGE)
        SYMBOL_CASE(SYMBOL_LINE)
        SYMBOL_CASE(SYMBOL_KEY)
        SYMBOL_CASE(SYMBOL_VALUE)
        SYMBOL_CASE(SYMBOL_TEXT)
        SYMBOL_CASE(SYMBOL_CHARACTER)
        SYMBOL_CASE(SYMBOL_CHARACTER_MORE)
        SYMBOL_CASE(SYMBOL_CHARACTER_COLON)
        SYMBOL_CASE(SYMBOL_CHARACTER_NL)
    }
#undef SYMBOL_CASE
}

void print_stack(enum symbol stack[], int stack_top)
{
    printf("stack\n{\n");
    for(int i = stack_top; i > -1; i--)
    {
        printf("\t%s\n", symbol_to_string(stack[i]));
    }
    printf("}\n");
}

int main()
{
    const char input[] = "text:hello world\n";
    const unsigned input_size = sizeof(input)/sizeof(input[0]) - 1;  // minus \0
    const char * input_it = input;
    const char * input_end = input + input_size;
    enum symbol stack[input_size];
    int stack_top = 0;
    stack[stack_top] = SYMBOL_START;

    char key[input_size];
    unsigned key_it = 0;
    char value[input_size];
    unsigned value_it = 0;

    char * field = NULL;
    unsigned * field_it = NULL;

    while(stack_top > -1 && input_it != input_end)
    {
        print_stack(stack, stack_top);

        const enum symbol symbol = stack[stack_top--];
        const char next_char = *input_it;

        printf("next symbol '%s'\n", symbol_to_string(symbol));
        printf("next char: '%x'\n", next_char);

        switch (symbol) {
            case SYMBOL_START:
                stack[++stack_top] = SYMBOL_MESSAGE;
            break;
            case SYMBOL_MESSAGE:
                stack[++stack_top] = SYMBOL_CHARACTER_NL;
                stack[++stack_top] = SYMBOL_LINE;
            break;
            case SYMBOL_LINE:
                stack[++stack_top] = SYMBOL_VALUE;
                stack[++stack_top] = SYMBOL_CHARACTER_COLON;
                stack[++stack_top] = SYMBOL_KEY;
            break;
            case SYMBOL_KEY:
                field = key;
                field_it = &key_it;
                stack[++stack_top] = SYMBOL_TEXT;
            break;
            case SYMBOL_VALUE:
                field = value;
                field_it = &value_it;
                stack[++stack_top] = SYMBOL_TEXT;
            break;
            case SYMBOL_TEXT:
                stack[++stack_top] = SYMBOL_CHARACTER_MORE;
                stack[++stack_top] = SYMBOL_CHARACTER;
            break;
            case SYMBOL_CHARACTER:
                if(isalnum(next_char) || next_char == ' ')
                {
                    input_it++;
                    field[(*field_it)++] = next_char;
                }
            break;
            case SYMBOL_CHARACTER_MORE:
                if(isalnum(next_char) || next_char == ' ')
                {
                    input_it++;
                    stack[++stack_top] = SYMBOL_CHARACTER_MORE;
                    field[(*field_it)++] = next_char;
                }
            break;
            case SYMBOL_CHARACTER_COLON:
                if(next_char == ':')
                {
                    input_it++;
                }
                else
                {
                    goto exit;
                }
            break;
            case SYMBOL_CHARACTER_NL:
                if(next_char == '\n')
                {
                    input_it++;
                }
                else
                {
                    goto exit;
                }
            break;
        }
        getchar();
    }

    exit:
    if(stack_top == -1 && input_it == input_end)
    {
        printf("string matches\n");
        key[key_it] = '\0';
        value[value_it] = '\0';
        printf("key: '%s', value: '%s'\n", key, value);
    }
    else
    {
        printf("string does not match\n");
        print_stack(stack, stack_top);
    }
}
