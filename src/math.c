#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "math.h"

typedef struct {
	const char *cur;
	const char *end;
	Buffer *b;
} MathParser;

typedef enum {
	ATOM_NONE,
	ATOM_GROUP,
	ATOM_COMMAND,
	ATOM_NUMBER,
	ATOM_IDENT,
	ATOM_OPERATOR
} AtomKind;

typedef struct {
	AtomKind kind;
	const char *start;
	const char *end;
} AtomSpan;

static bool is_space_uc(unsigned char c) {
	return isspace(c) != 0;
}

static bool is_alpha_uc(unsigned char c) {
	return isalpha(c) != 0;
}

static bool is_digit_uc(unsigned char c) {
	return isdigit(c) != 0;
}

static void skip_ws(MathParser *p) {
	while (p->cur < p->end && is_space_uc((unsigned char)*p->cur)) p->cur++;
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

static void parse_expr(MathParser *p);

static bool match_cmd(MathParser *p, const char *s) {
	size_t len = strlen(s);
	if (p->cur + len <= p->end && strncmp(p->cur, s, len) == 0) {
		if (is_alpha_uc((unsigned char)*s)) {
			if (p->cur + len < p->end && is_alpha_uc((unsigned char)p->cur[len])) return false;
		}
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
	{"ne", "<mo>&ne;</mo>"},
	{"neq", "<mo>&ne;</mo>"},
	{"approx", "<mo>&asymp;</mo>"},
	{"to", "<mo>&rarr;</mo>"},
	{"implies", "<mo>&Implies;</mo>"},
	{"vdash", "<mo>&vdash;</mo>"},
	{"rightarrow", "<mo>&rarr;</mo>"},
	{"leftarrow", "<mo>&larr;</mo>"},
	{"Rightarrow", "<mo>&rArr;</mo>"},
	{"Leftarrow", "<mo>&lArr;</mo>"},
	{"mid", "<mo>&mid;</mo>"},
	{"sim", "<mo>&sim;</mo>"},
	{"in", "<mo>&in;</mo>"},
	{"notin", "<mo>&notin;</mo>"},
	{"subseteq", "<mo>&sube;</mo>"},
	{"subset", "<mo>&subset;</mo>"},
	{"cup", "<mo>&cup;</mo>"},
	{"cap", "<mo>&cap;</mo>"},
	{"cdot", "<mo>&sdot;</mo>"},
	{"forall", "<mo>&forall;</mo>"},
	{"exists", "<mo>&exists;</mo>"},
	{"dots", "<mo>&hellip;</mo>"},
	{"cdots", "<mo>&ctdot;</mo>"},
	{"ldots", "<mo>&hellip;</mo>"},
	{"exp", "<mi>exp</mi>"},
	{"log", "<mi>log</mi>"},
	{"ln", "<mi>ln</mi>"},
	{"top", "<mo>&top;</mo>"},
	{"bot", "<mo>&bot;</mo>"},
	{"perp", "<mo>&perp;</mo>"},
	{"theta", "<mi>&theta;</mi>"},
	{"sigma", "<mi>&sigma;</mi>"},
	{"rho", "<mi>&rho;</mi>"},
	{"tau", "<mi>&tau;</mi>"},
	{"alpha", "<mi>&alpha;</mi>"},
	{"downarrow", "<mo>&darr;</mo>"},
	{"|", "<mo>&Vert;</mo>"},
	{"{", "<mo>{</mo>"},
	{"}", "<mo>}</mo>"}
};

#define CMD_TABLE_LEN (sizeof(CMD_TABLE) / sizeof(CMD_TABLE[0]))

static bool parse_atom_span(MathParser *p, AtomSpan *atom);
static void emit_atom_span(Buffer *b, const AtomSpan *atom);
static void emit_styled_atom(MathParser *p, const char *mathvariant);
static bool is_plain_identifier(const char *start, const char *end);

static void handle_frac(MathParser *p) {
	buf_append(p->b, "<mfrac>");
	AtomSpan num, den;

	if (parse_atom_span(p, &num))
		emit_atom_span(p->b, &num);
	if (parse_atom_span(p, &den))
		emit_atom_span(p->b, &den);
	buf_append(p->b, "</mfrac>");
}

static void handle_sqrt(MathParser *p) {
	buf_append(p->b, "<msqrt>");
	AtomSpan inner;

	if (parse_atom_span(p, &inner))
		emit_atom_span(p->b, &inner);
	buf_append(p->b, "</msqrt>");
}

static void emit_styled_atom(MathParser *p, const char *mathvariant) {
	AtomSpan atom;
	const char *text_start;
	size_t text_len;

	if (!parse_atom_span(p, &atom))
		return;

	if (atom.kind == ATOM_GROUP && atom.end > atom.start + 1) {
		text_start = atom.start + 1;
		text_len = (size_t)((atom.end - 1) - text_start);
	} else {
		text_start = atom.start;
		text_len = (size_t)(atom.end - atom.start);
	}

	if (strcmp(mathvariant, "normal") == 0 && text_len > 1 && is_plain_identifier(text_start, text_start + text_len)) {
		buf_append(p->b, "<mtext>");
		buf_escape(p->b, text_start, text_len);
		buf_append(p->b, "</mtext>");
		return;
	}

	buf_append(p->b, "<mi mathvariant=\"");
	buf_append(p->b, mathvariant);
	buf_append(p->b, "\">");
	buf_escape(p->b, text_start, text_len);
	buf_append(p->b, "</mi>");
}

static bool is_plain_identifier(const char *start, const char *end) {
	for (const unsigned char *p = (const unsigned char *)start; p < (const unsigned char *)end; p++) {
		if (!is_alpha_uc(*p))
			return false;
	}
	return start < end;
}

static void parse_command(MathParser *p) {
	if (p->cur < p->end && strchr(", !;:", *p->cur)) {
		char c = *p->cur++;
		const char *width = (c == '!') ? "-0.167em" :
		                    (c == ';') ? "0.278em" :
		                    (c == ':') ? "0.222em" : "0.167em";
		buf_append(p->b, "<mspace width=\"");
		buf_append(p->b, width);
		buf_append(p->b, "\"></mspace>");
		return;
	}

	if (match_cmd(p, "frac")) { handle_frac(p); return; }
	if (match_cmd(p, "sqrt")) { handle_sqrt(p); return; }

	static const struct { const char *name; const char *variant; } styled[] = {
		{"mathrm", "normal"}, {"text", "normal"}, {"operatorname", "normal"},
		{"mathbb", "double-struck"}, {"mathcal", "script"}, {"mathbf", "bold"},
	};

	for (size_t i = 0; i < sizeof(styled)/sizeof(styled[0]); i++) {
		if (match_cmd(p, styled[i].name)) {
			emit_styled_atom(p, styled[i].variant);
			return;
		}
	}

	if (match_cmd(p, "left") || match_cmd(p, "right")) return;

	for (size_t i = 0; i < CMD_TABLE_LEN; i++) {
		if (match_cmd(p, CMD_TABLE[i].name)) {
			buf_append(p->b, CMD_TABLE[i].output);
			return;
		}
	}

	buf_append(p->b, "<mi>?</mi>");
}

static bool parse_atom_span(MathParser *p, AtomSpan *atom) {
	skip_ws(p);
	if (p->cur >= p->end) {
		atom->kind = ATOM_NONE;
		atom->start = atom->end = p->cur;
		return false;
	}

	atom->start = p->cur;
	char c = *p->cur;

	if (c == '{') {
		int depth = 1;
		p->cur++;
		while (p->cur < p->end && depth > 0) {
			if (*p->cur == '{') depth++;
			else if (*p->cur == '}') depth--;
			p->cur++;
		}
		atom->kind = ATOM_GROUP;
		atom->end = p->cur;
		return true;
	}

	if (c == '\\') {
		p->cur++;
		if (p->cur < p->end && !is_alpha_uc((unsigned char)*p->cur))
			p->cur++;
		while (p->cur < p->end && is_alpha_uc((unsigned char)*p->cur))
			p->cur++;
		atom->kind = ATOM_COMMAND;
		atom->end = p->cur;
		return true;
	}

	if (is_digit_uc((unsigned char)c)) {
		while (p->cur < p->end && (is_digit_uc((unsigned char)*p->cur) || *p->cur == '.'))
			p->cur++;
		atom->kind = ATOM_NUMBER;
		atom->end = p->cur;
		return true;
	}

	if (is_alpha_uc((unsigned char)c)) {
		while (p->cur < p->end && is_alpha_uc((unsigned char)*p->cur))
			p->cur++;
		atom->kind = ATOM_IDENT;
		atom->end = p->cur;
		return true;
	}

	if (strchr("+-*/()[]=<>!|,:", c)) {
		p->cur++;
		atom->kind = ATOM_OPERATOR;
		atom->end = p->cur;
		return true;
	}

	p->cur++;
	atom->kind = ATOM_NONE;
	atom->end = p->cur;
	return false;
}

static void emit_atom_span(Buffer *b, const AtomSpan *atom) {
	switch (atom->kind) {
	case ATOM_GROUP:
		if (atom->end > atom->start + 1) {
			const char *inner_end = atom->end;
			if (inner_end > atom->start && *(inner_end - 1) == '}')
				inner_end--;
			MathParser inner = {
				.cur = atom->start + 1,
				.end = inner_end,
				.b = b
			};
			buf_append(b, "<mrow>");
			parse_expr(&inner);
			buf_append(b, "</mrow>");
		}
		break;
	case ATOM_COMMAND: {
		MathParser nested = { .cur = atom->start + 1, .end = atom->end, .b = b };
		parse_command(&nested);
		break;
	}
	case ATOM_NUMBER:
		buf_append(b, "<mn>");
		buf_append_len(b, atom->start, (size_t)(atom->end - atom->start));
		buf_append(b, "</mn>");
		break;
	case ATOM_IDENT:
		buf_append(b, "<mi>");
		buf_escape(b, atom->start, (size_t)(atom->end - atom->start));
		buf_append(b, "</mi>");
		break;
	case ATOM_OPERATOR:
		buf_append(b, "<mo>");
		buf_escape(b, atom->start, (size_t)(atom->end - atom->start));
		buf_append(b, "</mo>");
		break;
	case ATOM_NONE:
	default:
		break;
	}
}

static void emit_scripts(Buffer *b, const AtomSpan *base, const AtomSpan *sub, bool has_sub, const AtomSpan *sup, bool has_sup) {
	if (has_sub && has_sup) {
		buf_append(b, "<msubsup>");
		emit_atom_span(b, base);
		emit_atom_span(b, sub);
		emit_atom_span(b, sup);
		buf_append(b, "</msubsup>");
	} else if (has_sub) {
		buf_append(b, "<msub>");
		emit_atom_span(b, base);
		emit_atom_span(b, sub);
		buf_append(b, "</msub>");
	} else if (has_sup) {
		buf_append(b, "<msup>");
		emit_atom_span(b, base);
		emit_atom_span(b, sup);
		buf_append(b, "</msup>");
	} else {
		emit_atom_span(b, base);
	}
}

static void parse_expr(MathParser *p) {
	while (p->cur < p->end) {
		skip_ws(p);
		if (p->cur >= p->end) break;

		AtomSpan base, sub = {0}, sup = {0};
		bool has_sub = false;
		bool has_sup = false;

		if (!parse_atom_span(p, &base))
			continue;

		for (int i = 0; i < 2; i++) {
			skip_ws(p);
			if (p->cur >= p->end || (*p->cur != '^' && *p->cur != '_'))
				break;

			char op = *p->cur++;
			AtomSpan script;
			if (!parse_atom_span(p, &script))
				break;

			if (op == '^' && !has_sup) {
				sup = script;
				has_sup = true;
			} else if (op == '_' && !has_sub) {
				sub = script;
				has_sub = true;
			} else {
				emit_scripts(p->b, &base, &sub, has_sub, &sup, has_sup);
				base = script;
				has_sub = false;
				has_sup = false;
			}
		}
		emit_scripts(p->b, &base, &sub, has_sub, &sup, has_sup);
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
