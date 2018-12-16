all: run

run: rest_server
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib ./rest_server

rest_server: rest_server.c
	gcc -o rest_server rest_server.c -lulfius -ljansson

call_server:
	curl -XGET localhost:8080/run

copy_needed_libs:
	mkdir -p libs
	cp /usr/local/lib/libjansson.so.4 ./libs/
	cp /usr/local/lib/libulfius.so.2.3 ./libs/
	cp /usr/local/lib/libyder.so.1.3.3 ./libs/
	cp /usr/local/lib/libyder.so.1.3 ./libs/
	cp /usr/local/lib/liborcania.so.1.2 ./libs/
	cp /usr/local/lib/liborcania.so.1.2.3 ./libs/

docker_server_make: rest_server copy_needed_libs
	# Copy systems libs into a local dir so that we can include them
	# in the Docker container
	sudo docker build -t ttt-server .

# Running docker
run_docker:
	sudo docker run -p 8080:8080 -it ttt-server:latest

clean_image:
	docker images -a | grep "ttt-server" | awk '{print $3}' | xargs docker rmi -f

clean_container:
	docker ps -a | grep "ttt-server" | awk '{print $3}' | xargs docker rmi -f

clean:
	rm -rf ./rest_server ./libs
