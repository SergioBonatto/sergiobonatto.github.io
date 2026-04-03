#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "math.h"

typedef struct {
	const char *cur;
	const char *end;
	Buffer *b;
} MathParser;

static void skip_ws(MathParser *p) {
	while (p->cur < p->end && isspace(*p->cur)) p->cur++;
}

static bool peek(MathParser *p, char c) {
	skip_ws(p);
	return p->cur < p->end && *p->cur == c;
}

static bool match(MathParser *p, char c) {
	if (peek(p, c)) {
		p->cur++;
		return true;
	}
	return false;
}

static bool match_str(MathParser *p, const char *s) {
	size_t len = strlen(s);
	if (p->cur + len <= p->end && strncmp(p->cur, s, len) == 0) {
		p->cur += len;
		return true;
	}
	return false;
}

static void parse_expr(MathParser *p);
static void parse_atom(MathParser *p);

static bool match_cmd(MathParser *p, const char *s) {
	size_t len = strlen(s);
	if (p->cur + len <= p->end && strncmp(p->cur, s, len) == 0) {
		if (p->cur + len < p->end && isalpha(p->cur[len])) return false;
		p->cur += len;
		return true;
	}
	return false;
}

typedef struct {
	const char *name;
	const char *output;
} CmdEntry;

static const CmdEntry CMD_TABLE[] = {
	{"alpha", "<mi>&alpha;</mi>"},
	{"beta", "<mi>&beta;</mi>"},
	{"gamma", "<mi>&gamma;</mi>"},
	{"delta", "<mi>&delta;</mi>"},
	{"epsilon", "<mi>&epsilon;</mi>"},
	{"zeta", "<mi>&zeta;</mi>"},
	{"eta", "<mi>&eta;</mi>"},
	{"theta", "<mi>&theta;</mi>"},
	{"iota", "<mi>&iota;</mi>"},
	{"kappa", "<mi>&kappa;</mi>"},
	{"lambda", "<mi>&lambda;</mi>"},
	{"mu", "<mi>&mu;</mi>"},
	{"nu", "<mi>&nu;</mi>"},
	{"xi", "<mi>&xi;</mi>"},
	{"pi", "<mi>&pi;</mi>"},
	{"rho", "<mi>&rho;</mi>"},
	{"sigma", "<mi>&sigma;</mi>"},
	{"tau", "<mi>&tau;</mi>"},
	{"upsilon", "<mi>&upsilon;</mi>"},
	{"phi", "<mi>&phi;</mi>"},
	{"chi", "<mi>&chi;</mi>"},
	{"psi", "<mi>&psi;</mi>"},
	{"omega", "<mi>&omega;</mi>"},
	{"Gamma", "<mi>&Gamma;</mi>"},
	{"Delta", "<mi>&Delta;</mi>"},
	{"Theta", "<mi>&Theta;</mi>"},
	{"Lambda", "<mi>&Lambda;</mi>"},
	{"Xi", "<mi>&Xi;</mi>"},
	{"Pi", "<mi>&Pi;</mi>"},
	{"Sigma", "<mi>&Sigma;</mi>"},
	{"Phi", "<mi>&Phi;</mi>"},
	{"Psi", "<mi>&Psi;</mi>"},
	{"Omega", "<mi>&Omega;</mi>"},
	{"sum", "<mo>&sum;</mo>"},
	{"prod", "<mo>&prod;</mo>"},
	{"int", "<mo>&int;</mo>"},
	{"infty", "<mi>&infin;</mi>"},
	{"times", "<mo>&times;</mo>"},
	{"div", "<mo>&divide;</mo>"},
	{"pm", "<mo>&plusmn;</mo>"},
	{"mp", "<mo>&mp;</mo>"},
	{"partial", "<mo>&part;</mo>"},
	{"nabla", "<mo>&nabla;</mo>"},
	{"le", "<mo>&le;</mo>"},
	{"ge", "<mo>&ge;</mo>"},
	{"neq", "<mo>&ne;</mo>"},
	{"approx", "<mo>&asymp;</mo>"},
	{"to", "<mo>&rarr;</mo>"},
	{"rightarrow", "<mo>&rarr;</mo>"},
	{"leftarrow", "<mo>&larr;</mo>"},
	{"Rightarrow", "<mo>&rArr;</mo>"},
	{"Leftarrow", "<mo>&lArr;</mo>"}
};

#define CMD_TABLE_LEN (sizeof(CMD_TABLE) / sizeof(CMD_TABLE[0]))

static void handle_frac(MathParser *p) {
	buf_append(p->b, "<mfrac>");
	parse_atom(p);
	parse_atom(p);
	buf_append(p->b, "</mfrac>");
}

static void handle_sqrt(MathParser *p) {
	buf_append(p->b, "<msqrt>");
	parse_atom(p);
	buf_append(p->b, "</msqrt>");
}

static void parse_command(MathParser *p) {
	if (match_cmd(p, "frac")) {
		handle_frac(p);
		return;
	}
	if (match_cmd(p, "sqrt")) {
		handle_sqrt(p);
		return;
	}

	for (size_t i = 0; i < CMD_TABLE_LEN; i++) {
		if (match_cmd(p, CMD_TABLE[i].name)) {
			buf_append(p->b, CMD_TABLE[i].output);
			return;
		}
	}

	// Unknown command, skip alpha characters
	while (p->cur < p->end && isalpha(*p->cur)) p->cur++;
}

static void parse_atom(MathParser *p) {
	skip_ws(p);
	if (p->cur >= p->end) return;

	char c = *p->cur;

	if (c == '{') {
		p->cur++;
		parse_expr(p);
		if (match(p, '}')) {} // consumed
		return;
	}

	if (c == '\\') {
		p->cur++;
		parse_command(p);
		return;
	}

	if (isdigit(c)) {
		buf_append(p->b, "<mn>");
		while (p->cur < p->end && (isdigit(*p->cur) || *p->cur == '.')) {
			buf_append_len(p->b, p->cur++, 1);
		}
		buf_append(p->b, "</mn>");
	} else if (isalpha(c)) {
		buf_append(p->b, "<mi>");
		buf_append_len(p->b, p->cur++, 1);
		buf_append(p->b, "</mi>");
	} else if (strchr("+-*/()[]=<>!|,", c)) {
		buf_append(p->b, "<mo>");
		buf_escape(p->b, p->cur++, 1);
		buf_append(p->b, "</mo>");
	} else {
		p->cur++; // Skip unknown
	}
}

static void parse_expr(MathParser *p) {
	while (p->cur < p->end && *p->cur != '}') {
		skip_ws(p);
		if (p->cur >= p->end || *p->cur == '}') break;

		// Save current buffer length
		size_t start_len = p->b->len;
		parse_atom(p);
		size_t atom_len = p->b->len - start_len;

		skip_ws(p);
		if (p->cur < p->end && (*p->cur == '^' || *p->cur == '_')) {
			char op = *p->cur++;
			char atom_tmp[512];
			if (atom_len < sizeof(atom_tmp)) {
				memcpy(atom_tmp, p->b->data + start_len, atom_len);
				atom_tmp[atom_len] = '\0';
				p->b->len = start_len;

				if (op == '^') {
					buf_append(p->b, "<msup>");
					buf_append(p->b, atom_tmp);
					parse_atom(p);
					buf_append(p->b, "</msup>");
				} else {
					buf_append(p->b, "<msub>");
					buf_append(p->b, atom_tmp);
					parse_atom(p);
					buf_append(p->b, "</msub>");
				}
			}
		}
	}
}

void math_to_mathml(Buffer *b, const char *latex, size_t len, bool display_mode) {
	MathParser p = { .cur = latex, .end = latex + len, .b = b };
	
	if (display_mode) {
		buf_append(b, "<math display=\"block\" xmlns=\"http://www.w3.org/1998/Math/MathML\">");
	} else {
		buf_append(b, "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">");
	}
	
	parse_expr(&p);
	
	buf_append(b, "</math>");
}
