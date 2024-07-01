#ifndef test_hw2_H_
#define test_hw2_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
//my includes
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <wait.h>
#include <errno.h>


#define TEST(test) {							\
	std::cout << #test << ": " << std::flush;	\
	test();										\
	std::cout << "success" << std::endl;		\
}

#define SYS_SET_WEIGHT 334
#define SYS_GET_WEIGHT 335
#define SYS_GET_LEAF_CHILDREN_SUM 336
#define SYS_GET_HEAVIEST_ANCESTOR 337

// System call wrappers
int set_weight(int weight);
int get_weight();
int get_path_sum(pid_t target);
pid_t get_heaviest_sibling(void);

#endif // test_hw2_H_
