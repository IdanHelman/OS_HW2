#include "hw2_test.h"
#include <stdio.h>
#include <iostream>
#include <cassert>

using namespace std;

void test_init() {
	//cout << "test init weight is " << get_weight() <<endl;
	assert(get_weight() == 0);
}

void test_set_get() {
	set_weight(42);
	assert(get_weight() == 42);
}

void test_set_get_fork() {
	set_weight(18);

	if (fork() == 0) {
		assert(get_weight() == 18);

		set_weight(19);
		assert(get_weight() == 19);
		exit(0);
	} 

	wait(NULL);
	assert(get_weight() == 18);
}

void test_get_path_sum(){
	int father_pid = getpid();
	set_weight(1);
	pid_t pid = fork();
	if (pid == 0) {
		set_weight(2);
		pid_t pid2 = fork();
		if (pid2 == 0) {
			set_weight(3);
			pid_t pid3 = fork();
			if (pid3 == 0) {
				
				set_weight(4);
				exit(0);
			}
			wait(NULL);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
	wait(NULL);
	assert(get_path_sum(pid) == 6);
}

int main() {
	TEST(test_init);
    int x = get_weight();
	cout << "weight: " << x << endl;
	assert(x == 0);
    x = set_weight(5);
	cout << "set_weight returns: " << x << endl;
	assert(x == 0);
    x = get_weight();
	cout << "new weight: " << x << endl;
	assert(x == 5);
    cout << "===== SUCCESS =====" << endl;

	TEST(test_set_get);
	TEST(test_set_get_fork);
    return 0;
}

