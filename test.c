#include <stdio.h>
#include <assert.h>
#include <string.h>

#define GSTRING_IMPL
#include "gstr.h"

int test_constructor(void) {
	gstr x = gstr_new();
	assert(x.len == 0);
	assert(x.cap == GSTRING_INITIAL_CAPACITY);
	printf("%.*s\n", (int) x.len, x.content);
	gstr_del(&x);
	assert(x.len == 0);
	assert(x.cap == 0);
	return 1;
}

int test_static_constructor(void) {
	gstr s = gstr_from_static("Dinesh Kumar");
	printf("%.*s\n", (int) s.len, s.content);
	assert(strncmp(s.content, "Dinesh Kumar", s.len) == 0);
	gstr_del(&s);
	return 1;
}

int test_append(void) {
	gstr s = gstr_new();
	for (int i = 0; i < GSTRING_INITIAL_CAPACITY; i++) {
		gstr_append(&s, 's');
	}
	printf("%.*s\n", (int) s.len, s.content);
	// This should just work
	gstr_append(&s, 'x');

	printf("%.*s\n", (int) s.len, s.content);
	assert(s.cap == GSTRING_INITIAL_CAPACITY * 2);
	assert(s.len == 17);
	gstr_del(&s);
	return 1;
}

int test_pop(void) {
	gstr s = gstr_new();
	for (int i = 0; i < GSTRING_INITIAL_CAPACITY; i++) {
		gstr_append(&s, 'x');
	}
	printf("%.*s\n", (int) s.len, s.content);
	for (int i = 0; i < GSTRING_INITIAL_CAPACITY; i++) {
		opt_char res = gstr_pop(&s);
		assert(res.t == SOME);
	}
	opt_char res = gstr_pop(&s);
	assert(res.t == NONE);

	gstr name = gstr_from_static("dinesh kumar");
	res = gstr_pop(&name);
	assert(res.t == SOME);
	assert(res.c == 'r');

	gstr_del(&name);
	gstr_del(&s);
	return 1;
}

int test_remove() {
	gstr s = gstr_new();
	for (size_t i = 0; i < GSTRING_INITIAL_CAPACITY; i++) {
		gstr_append(&s, 'a' + i);
	}
	printf("%.*s\n", (int) s.len, s.content);
	gstr_remove(&s, 3);
	printf("%.*s\n", (int) s.len, s.content);
	gstr_remove(&s, 0);
	printf("%.*s\n", (int) s.len, s.content);
	gstr_remove(&s, s.len - 1);
	printf("%.*s\n", (int) s.len, s.content);
	gstr_del(&s);
	return 1;
}

int test_insert(void) {

	gstr s = gstr_new();

	for (int i = 0; i < GSTRING_INITIAL_CAPACITY; i++) {
		gstr_append(&s, 'a' + i);
	}

	printf("%.*s\n", (int) s.len, s.content);
	gstr_insert(&s, 0, 'x');
	assert(s.len == 17);
	assert(s.cap == 32);
	assert(s.content[0] == 'x');
	printf("%.*s\n", (int) s.len, s.content);

	gstr_insert(&s, s.len, 'x');
	assert(s.len == 18);
	assert(s.content[s.len - 1] == 'x');
	printf("%.*s\n", (int) s.len, s.content);

	gstr_del(&s);
	return 1;
}

int test_clear(void) {
	gstr s = gstr_new();
	for (size_t i = 0; i < s.cap; i++) {
		gstr_append(&s, 'a' + i);
	}

	gstr_clear(&s);
	assert(s.cap == GSTRING_INITIAL_CAPACITY);
	assert(s.len == 0);
	assert(s.content[0] == '\0');

	gstr_del(&s);
	return 1;
}

int main(void) {
	test_constructor();
	test_static_constructor();
	test_insert();
	test_clear();
	// test_append();
	// test_pop();
	// test_remove();
	return 0;
}
