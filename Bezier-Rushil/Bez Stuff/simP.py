#Name: Rushil Sidhu
#Date: 3/8/20
from random import *
from array import *
import numpy as np
from random import randint
from timeit import default_timer
import tkinter as tk
from tkinter import *
from PIL import Image, ImageTk
import os
import math
import time

##############CUSTOMIZABLE VARIABLES##################
vel = 6 # pixel/second, the picture field is 5 pixels/real inch
ang_vel = 1
minAngVel = 4
##############CUSTOMIZABLE VARIABLES##################

##############VARIABLES##################
#isRotating = 0
#isMoving = 0
##############VARIABLES##################
###########################################################################
                                                                          


##############CURRENTLY USELESS CODE##################   
def drawRobo(p1,p2,p3,p4):
   canvas.create_line(p1,p2)
   canvas.create_line(p2,p3)
   canvas.create_line(p3,p4)
   canvas.create_line(p4,p1) # I can use this for a trace model later
   
##############CURRENTLY USELESS CODE##################   

##############DERIVATIVES####################
##############DERIVATIVES####################


############ROTATION OF ROBOT########################
def rad(deg):
   return deg*math.pi/180
   
def rotateX(pos,theta): # This is basically just applying a transformation matrix
   (x,y) = pos
   global center
   return center[0] + (x-center[0])*math.cos(theta) + (y-center[1])*math.sin(theta)
   
def rotateY(pos,theta):
   (x,y) = pos
   return center[1] + (y-center[1])*math.cos(theta) - (x-center[0])*math.sin(theta)
   
def rotateBot(speed):
   global p1,p2,p3,p4,theta
   global center
   speed = rad(speed)
   theta = theta + speed
   p1 = (rotateX(p1,speed),rotateY(p1,speed))
   p2 = (rotateX(p2,speed),rotateY(p2,speed))
   p3 = (rotateX(p3,speed),rotateY(p3,speed))
   p4 = (rotateX(p4,speed),rotateY(p4,speed))
   updatecoords()
  
def rotateTo(temp_theta, rev):
   global minAngVel
   root.update_idletasks()
   root.update()
   time.sleep(0.01)
   global theta, ang_vel
   print(("TEST : ",temp_theta*math.pi/180))
   print(("TEST2 : ",theta))
   a = temp_theta*math.pi/180
   if(temp_theta*math.pi/180 > math.pi):
      a = 2*math.pi - temp_theta*math.pi/180
   ang_vel = a * 0.4 + minAngVel
   if(rev):
      rotateBot(-ang_vel)
   else:
      rotateBot(ang_vel)
   
############ROTATION OF ROBOT########################

############MOVEMENT OF ROBOT########################
def forward(vel):
   global p1,p2,p3,p4,theta
   p1 = (p1[0] - vel*math.sin(theta),p1[1] - vel*math.cos(theta))
   p2 = (p2[0] - vel*math.sin(theta),p2[1] - vel*math.cos(theta))
   p3 = (p3[0] - vel*math.sin(theta),p3[1] - vel*math.cos(theta))
   p4 = (p4[0] - vel*math.sin(theta),p4[1] - vel*math.cos(theta))
   updatecoords()
   
def drive(pixels):
   global center,robo,theta, vel
   #root.update_idletasks()
   #root.update()
   #time.sleep(0.01)
   goal = (center[0] - pixels*math.sin(theta), center[1] - pixels*math.cos(theta))
   if(pixels >= 0):
      while(center[0] - goal[0] > 1 or center[1] - goal[1] > 1):
         #vel = calcDist(goal[0],goal[1]) * 0.02 + 2 # dist * pK + min
         root.update_idletasks()
         root.update()
         time.sleep(0.01)
         forward(vel)
         updatecoords()
   else:
      while(center[0] - goal[0] > 1 or center[1] - goal[1] > 1):
         #vel = calcDist(goal[0],goal[1]) * 0.02 + 2 # dist * pK + min
         root.update_idletasks()
         root.update()
         time.sleep(0.01)
         forward(-vel)
      
def driveP(pixels):
   global center,robo,theta
   #root.update_idletasks()
   #root.update()
   #time.sleep(0.01)
   goal = (center[0] - pixels*math.sin(theta), center[1] - pixels*math.cos(theta))
   if(pixels >= 0):
      while(center[0] - goal[0] > 1 or center[1] - goal[1] > 1):
         vel = calcDist(goal[0],goal[1]) * 0.02 + 2 # dist * pK + min
         root.update_idletasks()
         root.update()
         time.sleep(0.01)
         forward(vel)
         updatecoords()
   else:
      while(center[0] - goal[0] > 1 or center[1] - goal[1] > 1):
         vel = calcDist(goal[0],goal[1]) * 0.02 + 2 # dist * pK + min
         root.update_idletasks()
         root.update()
         time.sleep(0.01)
         forward(-vel)
      
def calcDist(x,y):
   return math.sqrt( math.pow((center[0] - x),2) + math.pow((center[1] - y),2))

############MOVEMENT OF ROBOT########################

############UPDATE COORDS AND THETA##################

def updatecoords():
   global center, robo,p1,p2,p3,p4,theta
   center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
   canvas.coords(robo,p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],p4[0],p4[1])
   canvas.coords(dir,center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-20*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-20*math.cos(theta))
   if(theta >= 2*math.pi):
      theta = theta - 2*math.pi
   if(theta <= -2*math.pi):
      theta = theta + 2*math.pi
############UPDATE COORDS AND THETA##################

############FOLLOW LINES#################
def getLineX(x1,y1,x2,y2,t):
   return int((x1 + t * (x2 - x1)))
      
def getLineY(x1,y1,x2,y2,t):
   return int((y1 + t * (y2 - y1))) 
      
def drawLine(x1,y1,x2,y2):
   t = 0.0
   while t < 1.0:
      x = getLineX(x1,y1,x2,y2,t)
      y = getLineY(x1,y1,x2,y2,t)
      canvas.create_line((x,y),(x+1,y+1),fill="green")
      t += 0.001
      
def bezX(pos1, pos2, pos3, pos4,t):
   (x1,y1) = pos1
   (x2,y2) = pos2
   (x3,y3) = pos3
   (x4,y4) = pos4
   return int(x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4)
   
def bezY(pos1, pos2, pos3, pos4,t):
   (x1,y1) = pos1
   (x2,y2) = pos2
   (x3,y3) = pos3
   (x4,y4) = pos4
   return int(y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4)
   
def drawBezier(pos1, pos2, pos3, pos4):
   (x1,y1) = pos1
   (x2,y2) = pos2
   (x3,y3) = pos3
   (x4,y4) = pos4
   t = 0.0
   while t < 1.0:
      x = int(x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4)
      y = int(y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4)
      create_circle((x,y),1,canvas, "red")
      t+= 0.001
      
#def bezDev((x1,y1),(x2,y2),(x3,y3),(x4,y4), t):
   #t = 0.0
   #while t < 1.0:
     # x = int(-3*(1-t)*(1-t)*x1 + (3 - 12*t + 9*t*t)*x2 + (6*t - 9*t*t)*x3 + 3*t*t*x4)
    #  y = int(-3*(1-t)*(1-t)*y1 + (3 - 12*t + 9*t*t)*y2 + (6*t - 9*t*t)*y3 + 3*t*t*y4)
    #  print(x+bezX((x1,y1),(x2,y2),(x3,y3),(x4,y4),.1))
    #  print(bezY((x1,y1),(x2,y2),(x3,y3),(x4,y4),.1))
   #   create_circle((x+bezX((x1,y1),(x2,y2),(x3,y3),(x4,y4),.1),y+bezY((x1,y1),(x2,y2),(x3,y3),(x4,y4),.1)),1,canvas, "green")
  #    t+= 0.001
 
def bezDevX(pos1, pos2, pos3, pos4, t):
   (x1,y1) = pos1
   (x2,y2) = pos2
   (x3,y3) = pos3
   (x4,y4) = pos4
   return int(-3*(1-t)*(1-t)*x1 + (3 - 12*t + 9*t*t)*x2 + (6*t - 9*t*t)*x3 + 3*t*t*x4)
   
def bezDevY(pos1, pos2, pos3, pos4, t):
   (x1,y1) = pos1
   (x2,y2) = pos2
   (x3,y3) = pos3
   (x4,y4) = pos4
   return int(-3*(1-t)*(1-t)*y1 + (3 - 12*t + 9*t*t)*y2 + (6*t - 9*t*t)*y3 + 3*t*t*y4)
   
def lineX(pos,slope,t):
   (x,y) = pos
   return x + (slope * t)
   
def lineY(pos,slope,t):
   (x,y) = pos
   return y + (slope * t)
   
def slopeLine(pos,slopeX,slopeY,t):
   (x1,y1) = pos
   canvas.create_line((x1,y1),(lineX((x1,y1),slopeX,t),lineY((x1,y1),slopeY,t)), fill = "pink")
   
def create_circle(pos, r, canvasName, color): #center coordinates, radius
    (x, y) = pos
    x0 = x - r
    y0 = y - r
    x1 = x + r
    y1 = y + r
    return canvasName.create_oval(x0, y0, x1, y1,outline=color,fill=color)
    #else:
      #return canvasName.create_oval(x0, y0, x1, y1,outline=color)
############FOLLOW LINES#################   

############TURN TO POINT###############
def calcThetaToTurn(x,y):
   global center,theta
   if(x-center[0] == 0):
      if(y-center[1] > 0):
         return math.pi
      else:
         return 0
         
   if(y <= center[1]):
      return math.atan((x-center[0]) / (y- center[1]))
   else:
      return math.pi + math.atan((x-center[0]) / (y - center[1]))

def turnTo(x,y):
   global theta
   root.update_idletasks()
   root.update()
   temp = calcThetaToTurn(x,y)
   if(temp == 0):
      return
   if(temp < math.pi):      #abs(temp) < 2*math.pi - abs(temp)):
      print((temp*180/math.pi))
      while(theta < temp):
         rotateTo(temp*180/math.pi, False)
   else:
     print("TEST")
     print((temp*180/math.pi))
     while(theta + 2*math.pi > temp):
        rotateTo(temp*180/math.pi, True)
   

############TURN TO POINT###############
   
########BACKGROUND STUFF#############
root = tk.Tk()                         
Image_path="changeUpBackground.gif" 
image1 = tk.PhotoImage(file=Image_path)
canvas = tk.Canvas(root, width=900, height=720, bg='white')
canvas.create_image(360,360,image=image1)
canvas.pack()
########BACKGROUND STUFF#############

#########STARTING POINTS###########
# KEEP WIDTH 90 px or 80px
p1 = (205,620)
p2 = (285,620)
p3 = (285,705)
p4 = (205,705)
center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
theta = 0
robo = canvas.create_polygon(p1,p2,p3,p4, outline = "green")
drawLine(center[0],center[1],center[0]-100,center[1]-100)
dir = canvas.create_line(center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-50*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-50*math.cos(theta), fill = "red", width = 2)


#########STARTING POINTS###########


###########RUN METHOD################
temp = (center[0],center[1])
#rotateBot(-90)
#forward(70)
#rotateBot(90)
#forward(20)
#rotateBot(-30)
temp1 = calcThetaToTurn(temp[0],temp[1])


rot = 0# 
if(theta - temp1 > temp1 - theta):
   rot = 1
else:
   rot = -1
   #temp1 =-2*math.pi + temp1
  
#####################MAIN RUN STUFF#####################
#drawBezier((center[0],center[1]),(center[0],center[1]-200),(center[0]-150,center[1]-200),(center[0]-150,center[1]-400))
#bezDev((center[0],center[1]),(center[0],center[1]-200),(center[0]-150,center[1]-200),(center[0]-150,center[1]-400))

temppp = (center[0],center[1]),(center[0],center[1]-500),(center[0]-150,center[1]-500),(center[0]-150,center[1]-400)
drawBezier(temppp[0],temppp[1],temppp[2],temppp[3])


def followBez(p0, p1, p2, p3,drawBez,traceBot):
   (p0x,p0y) = p0
   (p1x,p1y) = p1
   (p2x,p2y) = p2
   (p3x,p3y) = p3
   global vel
   if(drawBez):
      drawBezier((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y))
   t = 0
   while( t < 1):
     s = 0.05
     a = lineX((bezX((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),\
            bezY((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t)),\
            bezDevX((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),s)
            
     b = lineY((bezX((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),\
            bezY((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t)),\
            bezDevY((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),s)
     if(traceBot):
         canvas.create_line((bezX((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),\
            bezY((p0x,p0y),(p1x,p1y),(p2x,p2y),(p3x,p3y),t),a,b), fill = "pink", width=3)
     #print(a,b)
     #print(center[0],center[1])
     turnTo(a,b)
     drive(calcDist(a,b))
     t+=0.05
     vel = (1-t) * 12 + .5
     updatecoords()
     time.sleep(0.01)
   
def run():
   global vel
   followBez(temppp[0],temppp[1],temppp[2],temppp[3],False,True)
   print(theta)
   print(center)
   #turnTo(360,720)
   #drive(calcDist(temp[0],temp[1]))
      
      ##################################### OK WELL NOW JUST ADD BEZIER AND THE DERIVATIVE MOVEMENT AND U CAN FLEX ON EVERYONE
      ##################################### SIM DUMB TESTS HAS THE MOUSE TURN CODE, TURNS ARE WACK, FIX AND THEN UR OP
x = True

def update_x():
    global x
    x = False
def reset():
   global p1,p2,p3,p4, center, theta, robo, drawLine, dir, vel
   p1 = (205,620)
   p2 = (285,620)
   p3 = (285,705)
   p4 = (205,705)
   vel = 6
   center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
   theta = 0
   updatecoords()
   #robo = canvas.create_polygon(p1,p2,p3,p4, outline = "green")
   #drawLine(center[0],center[1],center[0]-100,center[1]-100)
   #dir = canvas.create_line(center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-50*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-50*math.cos(theta), fill = "red", width = 2)
#canvas.bind("<Button 1>",run)
root.protocol("WM_DELETE_WINDOW", update_x)
f = Frame(root, height=32, width=128)
f.pack_propagate(0) # don't shrink
f.pack()
f.place(x=750,y=20)

b = Button(f, text="Activate Auton", command=run)
b.pack(fill=BOTH, expand=1)

f = Frame(root, height=32, width=128)
f.pack_propagate(0) # don't shrink
f.pack()
f.place(x=750,y=100)

b = Button(f, text="Reset", command=reset)
b.pack(fill=BOTH, expand=1)

while(x==True):
   root.update_idletasks()
   root.update()
   time.sleep(0.01)
   #canvas.after(1000)
   #run()
GOALS = [False,False,False,False]

#####################MAIN RUN STUFF#####################











































#def tick():
   # global isRotating,isMoving,vel,ang_vel, temp
#    #############DECISIONS################
#    if(isRotating == 1):
#       rotateBot(ang_vel)
#    elif(isRotating == -1):
#       rotateBot(-ang_vel)
#    
#    if(isMoving == 1):
#       forward(vel)
#    elif(isMoving == -1):
#       forward(-vel)
#    #############DECISIONS################   
#    
#    temp1 = calcThetaToTurn(temp[0],temp[1])
#    if(theta-temp1 <= temp1-theta):
#       isRotating = 1
#    else:
#       isRotating = -1
#    if(not GOALS[0]):
#       ang_vel = 1
#       if(theta >= temp1):
#          GOALS[0] = True
#       canvas.after(10,tick)
#    else:
#       canvas.create_line((0,0),(720,720))
#       if(not GOALS[1]):
#          isMoving = 1
#          #isRotating=0
#          if(abs(center[0] - temp[0]) < 2 and abs(center[1] - temp[1]) < 2):
#             GOALS[1] = True
#          canvas.after(10,tick)
#       else:
#          print("yay")
#          isMoving = 0
#          temp4 = (temp[0]-100,temp[1]-100)
#          temp5 = calcThetaToTurn(temp4[0],temp4[1])
#          if(not GOALS[2]):
#             if(theta <= temp5):
#                GOALS[2] = True
#             isRotating = -1
#             ang_vel = 1
#             canvas.after(10,tick)
#          else:
#             print(isMoving)
#             isRotating = 0
#             isMoving = 0
#             if(not GOALS[3]):
#                isMoving = 1
#                if(abs(center[0] - temp4[0]) < 7 and abs(center[1] - temp4[1]) < 7):
#                   GOALS[3] = True
#                canvas.after(10,tick)
#             else:
#                isMoving = 0
#                isRotating = 1
#                ang_vel = 10
#                canvas.after(10,tick)
      
###########RUN METHOD################
#canvas.after(500,tick)
#root.mainloop()
# 
# while(not GOALS[3]):
#    print(theta)
#    root.update_idletasks()
#    root.update()
#    time.sleep(0.01)
#    temp1 = calcThetaToTurn(temp[0],temp[1])
#    rotateTo(temp1)