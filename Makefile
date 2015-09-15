objects=mFold.cpp

test: $(objects)
	g++ -std=c++11  $(objects) -o   mFoldEmbedding -O2 -larmadillo -lopenblas -llapack 

clean :
	rm test 
