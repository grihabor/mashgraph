all:
	cd template; make all
clean:
	cd template; make clean	
train:
	template/build/bin/task2 -d data/multiclass/train_labels.txt -m model.txt --train
predict:
	template/build/bin/task2 -d data/multiclass/test_labels.txt -m model.txt model.txt --predict
compare:
	python template/compare.py
