LIB = -lm
CPP = g++
CPPFLAGS = -O -Wall
#CPPFLAGS = -g -Wall

OBJS = \
main.o\
matrix.o\
gal2ptBVP.o\
utility.o


runit : tryit
	tryit

tryit : $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJS) $(LIB)

main.o : main.cpp gal2ptBVP.h matrix.h utility.h
	$(CPP) -c $(CPPFLAGS) main.cpp

matrix.o : matrix.cpp matrix.h utility.h
	$(CPP) -c $(CPPFLAGS) matrix.cpp

gal2ptBVP.o : gal2ptBVP.cpp gal2ptBVP.h matrix.h utility.h
	$(CPP) -c $(CPPFLAGS) gal2ptBVP.cpp

utility.o : utility.cpp utility.h
	$(CPP) -c $(CPPFLAGS) utility.cpp

spotless : 
	rm tryit *~  *.o  example*
