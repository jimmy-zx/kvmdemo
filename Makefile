all: README.pdf

README.pdf: README.md
	pandoc -t beamer README.md -o README.pdf
