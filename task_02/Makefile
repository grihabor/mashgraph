all:
	rm output/results.txt
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


cross: output_dir
	make build
	
	rm -f output/results.txt
	
	python > output/params.txt util/get_params.py $(CUR_PATH)/template/src/task.cpp
	
	for i in `seq 20`; do \
		python util/gendata.py .. .9 ; \
		make train ; \
		make predict ; \
		make test ; \
	done
		
	python util/average.py $(CUR_PATH)/output/results.txt

output_dir:
	mkdir -p output

tar:
	make build
	if [ -d data/base ]; then \
		cp data/base/* data/multiclass/ -f -r ; \
	else \
		cp data/labels.txt data/multiclass/train_labels.txt ; \
	fi 
	make train
	cp output/model.txt template/model.txt
	cd template; make clean
	
	if [ -d template/.idea ]; then \
		mv template/.idea ./.idea-backup ; \
	fi
	
	tar -czvf task_02.tar.gz template
	rm template/model.txt
	
	if [ -d ./.idea-backup ]; then \
		mv ./.idea-backup template/.idea ; \
	fi
	
