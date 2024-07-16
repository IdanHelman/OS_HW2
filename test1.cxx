#include "hw2_test.h"
#include <cstdio>
#include <iostream>
#include <cassert>
#include <errno.h>
#include <vector>
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
	//perror("error is");
    assert(errno == EINVAL);
}

void test_set_get_fork() {
	set_weight(18);

	if (fork() == 0) {
		assert(get_weight() == 18);

		set_weight(19);
		assert(get_weight() == 19);
		exit(0);
	} 

	while (wait(NULL) != -1);
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
                set_weight(4);
				write(my_pipe[1], charPtr, str.size());
				sleep(5);
			}
			wait(NULL);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
    else{
	    close(my_pipe[1]);
	    char buffer[1000];
	    read(my_pipe[0], buffer, 5);
	    close(my_pipe[0]);
	    int child_pid = atoi(buffer);
	    int path_sum = get_path_sum(child_pid);
	    kill(child_pid, SIGKILL);
	    while(wait(NULL) != -1);
	    //cout << "path_sum is " << path_sum <<endl;
	    assert(path_sum == 10);
        //while (wait(NULL) == -1);
    }
}

void test_child_process() {
    set_weight(5);
    long result = get_path_sum(getpid());
    assert(result == 5);
    pid_t child_pid = fork();
    if (child_pid == 0) {
        sleep(1);
        exit(0);
    } else if (child_pid > 0) {
        result = get_path_sum(child_pid);
        assert(result == 10);
    } else {
        perror("fork failed");
    }
    while (wait(NULL) != -1);
}

void test_get_path_error(){
    get_path_sum(100000);
    assert(errno == ECHILD);

    set_weight(-1);
    assert(errno == EINVAL);

    get_path_sum(0);
    assert(errno == ECHILD);

    set_weight(-1);
    assert(errno == EINVAL);

    pid_t pid = getpid();
    if(fork() == 0){
        get_path_sum(pid);
        assert(errno == ECHILD);
        exit(0);
    }
    while (wait(NULL) != -1);
}

void test_illegal_pid() {
    long result = get_path_sum(-1);
    assert(errno == ECHILD);
	//cout << "test illegal pid" <<endl;
	//cout << "result is " << result << endl;
	//perror("illegal pid error is");
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
        exit(0);
    }
    while (wait(NULL) != -1);
}

void test_heaviest_sibling_fork3(){
    //cout << "my weight is " << get_weight() << endl;
    set_weight(200);
    //cout << "my weight is " << get_weight() << endl;
	vector<int> child_pids;
	char buffer[1000];
	int my_pipe[2];
	pipe(my_pipe);

	//child 1
	pid_t pid = fork();
    if (pid == 0) {
        set_weight(5);
        //cout << "heaviest: " << get_heaviest_sibling() << " pid: " << getpid() << " my weight " << get_weight() << endl;
        assert(get_heaviest_sibling() == getpid());
		write(my_pipe[1], "a", 1);
        sleep(10);
        exit(0);
    }
	read(my_pipe[0], buffer, 2);
	child_pids.push_back(pid);

	//child 2
	pid = fork();
    if (pid == 0) {
        set_weight(10);
        //cout << "heaviest: " << get_heaviest_sibling() << " pid: " << getpid() << " my weight " << get_weight() << endl;
        assert(get_heaviest_sibling() == getpid());
		write(my_pipe[1], "a", 1);
        sleep(10);
        exit(0);
    }
	read(my_pipe[0], buffer, 2);
	child_pids.push_back(pid);
	
	//child 3
	pid = fork();
    if (pid == 0) {
        set_weight(7);
        //cout << "heaviest: " << get_heaviest_sibling() << " pid: " << getpid() << " my weight " << get_weight() << endl;
        assert(get_heaviest_sibling() == child_pids[1]);
		write(my_pipe[1], "a", 1);
        sleep(10);
        exit(0);
    }
	read(my_pipe[0], buffer, 2);
	child_pids.push_back(pid);
	
	//cout << "killing " << child_pids[1] << endl;
	kill(child_pids[1], SIGKILL);
	waitpid(child_pids[1], nullptr, 0);
	child_pids.erase(child_pids.begin() + 1);
	//child 4
	pid = fork();
    if (pid == 0) {
        set_weight(3);
        //cout << "heaviest: " << get_heaviest_sibling() << " pid: " << getpid() << " my weight " << get_weight() << endl;
        assert(get_heaviest_sibling() == child_pids[1]);
		write(my_pipe[1], "a", 1);
        sleep(10);
        exit(0);
    }
	read(my_pipe[0], buffer, 2);
	child_pids.push_back(pid);

	for(int curpid : child_pids){
		kill(SIGKILL, curpid);
	}
    while(wait(NULL) != -1);
}

void test_heaviest_sibling_fork2() {
    set_weight(1);
    int first_sib = fork();
    if (first_sib == 0) {
        set_weight(11);
        sleep(10);
        exit(0);
    }
    else{
        sleep(1);
        for (int i = 1; i < 10; ++i) {
            if (fork() == 0) {
                set_weight(10-i);
                sleep(10);
                exit(0);
            }
        }
        if (fork() == 0) {
            assert(get_heaviest_sibling() == first_sib);
            exit(0);
        }
        else{
            while (wait(NULL) != -1);
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
    TEST(test_illegal_weight);
    TEST(test_set_get_fork);
    TEST(test_illegal_pid);
    TEST(test_get_path_sum);
    TEST(test_child_process);
    TEST(test_get_path_error);
    TEST(test_heaviest_sibling_fork1);
    TEST(test_heaviest_sibling_fork2);
	TEST(test_heaviest_sibling_fork3);
    return 0;
}
