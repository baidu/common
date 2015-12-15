



INCLUDEPATH=-I../include/ -Igtest/include
LDFLAGS=-L../ -Lgtest/lib -lgtest -lcommon -lpthread

TARGETS= test_logging test_thread_pool perf_test_thread_pool perf_test_counter test_mutex perf_test_mutex
all: $(TARGETS)

test_logging: test_logging.cc ../include/logging.h ../src/logging.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)
test_thread_pool: test_thread_pool.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

perf_test_counter: perf_test_counter.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

test_mutex: test_mutex.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_thread_pool: perf_test_thread_pool.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_mutex: perf_test_mutex.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

clean:
	rm -rf $(TARGETS)
