# AssaultCube External
This is a simple external cheat for the open source game AssaultCube. It is written in C++ and uses the Windows API for memory reading and writing. The cheatas been tested on the latest version of AssaultCube.

This project was created in order to learn more about game hacking and reverse engineering. It is meant to be used for educational purposes only.

### Disclaimer
This project is for educational purposes only. I do not condone cheating in any form.

## Showcase
![image](https://github.com/user-attachments/assets/0c5da17a-5036-49f3-b32f-39bff8a3b039)
![Screenshot 2024-08-14 154952](https://github.com/user-attachments/assets/16be2c84-46ef-46f1-80cd-d981906631a1)
![Screenshot 2024-08-14 162058](https://github.com/user-attachments/assets/4f55a968-9f2c-4c95-b71d-0ffd50e272d0)

## Features
- ESP (Name, Health, Box)
- Infinite Health
- Infinite Ammo
- Rapid Fire
- Stream Proof

## Dependencies
- [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [CMake](https://cmake.org/)

## Get Started (Windows Only)
1. Clone the repository recursively
```bash
git clone --recursive git@github.com:1neskk/AssaultCubeExternal.git
```
2. Run the script `setup.bat` to build the project
3. Open the solution file inside the `build` directory
4. Set the subsystem to Windows and build the solution
5. Run the resulting executable

## Usage
- Press 'INSERT' to toggle the imgui menu
- The 'Attach' button will attach the program to the game's proccess (the game must be open)
- When attached to the game's proccess the 'Detach' button will detach the program from the game's proccess
- The 'Exit' button will close the program (Only way to close the program except by using task manager)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
