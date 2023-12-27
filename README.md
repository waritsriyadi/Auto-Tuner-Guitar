![wiringSCA](https://github.com/waritsriyadi/Auto-Tuner-Guitar/assets/125733071/64a803c2-ed39-4b9b-95d2-add90462caaf)Developed an automatic guitar tuning system using fuzzy logic, providing guitar players with an easy way to achieve their desired tones.

Things you need:
1. OLED 128x64
2. Voice Sensor MAX4466
3. ESP32 or Any Microcontroller
4. ESP32 Expansion Board (optional)
5. Power Adapter 5V
6. USB to Micro-USB

WIRING DIAGRAM 
![wiringSCA](https://github.com/waritsriyadi/Auto-Tuner-Guitar/assets/125733071/4b2e9311-b6de-4f07-aa98-4da57c2a708a)


Say Hi!~

⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣟⠻⡿⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣸⣻⠿⢮⡿⣾⡖⠚⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣄⠒⢦⠸⣿⣄⠙⣧⠀⠰⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣛⣷⣞⠳⣄⠘⡆⠙⢇⠀⠀⠰⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣸⣯⡝⠻⣿⣏⠉⠻⣮⣷⣝⣶⣼⡀⠀⠀⢱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣇⠈⠙⢤⠈⢳⡀⠀⠹⣌⢻⠀⠙⣧⠀⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣼⠋⠳⢤⣄⡑⠀⠹⡄⠀⣌⠀⢳⡀⠸⡆⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⣠⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢹⣄⠀⠂⢻⡉⠑⣦⠹⡄⠸⡄⠀⢷⡄⡇⠀⠀⡇⠀⠀⠀⠀⣀⠤⠒⠋⠁⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠙⠿⣶⣤⣝⠀⠀⠀⢿⡶⢧⠀⠸⡇⡗⠀⢀⠃⠀⢀⡴⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣠⣤⣤⣤⣀⣀⡀⠀⠀
⠀⠀⠀⠀⢀⣀⣀⣀⡀⠙⢻⣷⣦⡀⠈⠋⠈⢧⠀⠀⡇⠀⠈⠀⠔⠋⠀⠀⢀⣀⣤⣤⣤⡶⢶⣿⠿⠭⢥⢽⣁⣤⠀⠒⠿⠿⣿⣷⣆
⠀⢀⡠⠚⠉⠉⠀⠀⠀⠀⠀⠙⢿⣿⣶⣤⣤⡘⣆⣼⣃⣴⣦⠀⠀⣀⡴⠞⣛⡉⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠘⣿⣛⣖⠲⠦⣄⣸⡿
⢠⠾⠒⠶⠀⠀⠀⠀⠀⡀⠀⡙⢺⣏⠛⢿⣿⣿⣿⣿⣿⣿⣿⣶⠞⡩⠖⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠓⠒⠤⣍⣙⣓⠶⣴⣶⠟⠁
⡾⠀⠂⠀⠀⠀⠀⠀⠈⠑⠉⢡⣿⣿⠛⠲⣤⣯⣿⣿⣿⣿⣿⣥⡞⠒⠒⠚⠛⠓⠠⣀⠀⠀⠀⠠⠄⠀⠠⣤⣷⣌⢙⣿⣦⡾⠋⠀⠀
⡗⠺⠤⠛⠉⠉⠁⠐⠉⠉⣹⣿⢿⢃⣠⣾⢋⣾⣿⣿⣿⣟⠻⠏⡿⣗⣦⡀⠀⠀⠀⠈⠙⠲⣄⡀⠀⠀⠀⠈⠙⠿⣿⣿⠟⠀⠀⠀⠀
⣇⣠⠤⠀⠀⠀⠀⢀⣴⡾⢛⡽⢋⣽⣟⣵⢫⣿⣿⣯⡏⢣⠀⠄⠀⠈⠉⢙⣻⠖⢦⣄⡀⠀⠤⢙⠶⣤⡀⠈⢳⣰⡟⠁⠀⠀⠀⠀⠀
⢿⠀⠀⠀⢀⡴⢪⠟⢩⠖⠋⢠⡾⢻⡟⢁⣿⣿⣿⡟⡀⠘⠀⣴⠀⣤⡐⠂⠈⠉⢗⣟⠿⣶⣤⡀⠲⣤⣹⣿⠞⠉⠀⠀⠀⠀⠀⠀⠀
⠘⣧⠀⠀⠈⠀⠀⠀⠀⡀⣠⠏⣴⠏⠀⢸⣿⠟⠻⢀⠆⠀⢀⠈⢳⡸⡗⢤⡀⠉⠪⠙⢧⡀⠉⠙⠓⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠘⢧⡀⠀⠀⢀⡴⢂⣜⡥⠚⠁⠀⠀⠀⠁⢠⡆⢸⣰⠀⠈⣧⠀⠑⢿⠀⠙⢦⠀⠀⡀⠻⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠳⣤⣖⣋⡴⠟⠉⠀⠀⠀⠀⠀⠀⠀⢸⡇⢸⢹⠆⠀⢸⡄⠀⠈⢷⡀⠀⠑⢦⡜⢦⣹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠃⣼⣾⠀⠈⢀⢳⡀⢠⡈⢻⣦⡀⠀⠙⢮⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣇⡿⡼⠀⠄⠈⠈⣇⠀⠹⣌⠹⣿⣄⠳⢰⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡁⢻⠀⠀⠀⠀⠙⠂⠀⠹⡁⡈⡻⣦⠾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢧⣸⡄⠀⠀⠀⠀⠀⢰⣄⣃⡴⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠒⠒⠒⠒⠒⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
