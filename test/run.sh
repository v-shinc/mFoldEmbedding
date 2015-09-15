g++ -std=c++11 -o evaluate  evaluate.cpp -O2 -larmadillo -lopenblas -llapack 

./evaluate ../data_fb13k/entitylist.txt ../data_fb13k/relationlist.txt ../$1/entity2vec ../$1/bias2vec ../$1/normal2vec ../$1/tranf2vec ../data_fb13k/testM.txt 
