GPP=g++
ARGS=-Wall -Wextra -pedantic -O3 -std=gnu++20
SRC_DIR=src
IMPL_DIR=private

ps2algo: $(SRC_DIR)/ps2algo.cpp
	$(GPP) $(ARGS) -I./include -o ps2algo $(SRC_DIR)/ps2algo.cpp $(IMPL_DIR)/*.cpp

clean:
	rm ps2algo
