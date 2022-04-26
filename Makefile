CC = g++
OUTDIR = bin
MKDIR = mkdir -p

all:
	@$(MKDIR) $(OUTDIR)
	@$(CC) -o bin/big_grep src/big_grep.cpp -lboost_regex -Ofast

install: 
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(OUTDIR)/* $(DESTDIR)$(PREFIX)/bin/

clean:
	@rm $(OUTDIR)/big_grep 
