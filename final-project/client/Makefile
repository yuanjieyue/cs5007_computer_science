all: run

run: Client
	java -cp bin:./libs/* edu.neu.cs5007.Client

Client: src/edu/neu/cs5007/Client.java
	mkdir -p bin
	javac -Xlint:deprecation -d bin -sourcepath src -cp ./libs/*:. src/edu/neu/cs5007/Client.java

clean:
	rm -rf bin
