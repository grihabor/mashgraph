all:
	make cross
	
CUR_PATH = $(shell pwd)

build:
	cd template; make all
clean:
	cd template; make clean	
train:
	template/build/bin/task2 -d data/multiclass/train_labels.txt \
		-m output/model.txt --train
predict:
	template/build/bin/task2 --predict --predicted_labels=output/predictions.txt \
		-d data/multiclass/test_labels.txt -m output/model.txt 	
test:
	python template/compare.py data/multiclass/test_labels.txt output/predictions.txt

cross:
	make build
	
	rm -f output/results.txt
	mkdir -p output
	
	python > output/params.txt util/get_params.py template/src/task2.cpp
	
	for i in `seq 20`; do \
		python util/gendata.py .. .9 ; \
		make train ; \
		make predict ; \
		python >> output/results.txt template/compare.py \
			data/multiclass/test_labels.txt output/predictions.txt ; \
	done
	
	python util/average.py output/results.txt
