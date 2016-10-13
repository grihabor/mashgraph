## Запуск
cd в директорию, куда хотите склонировать репозиторий
```
git clone https://github.com/grihabor/mashgraph
cd task_02
```
В текущую директорию положите папку с проектом (template) и папку с данными (data).
```
mv data/multiclass data/base
```
Все готово к тому, чтобы использовать make.
make all - соберет проект, обучит и протестирует классификатор.
