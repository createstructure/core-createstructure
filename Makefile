# For test only
# sudo apt install build-essential g++ libssl-dev libcurl4-openssl-dev git -y

all:
	make compile

createkeys:
	openssl genrsa -out private.pem 4096 > /dev/null && chmod +rwx private.pem
	openssl rsa -in private.pem -outform PEM -pubout -out public.pem > /dev/null && chmod +rwx public.pem

compile:
	g++ -Wall ./bin/core.cpp \
		./bin/local-libraries/getTemplate.cpp \
		./bin/local-libraries/getUploadURL.cpp \
		./bin/local-libraries/inputCheck.cpp \
		./bin/local-libraries/priority.cpp \
		./bin/local-libraries/repo.cpp \
		./bin/local-libraries/repoInfoCheck.cpp \
		./bin/local-libraries/workload.cpp \
		./bin/global-libraries/bin/cryptation.cpp \
		./bin/global-libraries/bin/rest.cpp \
		./bin/global-libraries/bin/sleep.cpp \
		-o core -lcurl -lcrypto

precompile:
	g++ -Wall -c ./bin/core.cpp -o ./bin/core.o
	g++ -Wall -c ./bin/local-libraries/getTemplate.cpp -o ./bin/local-libraries/getTemplate.o
	g++ -Wall -c ./bin/local-libraries/getUploadURL.cpp -o ./bin/local-libraries/getUploadURL.o
	g++ -Wall -c ./bin/local-libraries/inputCheck.cpp -o ./bin/local-libraries/inputCheck.o
	g++ -Wall -c ./bin/local-libraries/priority.cpp -o ./bin/local-libraries/priority.o
	g++ -Wall -c ./bin/local-libraries/repo.cpp -o ./bin/local-libraries/repo.o
	g++ -Wall -c ./bin/local-libraries/repoInfoCheck.cpp -o ./bin/local-libraries/repoInfoCheck.o
	g++ -Wall -c ./bin/local-libraries/workload.cpp -o ./bin/local-libraries/workload.o
	g++ -Wall -c ./bin/global-libraries/bin/cryptation.cpp -o ./bin/global-libraries/bin/cryptation.o
	g++ -Wall -c ./bin/global-libraries/bin/rest.cpp -o ./bin/global-libraries/bin/rest.o
	g++ -Wall -c ./bin/global-libraries/bin/sleep.cpp -o ./bin/global-libraries/bin/sleep.o

link:
	g++ -Wall ./bin/core.o \
		./bin/local-libraries/getTemplate.o \
		./bin/local-libraries/getUploadURL.o \
		./bin/local-libraries/inputCheck.o \
		./bin/local-libraries/priority.o \
		./bin/local-libraries/repo.o \
		./bin/local-libraries/repoInfoCheck.o \
		./bin/local-libraries/workload.o \
		./bin/global-libraries/bin/cryptation.o \
		./bin/global-libraries/bin/rest.o \
		./bin/global-libraries/bin/sleep.o \
		-o core -lcurl -lcrypto
