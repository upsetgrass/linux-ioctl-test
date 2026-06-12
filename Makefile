MODULE_DIR := module
TEST_DIR := test_c

.PHONY: all module test-build test load unload clean

all: module test-build

module:
	$(MAKE) -C $(MODULE_DIR)

test-build:
	$(MAKE) -C $(TEST_DIR)

load: module
	sudo insmod $(MODULE_DIR)/append_hello_mod.ko

unload:
	sudo rmmod append_hello_mod

test: test-build
	$(TEST_DIR)/append_hello_mod_test

clean:
	$(MAKE) -C $(MODULE_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean