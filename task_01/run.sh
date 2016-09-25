rm -r output
mkdir output
cd ./align_project
#make clean
make
cd ..
:'
cp "./pictures/ocean.bmp" "./output/"ocean".bmp"
./align_project/build/bin/align "./pictures/ocean.bmp" "./output/"ocean"_unsharp.bmp" --unsharp 
./align_project/build/bin/align "./pictures/ocean.bmp" "./output/"ocean"_gray-world.bmp" --gray-world 
./align_project/build/bin/align "./pictures/ocean.bmp" "./output/"ocean"_median.bmp" --median 3
./align_project/build/bin/align "./pictures/ocean.bmp" "./output/"ocean"_median_linear.bmp" --median-linear 5
./align_project/build/bin/align "./pictures/ocean.bmp" "./output/"ocean"_median_const.bmp" --median-const 7

exit
'
for i in 01 02 03 04 05 06 07 08 09 10 11 12
do
    ./align_project/build/bin/align "./pictures/$i.bmp" "./output/$i.bmp" --align
    
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_autoconstrast.bmp" --autocontrast 0.05
    
    : '
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_median.bmp" --median 1
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_median_linear.bmp" --median-linear 1
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_median_const.bmp" --median-const 1
    
    
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_unsharp.bmp" --unsharp
    
    ./align_project/build/bin/align "./output/$i.bmp" "./output/"$i"_gray.bmp" --gray-world
    cp ./pictures/20.bmp ./output/00$i.bmp
    
    ./align_project/build/bin/align ./pictures/20.bmp ./output/custom.bmp --custom "1;0;0"
    ./align_project/build/bin/align ./pictures/20.bmp ./output/sobel_x.bmp --sobel-x
    ./align_project/build/bin/align ./pictures/20.bmp ./output/sobel_y.bmp --sobel-y
    ./align_project/build/bin/align ./pictures/20.bmp ./output/gaussian.bmp --gaussian 1.4 2
    ./align_project/build/bin/align ./pictures/20.bmp ./output/gaussian-sep.bmp --gaussian-separable 1.4 2
    '
    
done
