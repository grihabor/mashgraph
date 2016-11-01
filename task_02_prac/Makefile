all: output_dir
	rm -f output/results.txt
	make build
	make train
	make predict
	make test
	cat output/results.txt
	
CUR_PATH = $(shell pwd)

build:
	cd template; make all
clean:
	cd template; make clean	
	rm -f -r output
	rm -f task_02.tar.gz

train: output_dir
	template/build/bin/task2 -d $(CUR_PATH)/data/multiclass/train_labels.txt \
		-m $(CUR_PATH)/output/model.txt --train
predict: output_dir
	template/build/bin/task2 --predict --predicted_labels=$(CUR_PATH)/output/predictions.txt \
		-d data/multiclass/test_labels.txt -m $(CUR_PATH)/output/model.txt 	
test: output_dir
	mv $(CUR_PATH)/output/predictions.txt $(CUR_PATH)/predictions.txt ; \
	python >> output/results.txt $(CUR_PATH)/template/compare.py \
		$(CUR_PATH)/data/multiclass/test_labels.txt $(CUR_PATH)/predictions.txt ; \
	mv $(CUR_PATH)/predictions.txt $(CUR_PATH)/output/predictions.txt ; \

output_dir:
	mkdir -p output
