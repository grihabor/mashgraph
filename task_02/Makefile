all:
	make cross
	
CUR_PATH = $(shell pwd)

build:
	cd template; make all
clean:
	cd template; make clean	
train:
	template/build/bin/task2 -d $(CUR_PATH)/data/multiclass/train_labels.txt \
		-m $(CUR_PATH)/output/model.txt --train
predict:
	template/build/bin/task2 --predict --predicted_labels=$(CUR_PATH)/output/predictions.txt \
		-d data/multiclass/test_labels.txt -m $(CUR_PATH)/output/model.txt 	
test:
	python template/compare.py $(CUR_PATH)/data/multiclass/test_labels.txt $(CUR_PATH)/predictions.txt


cross:
	make build
	
	rm -f output/results.txt
	mkdir -p output
	
	python > output/params.txt util/get_params.py $(CUR_PATH)/template/src/task2.cpp
	
	for i in `seq 3`; do \
		python util/gendata.py .. .9 ; \
		make train ; \
		make predict ; \
		mv $(CUR_PATH)/output/predictions.txt $(CUR_PATH)/predictions.txt ; \
		python >> output/results.txt $(CUR_PATH)/template/compare.py \
			$(CUR_PATH)/data/multiclass/test_labels.txt $(CUR_PATH)/predictions.txt ; \
		mv $(CUR_PATH)/predictions.txt $(CUR_PATH)/output/predictions.txt ; \
	done
		
	python util/average.py $(CUR_PATH)/output/results.txt
