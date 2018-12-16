all: date

date: date_compute.sh

run: date
	./date_compute.sh

docker: date
	sudo docker build -t date-ubuntu .

docker_run: docker
	sudo docker run -it date-ubuntu ./compute_date.sh 4
	sudo docker run -it date-ubuntu ./compute_date.sh 5

clean:
	rm  *o. date
