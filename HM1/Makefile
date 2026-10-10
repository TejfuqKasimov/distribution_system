task3_orig: pth_ll_rwl/pth_ll_rwl.c
	mkdir -p build
	gcc -o build/task3_orig -Ipth_ll_rwl pth_ll_rwl/my_rand.c pth_ll_rwl/pth_ll_rwl.c -lpthread -O3

task3_custom: pth_ll_rwl/pth_ll_rwl_custom.c
	mkdir -p build
	gcc -o build/task3_custom -Ipth_ll_rwl pth_ll_rwl/my_rand.c pth_ll_rwl/pth_ll_rwl_custom.c -lpthread -O3

task1: task1.c
	mkdir -p build
	gcc -o build/task1 -lpthread -O3 task1.c
	cp generate.py build/

task1_orig: task1_orig.c
	mkdir -p build
	gcc -o build/task1_orig -lpthread -O3 task1_orig.c
	cp generate.py build/