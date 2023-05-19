# Hosts File Editor

This is a simple command line program to edit the hosts file. It works on both Windows and Linux (including WSL) systems.

## Features

- Interactive menu to add or edit entries.
- Validation for IP addresses and URLs.
- Ability to write back to the hosts file.

## How to Run

1. Compile the program using a C++ compiler. For example, if you're using g++, you can use the following command:

    ```
    g++ -o hosts_editor main.cpp
    ```

2. Run the executable. On Linux, use `./hosts_editor`. On Windows, just use `hosts_editor`.

## Important

- This program needs administrative privileges to modify the hosts file. Make sure to run it as an administrator or with sudo.
- If you're using WSL, the program will detect it and it will ask you if you want to use the Linux hosts file at `/etc/hosts`. If you choose no, the program will exit.

## Usage

- Choose an option from the menu by typing the number and pressing Enter.
- To add a new entry, choose the "Add a new entry" option and follow the prompts.
- To edit an existing entry, choose the "Edit an entry" option and follow the prompts.
- When you're done, choose the "Exit" option.

Note: You need to press Enter after typing an option number or any input.

## Limitations

- The program only supports IPv4 addresses.
- The program doesn't check if the URL is valid, it only checks if it contains valid characters.

## Contributions

Contributions are welcome. Please make a pull request.

## License

This program is free software and is distributed under the MIT License.
