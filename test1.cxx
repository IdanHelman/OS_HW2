#include "hw2_test.h"
#include <cstdio>
#include <iostream>
#include <cassert>
#include <errno.h>
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
	int x = set_weight(-1);
	perror("error is");
    //assert(errno == -EINVAL);
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
	int my_pipe[2];
	pipe(my_pipe);
	set_weight(1);
	pid_t pid = fork();
	if (pid == 0) {
		set_weight(2);
		pid_t pid2 = fork();
		if (pid2 == 0) {
			set_weight(3);
			pid_t pid3 = fork();
			if (pid3 == 0) {
				close(my_pipe[0]);
				std::string str = std::to_string(getpid());
				const char* charPtr = str.c_str();
				write(my_pipe[1], charPtr, str.size());
				set_weight(4);
				while(1);
			}
			wait(NULL);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
	close(my_pipe[1]);
	char buffer[1000];
	read(my_pipe[0], buffer, 5);
	int child_pid = atoi(buffer);
	int path_sum = get_path_sum(child_pid);
	kill(child_pid, SIGKILL);
	wait(NULL);
	assert(path_sum == 10);
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
	cout << "test illegal pid" <<endl;
	cout << "result is " << result << endl;
	perror("illegal pid error is");
}

// tests for get_heaviest_sibling
void test_heaviest_sibling_fork1() {
    set_weight(1);
    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            sleep(10);
            exit(0);
        }
    }
    if (fork() == 0) {
        set_weight(10);
        assert(get_heaviest_sibling() == getpid());
    }
    while (wait(NULL) == -1);
}

void test_heaviest_sibling_fork2() {
    set_weight(1);
    int first_sib = fork();
    if (first_sib == 0) {
        set_weight(11);
        sleep(10);
        exit(0);
    }
    for (int i = 1; i < 10; ++i) {
        if (fork() == 0) {
            set_weight(10-i);
            sleep(10);
            exit(0);
        }
    }
    if (fork() == 0) {
        assert(get_heaviest_sibling() == first_sib);
    }
    while (wait(NULL) == -1);
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
	TEST(test_set_get_fork);
    TEST(test_child_process);
    TEST(test_get_path_sum);
    TEST(test_heaviest_sibling_fork1);
    TEST(test_heaviest_sibling_fork2);
    return 0;
}

