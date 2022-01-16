EXEC = main
CXXFLAGS = -Wall -MMD -lncurses -ggdb -g `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
CXX = gcc

OBJDIR = obj
SRCDIR = src
BINDIR = bin

CFILES = $(wildcard $(SRCDIR)/*.c)
OFILES = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CFILES))
DEPS = $(OFILES:.o=.d)

$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))

all : $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC) : $(OFILES)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c 
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./bin/$(EXEC)

dbg: all
	valgrind --leak-check=full bin/$(EXEC)

clean:
	$(RM) $(OFILES)
	$(RM) $(DEPS)

mrproper : clean
	$(RM) $(EXEC)

.PHONY: mrproper

-include $(DEPS)


