# ATMTool
This is a tool used for decrypting and ecrypting `.ATM` files from the video game Rock Band Unplugged. This tool was created by reverse-engineering the Rock Band Unplugged EBOOT and mimicking the encrypt/decrypt processes used within the game. 

## Building
In order to build ATMTool, you must follow these steps:

1. `git clone https://github.com/maodus/ATMTool.git`
2. `cd ATMTool/`
3. `mkdir build`
4. `cd build/`
5. `cmake .. && make`

The final executable will then be located within `../bin`

## Usage
For a general overview of the program, execute the following: `./ATMTool -h`

### Decryption
You can decrypt an `ATM` file by running `./ATMTool -d filename.ATM`. This will output the 5 tracks held within the encrypted files.

- drums.at3
- bass_guitar.at3
- lead_guitar.at3
- vocals.at3
- fill.at3

### Encryption
You can encrypt an `ATM` file by running `./ATMTool -e filename.ATM`. This will generate one `ATM` file that can be used within Rock Band Unplugged. Before this command is executed, the files mentioned above (with the exact names mentioned), must be located in the same directory as the executable.

## Support
If you'd like to support me and my software development adventures, feel free to contribute. Your support means a lot to me!

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D31BH824)
