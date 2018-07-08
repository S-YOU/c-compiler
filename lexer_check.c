#include <stdio.h>
#include "lexer.h"

void read_all_tokens(const char* str)
{
	struct Token tok;
	do{
		tok = get_token(&str);
		print_token(tok);
		printf("\n");
		if(tok.kind == END) {
			break;
		}
	}while(1);
}

int main()
{
	char str[1000];
	/* const char* str = "123+456-789"; */
	scanf("%s", str); /* VULNERABLE!!! */
	read_all_tokens(str);
}