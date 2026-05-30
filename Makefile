CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinclude
OBJS = build/board.o build/piece.o build/king.o build/queen.o build/rook.o build/bishop.o build/knight.o build/pawn.o 

all: undoTester

chess: build/main.o $(OBJS)
	$(CXX) -o chess build/main.o $(OBJS)

chessTester: build/test_driver.o $(OBJS)
	$(CXX) -o chessTester build/test_driver.o $(OBJS)

undoTester: build/undo_tester.o $(OBJS)
	$(CXX) -o undoTester build/undo_tester.o $(OBJS)

build/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 	
	rm -f build/*.o chessTester undoTester chess