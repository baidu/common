



INCLUDEPATH=-I../include/ -Igtest/include
LDFLAGS=-L../ -Lgtest/lib -lgtest -lcommon -lpthread

TARGETS= test_thread_pool perf_test_thread_pool perf_test_counter
all: $(TARGETS)

test_thread_pool: test_thread_pool.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

perf_test_counter: perf_test_counter.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

perf_test_thread_pool: perf_test_thread_pool.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

clean:
	rm -rf $(TARGETS)
