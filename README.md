# การใช้งาน 

ต้องติดตั้ง 

## ฝั่ง Server
1. Python 3
2. Flask
3. linebot
4. ngrok

## ฝั่ง Client
สิ่งที่ต้องการ
1. nodeMCU
2. Solid State Relay (SSR)
3. pm 2.5 sensor (sharp gp2y10)
4. STM32F4 discovery

## Programs (IDEs)
1. STM32Cube
2. Arduino IDE

## Instruction
1. run server
```
python3 testserver.py
```
2. run ngrok
```
ngrok http 80
```
3. connect LAN to client
4. enjoy!

### EMBEDDED SYS LAB
testserver.py -> Server หลัก ต้อง handle request+LINE API
Arduino Code -> Code ฝั่ง Client (NodeMCU)
