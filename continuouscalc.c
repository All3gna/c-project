#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 512

typedef struct {
	const char *cursor;
	double answer;
	int has_answer;
	int error;
} Parser;

static void skip_spaces(Parser *parser)
{
	while (isspace((unsigned char)*parser->cursor)) {
		parser->cursor++;
	}
}

static double parse_expression(Parser *parser);

static double parse_primary(Parser *parser)
{
	skip_spaces(parser);

	if (*parser->cursor == '(') {
		parser->cursor++;
		double value = parse_expression(parser);
		skip_spaces(parser);
		if (*parser->cursor != ')') {
			parser->error = 1;
			return 0;
		}
		parser->cursor++;
		return value;
	}

	if (isalpha((unsigned char)*parser->cursor)) {
		char name[16];
		size_t length = 0;

		while (isalpha((unsigned char)*parser->cursor)) {
			if (length + 1 < sizeof(name)) {
				name[length++] = (char)tolower((unsigned char)*parser->cursor);
			}
			parser->cursor++;
		}
		name[length] = '\0';

		if (strcmp(name, "ans") == 0 && parser->has_answer) {
			return parser->answer;
		}
		parser->error = 1;
		return 0;
	}

	char *end;
	double value = strtod(parser->cursor, &end);
	if (end == parser->cursor || !isfinite(value)) {
		parser->error = 1;
		return 0;
	}
	parser->cursor = end;
	return value;
}

static double parse_unary(Parser *parser)
{
	skip_spaces(parser);
	if (*parser->cursor == '+') {
		parser->cursor++;
		return parse_unary(parser);
	}
	if (*parser->cursor == '-') {
		parser->cursor++;
		return -parse_unary(parser);
	}
	return parse_primary(parser);
}

static double parse_term(Parser *parser)
{
	double value = parse_unary(parser);

	while (!parser->error) {
		skip_spaces(parser);
		char operation = *parser->cursor;
		if (operation != '*' && operation != '/') {
			break;
		}

		parser->cursor++;
		double right = parse_unary(parser);
		if (operation == '/' && right == 0) {
			parser->error = 1;
			return 0;
		}
		value = operation == '*' ? value * right : value / right;
		if (!isfinite(value)) {
			parser->error = 1;
		}
	}
	return value;
}

static double parse_expression(Parser *parser)
{
	double value = parse_term(parser);

	while (!parser->error) {
		skip_spaces(parser);
		char operation = *parser->cursor;
		if (operation != '+' && operation != '-') {
			break;
		}

		parser->cursor++;
		double right = parse_term(parser);
		value = operation == '+' ? value + right : value - right;
		if (!isfinite(value)) {
			parser->error = 1;
		}
	}
	return value;
}

static int is_exit_command(const char *line)
{
	return strcmp(line, "q") == 0 || strcmp(line, "quit") == 0 ||
		   strcmp(line, "exit") == 0;
}

int main(void)
{
	char line[INPUT_SIZE];
	double answer = 0;
	int has_answer = 0;

	printf("Continuous calculator (use ans or start with an operator; q to quit)\n");
	while (1) {
		printf(has_answer ? "[%.10g] > " : "> ", answer);
		fflush(stdout);

		if (fgets(line, sizeof(line), stdin) == NULL) {
			break;
		}

		size_t length = strlen(line);
		if (length > 0 && line[length - 1] == '\n') {
			line[--length] = '\0';
		} else if (!feof(stdin)) {
			int character;
			while ((character = getchar()) != '\n' && character != EOF) {
			}
			printf("Input is too long.\n");
			continue;
		}

		char *expression = line;
		while (isspace((unsigned char)*expression)) {
			expression++;
		}
		char *end = expression + strlen(expression);
		while (end > expression && isspace((unsigned char)end[-1])) {
			*--end = '\0';
		}

		if (*expression == '\0') {
			continue;
		}
		if (is_exit_command(expression)) {
			break;
		}

		char continued[INPUT_SIZE + 8];
		if (has_answer && (*expression == '+' || *expression == '-' ||
						   *expression == '*' || *expression == '/')) {
			snprintf(continued, sizeof(continued), "ans %s", expression);
			expression = continued;
		}

		Parser parser = {expression, answer, has_answer, 0};
		double result = parse_expression(&parser);
		skip_spaces(&parser);
		if (parser.error || *parser.cursor != '\0') {
			printf("Invalid expression or division by zero.\n");
			continue;
		}

		answer = result;
		has_answer = 1;
		printf("= %.10g\n", answer);
	}

	return 0;
}
