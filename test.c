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

int main(void) {
	test_constructor();
	test_static_constructor();
	test_append();
	return 0;
}
