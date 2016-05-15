vpath %.cpp ../source
vpath %.h ../source
game: main.o Desk.o Card.o Player.o level.o Interface.o Control.o
	g++ main.o Desk.o Card.o Player.o level.o Interface.o Control.o -o ../target/game
clean:
	rm -rf *.o game

