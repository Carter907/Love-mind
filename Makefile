CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -g -Isrc
TARGET = love-mind

SRC = src/main.c src/tokenizer.c src/model.c src/parser.c
LIB_UNITY = lib/unity/unity.c
UNITY_CFLAGS = -Ilib/unity

TEST_TARGET = test_runner
TEST_SRC = tests/test_tokenizer.c src/tokenizer.c $(LIB_UNITY)

TEST_MODEL_TARGET = test_model_runner
TEST_MODEL_SRC = tests/test_model.c src/model.c $(LIB_UNITY)

TEST_PARSER_TARGET = test_parser_runner
TEST_PARSER_SRC = tests/test_parser.c src/parser.c src/tokenizer.c src/model.c $(LIB_UNITY)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TEST_TARGET) $(TEST_MODEL_TARGET) $(TEST_PARSER_TARGET)
	./$(TEST_TARGET)
	./$(TEST_MODEL_TARGET)
	./$(TEST_PARSER_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(UNITY_CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

$(TEST_MODEL_TARGET): $(TEST_MODEL_SRC)
	$(CC) $(CFLAGS) $(UNITY_CFLAGS) $(TEST_MODEL_SRC) -o $(TEST_MODEL_TARGET)

$(TEST_PARSER_TARGET): $(TEST_PARSER_SRC)
	$(CC) $(CFLAGS) $(UNITY_CFLAGS) $(TEST_PARSER_SRC) -o $(TEST_PARSER_TARGET)

run: $(TARGET)
	./$(TARGET)

format:
	@command -v clang-format >/dev/null 2>&1 && clang-format -i src/*.c src/*.h tests/*.c || echo "clang-format not found; skipping formatting"

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_MODEL_TARGET) $(TEST_PARSER_TARGET)

.PHONY: all run clean test format
