default: bin

all: bin doc

bin:
	scons

doc:
	(cd doxygen && doxygen corona.doxy)

.PHONY: all bin default doc
