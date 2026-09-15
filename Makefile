CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -DAFE_NXP_UNIT_TEST -Isrc -Itest/arduino_stub

STUB_SRCS   := test/arduino_stub/Arduino_stub.cpp test/arduino_stub/SPI_stub.cpp
LIB_SRCS    := src/AFE_NXP.cpp src/SPI_for_AFE.cpp
BUILD_DIR   := build/test

TESTS := test_get_data24 test_cvd test_calc_delay test_raw2v

.PHONY: test clean

test: $(addprefix $(BUILD_DIR)/,$(TESTS))
	@set -e; for t in $^; do echo "== $$t =="; ./$$t; done

# test_get_data24 and test_cvd don't need the library's out-of-line sources.
$(BUILD_DIR)/test_get_data24: test/test_get_data24.cpp src/SPI_for_AFE.cpp $(STUB_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/test_cvd: test/test_cvd.cpp | $(BUILD_DIR)
	$(CXX) -std=c++17 -Wall -Wextra $< -o $@

$(BUILD_DIR)/test_calc_delay: test/test_calc_delay.cpp $(LIB_SRCS) $(STUB_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/test_raw2v: test/test_raw2v.cpp $(LIB_SRCS) $(STUB_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build
