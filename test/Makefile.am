check_PROGRAMS = cpp_miner_tests
cpp_miner_tests_CPPFLAGS = -I../src/
cpp_miner_tests_LDADD = -lboost_program_options -lboost_filesystem -lboost_system -lsecp256k1 -lboost_unit_test_framework -lcrypto
cpp_miner_tests_SOURCES = runner.cpp cpp_miner_tests.cpp ../src/util.cpp ../src/transaction.cpp
