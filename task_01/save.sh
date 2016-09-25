if [ "$1" == "" ] || [ "$1" == "send" ]; then
    echo "Save graphics folder"
    rm graphics.tar.gz
    rm solution.tar.gz
    rm -r output
    mkdir ../.temp_pictures
    mv pictures ../.temp_pictures/
    cd ..
    tar -czvf graphics.tar.gz graphics
    cd graphics
    mv ../graphics.tar.gz ./graphics.tar.gz
    mv ../.temp_pictures/pictures ./
    rm -r ../.temp_pictures
    if [ "$1" == "send" ]; then
        python send.py "grihabor@mail.ru" "9+uyp5lbZHJH" "graphics.tar.gz"
    fi
else
    if [ "$1" == "project" ]; then
        echo "Save only the project"
        cd align_project
        make clean
        cd ..
        tar cvzf solution.tar.gz align_project
    else
        echo "Wrong parameter"
    fi
fi
