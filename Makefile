CXXFLAGS = -Wall -std=c++17

all: chessTester

chess: main.o board.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o
	g++ $(CXXFLAGS) -o chess main.o board.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o

chessTester: testBoard.o board.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o
	g++ $(CXXFLAGS) -o chessTester testBoard.o board.o piece.o king.o queen.o rook.o bishop.o knight.o pawn.o

testBoard.o: testBoard.cc board.h piece.h king.h queen.h rook.h bishop.h knight.h pawn.h
	g++ $(CXXFLAGS) -c testBoard.cc

main.o: main.cc board.h piece.h king.h queen.h rook.h bishop.h knight.h pawn.h
	g++ $(CXXFLAGS) -c main.cc

board.o: board.cc board.h king.h queen.h rook.h bishop.h knight.h pawn.h
	g++ $(CXXFLAGS) -c board.cc

king.o: king.cc king.h piece.h
	g++ $(CXXFLAGS) -c king.cc

queen.o: queen.cc queen.h piece.h
	g++ $(CXXFLAGS) -c queen.cc

rook.o: rook.cc rook.h piece.h
	g++ $(CXXFLAGS) -c rook.cc

bishop.o: bishop.cc bishop.h piece.h
	g++ $(CXXFLAGS) -c bishop.cc

knight.o: knight.cc knight.h piece.h
	g++ $(CXXFLAGS) -c knight.cc

pawn.o: pawn.cc pawn.h piece.h
	g++ $(CXXFLAGS) -c pawn.cc

clean: 	
	rm -f *.o chessTester chess


# CXX = g++
# CXXFLAGS = -Wall -std=c++17

# # Shared object files (common to game and tester)
# COMMON_OBJS = board.o piece.o rook.o bishop.o knight.o queen.o king.o pawn.o

# # Main game
# GAME_TARGET = chess
# GAME_SRCS = main.cc $(COMMON_OBJS:.o=.cc)
# GAME_OBJS = main.o $(COMMON_OBJS)

# # Tester
# TEST_TARGET = chessTester
# TEST_SRCS = testBoard.cc $(COMMON_OBJS:.o=.cc)
# TEST_OBJS = testBoard.o $(COMMON_OBJS)

# # Default target builds tester
# all: $(TEST_TARGET)

# # Build game
# $(GAME_TARGET): $(GAME_OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(GAME_OBJS)

# # Build tester
# $(TEST_TARGET): $(TEST_OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS)

# # Generic rule to compile any .cc to .o
# %.o: %.cc
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean everything
# clean:
# 	rm -f *.o $(GAME_TARGET) $(TEST_TARGET)
