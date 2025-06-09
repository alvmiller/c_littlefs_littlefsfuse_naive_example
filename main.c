#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
https://github.com/littlefs-project
https://github.com/mluis/qemu-esp32
https://www.thevtool.com/mounting-littlefs-on-linux-machine/
https://github.com/littlefs-project/littlefs/tree/master
https://github.com/littlefs-project/littlefs-fuse


littlefs:
make
sudo apt install golang-github-pelletier-go-toml
pip install toml
make test


littlefs-fuse:
fusermount -V
sudo apt-get install libfuse-dev
make
cd littlefs/
make
losetup
(find device that not in use right now if possible)
ls -la /dev/
sudo chmod a+rw /dev/loop16
dd if=/dev/zero of=image bs=512 count=2048
sudo losetup -d /dev/loop8
or
sudo losetup -f
losetup /dev/loop16 image
./lfs --format /dev/loop16
mkdir mount
./lfs /dev/loop16 mount
cd mount
echo "hello" > hi.txt
ls
cat hi.txt
(like all other FSs)
pwd
( /home/user1/littlefs/littlefs-fuse-master/mount )
( /home/user1/littlefs/littlefs-fuse-master/littlefs )
( /home/user1/littlefs/littlefs-fuse-master/ )
df -H

gcc -o file_lfs \
-I/home/user1/littlefs/littlefs-fuse-master/littlefs/ \
main_lfs.c \
-L/home/user1/littlefs/littlefs-fuse-master/littlefs -llfs

gcc main.c
./a.out

cd ..
umount mount
(after work)
sudo losetup -d /dev/loop16
*/

int main(void)
{
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("/home/user1/littlefs/littlefs-fuse-master/mount/hi.txt", "w");

    // Write some text to the file
    fprintf(fptr, "Some text\n");

    // Close the file
    fclose(fptr);

    return 0;
}
