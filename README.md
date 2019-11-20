# การใช้งาน 

ต้องติดตั้ง 

1. Python 3
2. numpy
3. scipy
4. pygame

หากลง numpy กับ scipy ไม่เป็นโปรดใช้ Anaconda ช่วย 

รัน server

```
python server.py
```

สามารถทดลองยิง request ได้ผ่าน web browser ที่ http://localhost:5000/?key=10 เป็นต้น

### EMBEDDED SYS LAB
testserver.py -> Server หลัก ต้อง handle request+LINE API
Arduino Code -> Code ฝั่ง Client (NodeMCU)