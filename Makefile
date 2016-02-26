CC := clang
OBJDIR := obj
BINDIR := bin
SRCDIR := src
OBJS := $(addprefix $(OBJDIR)/,mkfs_t.o SFS.o)
CFLAGS :=
#CFLAGS := -Wall -Wextra -pedantic

all: $(BINDIR)/mkfs_t

$(BINDIR)/mkfs_t: $(OBJDIR)/mkfs.o $(OBJDIR)/SFS.o $(OBJDIR)/write_t.o $(OBJDIR)/read_t.c
		@$(CC) -o $(BINDIR)/mkfs $(OBJDIR)/mkfs.o $(OBJDIR)/SFS.o
			@echo Building mkfs_t

$(BINDIR)/tshell: $(OBJDIR)/tshell.o $(OBJDIR)/SFS.o $(OBJDIR)/write_t.o
		@$(CC) -o $(BINDIR)/tshell $(OBJDIR)/tshell.o $(OBJDIR)/SFS.o
			@echo Building mkfs_t

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
