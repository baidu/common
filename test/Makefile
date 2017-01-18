



INCLUDEPATH=-I../include/ -Igtest/include -I../../../thirdparty/boost_1_57_0/
CXXFLAGS=-std=c++11 -g -O2
#TCMALLOC=-L../../tera/thirdparty/lib -ltcmalloc_minimal
LDFLAGS=$(TCMALLOC) -L../ -Lgtest/lib -lgtest -lcommon -lpthread -lrt

TARGETS= test_logging test_thread_pool perf_test_thread_pool perf_test_counter test_mutex \
		 test_util perf_test_mutex perf_test_condvar perf_test_framework
all: $(TARGETS)

test_logging: test_logging.cc ../include/logging.h ../src/logging.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)
test_thread_pool: test_thread_pool.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

test_thread: test_thread.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_counter: perf_test_counter.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

test_mutex: test_mutex.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

test_util: test_util.cc ../include/util.h
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_thread_pool: perf_test_thread_pool.cc ../include/thread_pool.h
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_mutex: perf_test_mutex.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_condvar: perf_test_condvar.cc
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)

perf_test_framework: perf_test_framework.cc ../include/mutex.h
	$(CXX) $(CXXFLAGS) $(INCLUDEPATH) $< -o $@ $(LDFLAGS)


clean:
	rm -rf $(TARGETS)
