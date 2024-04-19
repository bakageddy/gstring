#ifndef GSTRING_H
#define GSTRING_H

#ifndef GSTRING_INITIAL_CAPACITY
#define GSTRING_INITIAL_CAPACITY 16
#endif //!GSTRING_INITIAL_CAPACITY

#ifndef GSTRING_MINIMUM_CAPACITY
#define GSTRING_MINIMUM_CAPACITY 8
#endif // !GSTRING_MINIMUM_CAPACITY

#include <stddef.h>

typedef enum {
	SOME,
	NONE
} option_tag;

typedef struct {
	option_tag t;
	char c;
} opt_char;

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
void gstr_clear(gstr *s);
void gstr_del(gstr *s);
int gstr_append(gstr *s, char c);
opt_char gstr_pop(gstr *s);
int gstr_remove(gstr *s, size_t idx);
gstr gstr_from_lit(const char *array, size_t len);

#define gstr_from_static(cstr) gstr_from_lit(cstr, sizeof(cstr) - 1)

#define GSTRING_IMPL // For my syntax highlighting
#ifdef GSTRING_IMPL
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

gstr gstr_new(void) {
	char *content = (char *) malloc(sizeof(char) * GSTRING_INITIAL_CAPACITY);
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

	str.content = (char *) malloc(sizeof(char) * len);
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
		s -> content = (char *) res;
		s -> cap *= 2;
		return 0;
	}
}

int gstr_shrink(gstr *s) {
	size_t new_size = (size_t) (sizeof(char) * s -> cap * 0.5);
	void *res = realloc(s -> content, new_size);
	if (res == NULL) {
		return -1;
	} else {
		s -> content = (char *) res;
		// BECAREFUL, you are threading on dangerous grounds!
		s -> cap = new_size;
		return 0;
	}
}

int gstr_insert(gstr *s, size_t idx, char c) {
	if (idx > s -> len) {
		return -1;
	} else if (idx == s -> len) {
		return gstr_append(s, c);
	}

	if (s -> len == s -> cap) {
		if (gstr_grow(s) < 0) return -1;
	}
	memmove(s -> content + idx + 1, s -> content + idx, s -> len - idx);
	s -> content[idx] = c;
	s -> len += 1;
	return 0;
}

int gstr_append(gstr *s, char c) {
	if (s -> len == s -> cap) {
		if (gstr_grow(s) < 0) return -1;
	}
	s -> content[s -> len++] = c;
	return 0;
}

bool gstr_should_shrink(const size_t len, const size_t cap) {
	return (len <= (size_t) (cap / 2)) && (cap / 2 > GSTRING_MINIMUM_CAPACITY);
}

int gstr_remove(gstr *s, size_t idx) {
	if (idx >= s -> len) {
		return -1;
	}
	memmove(s -> content + idx, s -> content + idx + 1, s -> len - idx);

	s -> len -= 1;
	if (gstr_should_shrink(s -> len, s -> cap)) {
		if (gstr_shrink(s) < 0) {
			return -1;
		}
	}
	return 0;
}

[[nodiscard]] opt_char gstr_pop(gstr *s) {
	if (s -> len == 0) {
		return (opt_char) {
			.t = NONE,
			.c = 0,
		};
	}
	char popped = s -> content[s -> len - 1];
	s -> len -= 1;

	// Only shrink when you need to
	if (gstr_should_shrink(s -> len, s -> cap)) {
		int res = gstr_shrink(s);
		if (res < 0) return (opt_char) {
			.t = NONE,
			.c = 0,
		};
	}

	return (opt_char) {
		.t = SOME,
		.c = popped,
	};
}

void gstr_clear(gstr *s) {
	// Why not?
	memset(s -> content, '\0', s -> len);
	s -> len = 0;
}

#endif // !GSTRING_IMPL
#endif // !GSTRING_H
