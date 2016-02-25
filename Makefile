OBJDIR := obj
BINDIR := bin
SRCDIR := src
#OBJS := $(addprefix $(OBJDIR)/,mkfs_t.o)
CFLAGS :=
#CFLAGS := -Wall -Wextra -pedantic

all: $(BINDIR)/mkfs_t

$(BINDIR)/mkfs_t: $(OBJDIR)/mkfs_t.o
		@gcc -o $(BINDIR)/mkfs_t $(OBJDIR)/mkfs_t.o
			@echo Building mkfs_t

$(OBJDIR)/%.o:  $(SRCDIR)/%.c | $(OBJDIR) $(BINDIR)
		@gcc -c $(CFLAGS) $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o
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
