![alt text](https://github.com/h010dev/starcmd/blob/main/data/icons/starcmd-logo.svg)

# About The Project

STARCMD is a personal desktop application aimed at helping developers and alike to store hard to remember commands. Users can add additional information such as examples, references, and documentation to aid in understanding the command at a later date. The goal is to reduce the need to create complex aliases or bookmark StackOverflow pages just to be able to revisit them again at a later date. 

This project is being actively worked on, so stay tuned for more updates!

### Built With
* GTK+ 3.0
* Glade
* C

# Getting Started

At its current state, you will have to compile the application locally to get it up and running. I've only tested this on Linux platforms (namely Ubuntu 20.0.4 LTS), but plan on expanding testing and coverage once the current iteration of the app is completed. 

The application has a few dependencies, those being:
* cURL
* sqlite3

These steps apply to Ubuntu 20.04 LTS but can be modified to work with other distros:

### Install Dependencies

```sh
sudo apt update && \
    sudo apt install libsqlite3-dev && \
    sudo apt install sqlite3 && \
    sudo apt install -y libgtk-3-dev && \
    sudo apt install libcurl4-openssl-dev && \
    sudo apt update
```

### Download Source Code and Setup Environment

```sh
git clone https://github.com/h010dev/starcmd.git && \
    cd starcmd && \
    ./config.sh
```

### Install Application

```sh
make && \
    sudo make install && \
    make clean
```


At this point, your system should have the application available in its app directory/browser under the name `StarCMD`.

### Uninstall Application

```sh
sudo make uninstall
```

# License

Distributed under the GPLv3.0 License. See `LICENSE` for more information.

# Contact

Email: mohamed.n.al.hussein@gmail.com
