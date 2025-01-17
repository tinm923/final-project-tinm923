CXX:=g++
STD:=-std=c++14
CFLAGS:=-Wall -Werror -Wextra -Weffc++ -pedantic
DEBUG_FLAGS:=-g

SRC:=$(wildcard *.cpp)
INC:=$(wildcard include/*.h)
OBJ:=$(SRC:%.cpp=obj/%.o)
DEP:=$(SRC:%.cpp=dep/%.d)
TRGT:=main.exe

INCLUDES:=-isysteminclude -I../include
PYTHON_INCLUDES:=$(shell python3-config --includes)
PYTHON_LIB_DIR:=$(shell python3-config --prefix)/lib
PYTHON_LD_FLAGS:=-Wl,-rpath,$(PYTHON_LIB_DIR) -L$(PYTHON_LIB_DIR) -lpython3.6m

HTML:=sandpile.html

$(TRGT): setup $(OBJ)
	$(CXX) $(PYTHON_LD_FLAGS) $(OBJ) -o $@

setup:
	@mkdir -p obj
	@mkdir -p dep

-include $(DEP)

obj/%.o: %.cpp $(INC)
	$(CXX) $(STD) $(DEBUG_FLAGS) $(CFLAGS) $(INCLUDES) $(PYTHON_INCLUDES) -MF dep/$(<:.cpp=.d) -MMD -c $< -o $@

clean:
	rm -rf obj dep $(TRGT) test_output __pycache__ $(HTML) *.dat

test: $(TEST) $(TRGT)
	contest contest.yaml

move-plot:
	@mkdir -p ~/public_html
	@chmod 755 ~/public_html
	@cp $(HTML) ~/public_html
	@chmod 755 ~/public_html/$(HTML)

.PHONY:all clean test setup move-plot
