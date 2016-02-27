CC := clang
OBJDIR := obj
BINDIR := bin
SRCDIR := src
OBJS_MKFS := $(addprefix $(OBJDIR)/,mkfs.o SFS.o read_t.o)
OBJS_TSHELL := $(addprefix $(OBJDIR)/,mkfs.o SFS.o write_t.o read_t.o)
#CFLAGS :=
CFLAGS := -Wall -Wextra -pedantic

all: $(BINDIR)/mkfs $(BINDIR)/tshell

$(BINDIR)/mkfs: $(OBJS_MKFS)
		@$(CC) -o $(BINDIR)/mkfs $(OBJS_MKFS)
			@echo Building mkfs_t

$(BINDIR)/tshell: $(OBJDIR)/tshell.o $(OBJS_TSHELL)
		@$(CC) -o $(BINDIR)/tshell $(OBJS_TSHELL)
			@echo Building tshell

$(OBJDIR)/%.o:  $(SRCDIR)/%.c | $(OBJDIR) $(BINDIR)
		@$(CC) -c $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o
			@echo Building $*.o

$(OBJDIR):
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
			@echo Create $(OBJDIR) directory

$(BINDIR):
		@test -d $(BINDIR) || mkdir $(BINDIR)
			@echo Create $(BINDIR) directory

.PHONY: clean
clean:
	rm -rf $(BINDIR) $(OBJDIR)
