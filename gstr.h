#ifndef GSTRING_H
#define GSTRING_H

#ifndef GSTRING_INITIAL_CAPACITY
#define GSTRING_INITIAL_CAPACITY 16
#endif //!GSTRING_INITIAL_CAPACITY

#include <stddef.h>

typedef struct {
	char *content;
	size_t len;
	size_t cap;
} gstr;

typedef struct {
	char *content;
	size_t len;
} gsv;


gstr gstr_new(void);
void gstr_del(gstr *s);
int gstr_append(gstr *s, char c);
gstr gstr_from_lit(const char *array, size_t len);

#define gstr_from_static(cstr) gstr_from_lit(cstr, sizeof(cstr) - 1)

#ifdef GSTRING_IMPL
#include <stdlib.h>
#include <string.h>

gstr gstr_new(void) {
	char *content = malloc(sizeof(char) * GSTRING_INITIAL_CAPACITY);
	if (content == NULL) return (gstr) {0};
	gstr str = {
		.content = content,
		.len = 0,
		.cap = GSTRING_INITIAL_CAPACITY,
	};
	return str;
}

void gstr_del(gstr *s) {
	free(s -> content);
	s -> len = 0;
	s -> cap = 0;
}

gstr gstr_from_lit(const char *array, size_t len) {
	gstr str = {
		.content = NULL,
		.len = 0,
		.cap = 0,
	};

	str.content = malloc(sizeof(char) * len);
	if (str.content == NULL) {
		return str;
	}
	strncpy(str.content, array, len);
	str.len = len;
	str.cap = len;
	return str;
}

int gstr_grow(gstr *s) {
	void *res = realloc(s -> content, sizeof(char) * s -> cap * 2);
	if (res == NULL) {
		return -1;
	} else {
		s -> content = res;
		s -> cap *= 2;
		return 0;
	}
}

int gstr_append(gstr *s, char c) {
	if (s -> len == s -> cap) {
		if (gstr_grow(s) < 0) return -1;
	}
	s -> content[s -> len++] = c;
	return 0;
}

#endif // !GSTRING_IMPL
#endif // !GSTRING_H
