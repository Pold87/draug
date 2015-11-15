tar -zcvf genimgs.tar.gz genimgs/ 
rclone copy genimgs.tar.gz google:genimgs.tar.gz
rclone copy targets.csv google:targets.csv
