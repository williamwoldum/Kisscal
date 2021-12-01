CC := gcc
CFLAGS := -std=c89 -Wall

MEXE := Kisscal.exe
TEXE := Tester.exe

MOUT := Kisscal.out
TOUT := Tester.out

#########################################################################

MAIN_SRC := main/sources
MAIN_OBJ := main/objects
MAIN_DEP := main/dependencies

TEST_SRC := test/sources
TEST_OBJ := test/objects
TEST_DEP := test/dependencies

#########################################################################

MAIN_SRCS := $(wildcard $(MAIN_SRC)/*.c)
MAIN_OBJS := $(patsubst $(MAIN_SRC)/%.c, $(MAIN_OBJ)/%.o, $(MAIN_SRCS))
MAIN_DEPS := $(patsubst $(MAIN_OBJ)/%.o, $(MAIN_DEP)/%.d, $(MAIN_OBJS))

TEST_SRCS := $(wildcard $(TEST_SRC)/*.c) $(wildcard $(TEST_SRC)/*/*.c)
TEST_OBJS_PRE := $(patsubst $(TEST_SRC)/%.c, $(TEST_OBJ)/%.o, $(TEST_SRCS)) 
TEST_OBJS := $(addprefix $(TEST_OBJ)/,$(notdir $(TEST_OBJS_PRE)))
TEST_DEPS := $(patsubst $(TEST_OBJ)/%.o, $(TEST_DEP)/%.d, $(TEST_OBJS))

#########################################################################

all: $(MEXE) $(TEXE) $(MOUT) $(TOUT)

-include $(MAIN_DEPS)
-include $(TEST_DEPS)

MAIN_DEPFLAGS = -MMD -MF $(patsubst $(MAIN_OBJ)/%.o, $(MAIN_DEP)/%.d, $(@))
TEST_DEPFLAGS = -MMD -MF $(patsubst $(TEST_OBJ)/%.o, $(TEST_DEP)/%.d, $(@))

$(MEXE): $(MAIN_OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(TEXE): $(TEST_OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(MOUT): $(MAIN_OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(TOUT): $(TEST_OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

$(MAIN_OBJ)/%.o: $(MAIN_SRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(MAIN_DEPFLAGS)

$(TEST_OBJ)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(TEST_DEPFLAGS)

vpath %.c $(shell find $(TEST_SRC) -type d)

$(shell mkdir -p $(MAIN_OBJ) $(MAIN_DEP))
$(shell mkdir -p $(TEST_OBJ) $(TEST_DEP))

clean:
	rm -rf $(MAIN_OBJ) $(MAIN_DEP)
	rm -rf $(TEST_OBJ) $(TEST_DEP)
	rm -f *.exe *.out
	rm -f ./main/cal_storage
	rm -f ../.DS_Store
