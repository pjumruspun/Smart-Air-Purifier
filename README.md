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
1. run python flask server
```
python3 testserver.py
```
2. run ngrok
```
ngrok http 80
```
3. run stm32

	3.1) เปิด STM Project `Lab2_03`
	
	3.2) Debug main.c
	
4. connect LAN to client
5. enjoy!

### EMBEDDED SYS LAB
testserver.py -> Server หลัก ต้อง handle request+LINE API
Lab2_03 -> Code ฝั่ง Sensor Input (STM32)
SSR_tester -> Code ฝั่ง Client (NodeMCU)
