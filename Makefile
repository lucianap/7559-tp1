tp1: src/Cajon/Cajon.cpp src/Productor/Productor.cpp src/Ramo/Ramo.cpp src/Utils/Utils.cpp src/main.cpp
	g++ -o tp1 src/Cajon/Cajon.cpp src/Productor/Productor.cpp src/Ramo/Ramo.cpp src/Utils/Utils.cpp src/main.cpp -Isrc/ 
clean: 
	rm -f *.o tp1