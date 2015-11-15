tar zcvf genimgs.tar.gz genimgs/
rclone sync genimgs.tar.gz google:
rclone sync targets.csv google:
