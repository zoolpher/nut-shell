CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = nut-shell
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)