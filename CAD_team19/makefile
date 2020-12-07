
CXX = g++ -O3 

TARGET = cadb141.o


all: $(TARGET)

$(TARGET): main.o readgraph.o readcase.o updatePIQ.o traceTimeInterval.o forward.o longestpath.o graph.o
	$(CXX)  -o $(TARGET) main.o readgraph.o readcase.o updatePIQ.o traceTimeInterval.o forward.o longestpath.o graph.o

#main.o : main.cpp 
#	$(CFLAGS) -c main.cpp 

main.o : main.cpp 
	$(CXX)  -c main.cpp

readgraph.o : readgraph.cpp
	$(CXX)  -c readgraph.cpp 

readcase.o : readcase.cpp 
	$(CXX)  -c readcase.cpp 
 
updatePIQ.o : updatePIQ.cpp
	$(CXX)  -c updatePIQ.cpp 	
 
traceTimeInterval.o : traceTimeInterval.cpp
	$(CXX)  -c traceTimeInterval.cpp 	
 
forward.o : forward.cpp
	$(CXX)  -c forward.cpp 	
 
graph.o :graph.cpp
	$(CXX)  -c graph.cpp 	

longestpath.o : longestpath.cpp
	$(CXX)  -c longestpath.cpp 	


#longestpath.o : longestpath.cpp graph.cpp
#	$(CXX)  -c longestpath.cpp graph.cpp 	 
 
#graph.o : graph.cpp
#	$(CXX)  -c forward.cpp 	 
.PHONY: all clean
	
clean:
	$(RM) $(TARGET) *.o *~ 
 
run_case1: $(TARGET) PD_case/case1/input/case1 
	./cadb141.o PD_case/case1/input/case1 4

run_case2: $(TARGET) PD_case/case2/input/case2
	./cadb141.o PD_case/case2/input/case2 10
 
run_case3: $(TARGET) PD_case/case3/input/case3
	./cadb141.o PD_case/case3/input/case3 6

run_case4: $(TARGET) PD_case/case4/input/case4
	./cadb141.o PD_case/case4/input/case4 6
 
run_case5: $(TARGET) PD_case/case5/input/case5
	./cadb141.o PD_case/case5/input/case5 10