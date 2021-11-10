# Get the ubuntu image from Docker Hub
FROM ubuntu:latest

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY ./bin /usr/src/createstructure

# Specify the working directory
WORKDIR /usr/src/createstructure

# Install requirements, use GCC to compile the source file and remove the no longer needed packages
RUN apt-get update
    # Install the required packages
RUN apt-get install g++ libssl-dev libcurl4-openssl-dev git -y &&
    # Compile the source code
    g++ core.cpp -o core -std=c++17 -lcurl -lcrypto &&
    # Remove the no longer needed packages
    apt-get remove g++ libssl-dev libcurl4-openssl-dev -y; apt autoremove -y

# Setup git
RUN git config --global user.email "help@castellanidavide.it"
RUN git config --global user.name "createstructure"
RUN git config --global init.defaultBranch "main"

# Run the program output from the previous step
ENTRYPOINT ["./core"]
