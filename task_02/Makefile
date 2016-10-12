all:
	make cross
	
build:
	cd template; make all
clean:
	cd template; make clean	
train:
	template/build/bin/task2 -d data/multiclass/train_labels.txt -m model.txt --train
predict:
	template/build/bin/task2 --predict --predicted_labels=predictions.txt -d data/multiclass/test_labels.txt -m model.txt model.txt 
	
	
CUR_PATH = $(shell pwd)

test:
	python template/compare.py $(CUR_PATH)/data/multiclass/test_labels.txt $(CUR_PATH)/predictions.txt

cross:
	make build
	
	rm -f output.txt
	
	for i in `seq 20`; do \
		python gendata.py .9 ; \
		make train ; \
		make predict ; \
		python  >> output.txt template/compare.py $(CUR_PATH)/data/multiclass/test_labels.txt $(CUR_PATH)/predictions.txt ; \
	done
	
	python average.py output.txt
