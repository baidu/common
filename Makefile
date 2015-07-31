
INCLUDE_PATH=-Iinclude/

COMMON_OBJ = $(patsubst %.cc, %.o, $(wildcard src/*.cc))

libcommon.a: $(COMMON_OBJ) $(COMMON_HEADER)
	$(AR) -rs $@ $(COMMON_OBJ)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm -rf libcommon.a
	rm -rf src/*.o
