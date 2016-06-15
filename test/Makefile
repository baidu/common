



INCLUDEPATH=-I../include/ -Igtest/include -I../../../thirdparty/boost_1_57_0/
CXXFLAGS=-g -O2
#TCMALLOC=-L../../tera/thirdparty/lib -ltcmalloc_minimal
LDFLAGS=$(TCMALLOC) -L../ -Lgtest/lib -lgtest -lcommon -lpthread -lrt

TARGETS= test_logging test_thread_pool perf_test_thread_pool perf_test_counter test_mutex perf_test_mutex perf_test_condvar perf_test_framework
all: $(TARGETS)

test_logging: test_logging.cc ../include/logging.h ../src/logging.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)
test_thread_pool: test_thread_pool.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

test_thread: test_thread.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_counter: perf_test_counter.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

test_mutex: test_mutex.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_thread_pool: perf_test_thread_pool.cc ../include/thread_pool.h
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_mutex: perf_test_mutex.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS) 

perf_test_condvar: perf_test_condvar.cc
	g++ -g $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_framework: perf_test_framework.cc ../include/mutex.h
	g++ -g -O2 $(INCLUDEPATH) $< -o $@ $(LDFLAGS)


clean:
	rm -rf $(TARGETS)
