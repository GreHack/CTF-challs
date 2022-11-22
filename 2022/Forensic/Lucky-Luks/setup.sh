#!/bin/sh -x

DISK0="disk-17-11-22.img"
DISK1="disk-19-11-22.img"
DISK_SIZE="20M"
CRYPTSETUP_NAME="disk"
CRYPTSETUP_DEVICE="/dev/mapper/$CRYPTSETUP_NAME"

# Create the disk image
qemu-img create "$DISK0" "$DISK_SIZE"

# Prepare the first disk
cryptsetup luksFormat "$DISK0"
sudo cryptsetup open "$DISK0" "$CRYPTSETUP_NAME"
sudo mkfs.ext4 "$CRYPTSETUP_DEVICE"
sudo mount "$CRYPTSETUP_DEVICE" /mnt/
sudo cp notes0 /mnt/notes
sudo umount /mnt
sudo cryptsetup close "$CRYPTSETUP_NAME"

# Prepare the second disk
cp "$DISK0" "$DISK1"

# Change the previous key with a secure one
cryptsetup luksAddKey "$DISK1"
cryptsetup luksRemoveKey "$DISK1"

# Write the content of the second disk
sudo cryptsetup open "$DISK1" "$CRYPTSETUP_NAME"
sudo mount "$CRYPTSETUP_DEVICE" /mnt/
sudo cp flag /mnt
sudo cp notes1 /mnt/notes
sudo umount /mnt
sudo cryptsetup close "$CRYPTSETUP_NAME"
