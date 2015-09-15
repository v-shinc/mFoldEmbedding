make

time ./mFoldEmbedding -dim 100 -epoch 100 -batch 1000  -lr 0.005  -margin 1 -epsilon 0.1 -beta 0.015 -entity data_fb13k/entitylist.txt -rel data_fb13k/relationlist.txt -train data_fb13k/trainM.txt -bias_out result/bias2vec -entity_out result/entity2vec -normal_out result/normal2vec -a_out result/tranf2vec
