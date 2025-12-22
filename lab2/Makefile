# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

# Имя итогового исполняемого файла
TARGET = lab2

# Все объектные файлы
OBJS = lab1.o utils.o pipe.o station.o manager.o

# Цель по умолчанию
all: $(TARGET)

# Линковка
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило компиляции отдельных .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Запуск
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run