sudo apt-get isntall build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
git clone https://github.com/Itseez/opencv.git
cd opencv
mkdir release
cd release
#cmake -D CMAKE_BUILD_TYPE=release -D CMAKE_INSTALL_PREFIX=/usr/local ..
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_OPNCL=OFF -D BUILD_opencv_gpu=OFF -D BUILD_opencv_nonfree=OFF -D BUILD_opencv_stitching=OFF -D BUILD_opencv_superres=OFF ..
make
sudo make install

