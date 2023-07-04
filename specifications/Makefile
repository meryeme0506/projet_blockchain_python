CPPFLAGS += -I /usr/include/python3.7m
LIBS=-lpython3.7m

COMPOSANT = bip39

LIBSO = ${COMPOSANT}.so
LIBSO_OBJS = ${COMPOSANT}.o 

%.o: %.cpp
	g++ -c $(CPPFLAGS) $< -o $@

all: ${LIBSO}

include ../gcc.mk
include ../pybind11.mk

test: $(LIBSO)
	echo "Test execution"
	python3 test.py

	
clean:
	rm -f ${LIBSO}  ${LIBSO_OBJS} ${DESTINATION_LIBSO}


${DESTINATION_LIBSO}: ${LIBSO}
	cp $< $@
