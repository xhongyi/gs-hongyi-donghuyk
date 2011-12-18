
PAPER = paper

TEX = $(wildcard ./*.tex) 
BIB = ref.bib

.PHONY: all, clean
all: $(TEX) $(BIB) $(FIGS) 
	pdflatex $(PAPER) 
	bibtex $(PAPER)
	pdflatex $(PAPER)
	pdflatex $(PAPER)

clean:
	rm -rf *.dvi *.log *.aux *.bbl *.blg *.pdf
