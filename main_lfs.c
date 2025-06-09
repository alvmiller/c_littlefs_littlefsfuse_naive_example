#include <lfs.h>

#include <unistd.h>

// @TODO: In Progress...

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

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

/* 
   For Build Test only.
   Result:
   file_lfs: littlefs/lfs.c:4218: lfs_init: Assertion `lfs->cfg->prog != ((void *)0)' failed.
   Aborted (core dumped)
*/
const void * const user_provided_block_device_read = read;
const void * const user_provided_block_device_prog = NULL;
const void * const user_provided_block_device_erase = NULL;
const void * const user_provided_block_device_sync = sync;

// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = user_provided_block_device_read,
    .prog  = user_provided_block_device_prog,
    .erase = user_provided_block_device_erase,
    .sync  = user_provided_block_device_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 128,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};

// entry point
int main(void) {
    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

    // read current count
    uint32_t boot_count = 0;
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // release any resources we were using
    lfs_unmount(&lfs);

    // print the boot count
    printf("boot_count: %d\n", boot_count);
    return 0;
}
