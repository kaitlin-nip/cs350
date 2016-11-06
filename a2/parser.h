#ifndef PARSER_H
#define PARSER_H

#include "cmd_node.h"

int isoperator(char c);
void skip_whitespace(char** cmdptr);
char* parse_token(char** cmdptr);

int parse_command(char** cmdptr, struct cmd_node* cmd_heads);
struct cmd_node* parse_line(char** line);

#endif