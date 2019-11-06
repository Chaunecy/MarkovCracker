TARGET = train guess
all: $(TARGET)

train: markov_train.cpp common.h
	g++ markov_train.cpp -o train -O2 -no-pie

guess: markov_guess.cpp common.h
	g++ markov_guess.cpp -o guess -O2 -no-pie

.PHONY: clean
clean:
	rm -f train
	rm -f guess
	rm -f *.o
