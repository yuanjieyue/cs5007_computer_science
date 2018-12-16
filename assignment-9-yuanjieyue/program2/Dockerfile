#Download base image ubuntu 16.04
FROM ubuntu:16.04


#Copy the local compiled C binary into the container
COPY ./date_compute.sh /date_compute.sh

#Set the command for the container to run
CMD ["/date_compute.sh"]
