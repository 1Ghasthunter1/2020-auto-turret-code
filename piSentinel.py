import cv2, time
import numpy as np
import serial

try:
    arduinoSerial = serial.Serial(port = "/dev/ttyUSB0", baudrate = 9600)
    time.sleep(3)
                            
except:
    print("No Serial found")
    exit()
    time.sleep(3)
                              
video=cv2.VideoCapture(0)


hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
 
 
video.set(cv2.CAP_PROP_FRAME_WIDTH, 400)
video.set(cv2.CAP_PROP_FRAME_HEIGHT,266)
video.set(cv2.CAP_PROP_FPS, 30)


while True:
    ret, frame = video.read()
    
    boxes, weights = hog.detectMultiScale(frame, winStride=(8,8) )
    if len(boxes) == 0:
        arduinoSerial.write("< , , 0>")
        
    boxes = np.array([[x, y, x + w, y + h] for (x, y, w, h) in boxes])

    for (xA, yA, xB, yB) in boxes:
        # display the detected boxes in the colour picture
        cv2.rectangle(frame, (xA, yA), (xB, yB),
                          (0, 255, 0), 2)
        centerX = str((xA+xB)/2)
        centerY = str((yA+yB)/2)
        solenoidState = "1"    #left this here, presumably because  if detectMultiScale returns None, then there will be no terms in boxes, and the for loop will not execute
        cv2.putText(frame, "Coords: " + centerX + ", " + centerY, (10, 200), cv2.FONT_HERSHEY_SIMPLEX, .5, (255, 0, 0), 3)
        serialData = "<"+centerX+", 250, "+solenoidState+">"
        arduinoSerial.write(serialData)
  
#Show frame
    cv2.imshow("Capture", frame)
    key=cv2.waitKey(1)

#Press Q will close the window
    if key & 0xFF== ord('q'):
        break

#switch off camera once q is pressed
video.release()
cv2.destroyAllWindows
