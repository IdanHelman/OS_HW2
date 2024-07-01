#include "hw2_test.h"
#include <cstdio>
#include <iostream>
#include <cassert>

using namespace std;

// tests for get_weight and set_weight
void test_init() {
	//cout << "test init weight is " << get_weight() <<endl;
	assert(get_weight() == 0);
}

void test_set_get() {
	set_weight(42);
	assert(get_weight() == 42);
}

long test_illegal_weight() {
    assert(set_weight(-1) == -EINVAL);
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

// tests for get_path_sum
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

void test_child_process() {
    set_weight(5);
    pid_t child_pid = fork();
    if (child_pid == 0) {
        sleep(1);
        exit(0);
    } else if (child_pid > 0) {
        long result = get_path_sum(child_pid);
        assert(result == 10);
    } else {
        perror("fork failed");
    }
}

void test_illegal_pid() {
    long result = get_path_sum(-1);
    assert(result == -ECHILD);
}

// tests for get_heaviest_sibling
void test_heaviest_sibling_fork() {
    set_weight(1);
    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            set_weight(i);
            cout << get_heaviest_sibling() << " " << getpid() << endl;
            sleep(1);
            exit(0);
        }
    }
}


int main() {
	TEST(test_init);
    int x = get_weight();
	assert(x == 0);
    x = set_weight(5);
	assert(x == 0);
    x = get_weight();
	assert(x == 5);
    cout << "===== SUCCESS =====" << endl;
	TEST(test_set_get);
//    TEST(test_illegal_weight);
	TEST(test_set_get_fork);
    TEST(test_child_process);
   // TEST(test_get_path_sum);
  //  TEST(test_illegal_pid);
 //   TEST(test_heaviest_sibling_fork);

    return 0;
}

