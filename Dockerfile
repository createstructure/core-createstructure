# Get the ubuntu image from Docker Hub
FROM ubuntu:latest

# Labels
LABEL org.opencontainers.image.authors="DavideC03"
LABEL org.opencontainers.image.title="core createstructure"
LABEL org.opencontainers.image.url="https://github.com/createstructure/core-createstructure"
LABEL org.opencontainers.image.description DESCRIPTION
LABEL org.opencontainers.image.licences="GNU General Public License v3.0"
LABEL org.opencontainers.image.source="https://github.com/createstructure/core-createstructure"
LABEL org.opencontainers.image.documentation="https://raw.githubusercontent.com/createstructure/core-createstructure/main/docs/README.md"

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY ./bin /usr/src/createstructure

# Specify the working directory
WORKDIR /usr/src/createstructure

# Install requirements
RUN apt-get update
RUN apt-get install g++ build-essential libssl-dev libcurl4-openssl-dev git -y

# Use G++ to compile the source file
RUN make install

# Setup git
RUN git config --global user.email "help@castellanidavide.it"
RUN git config --global user.name "createstructure"
RUN git config --global init.defaultBranch "main"

# Run the program output from the previous step
ENTRYPOINT ["./core"]
