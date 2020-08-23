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
##############CUSTOMIZABLE VARIABLES##################

##############VARIABLES##################
isRotating = 0
isMoving = 0
##############VARIABLES##################
########################################################################### OK so ur idea is trace a parabola by using tangent line and then moving small amounts and correcting
                                                                          # Also you're going to use the length of the parabola to figure out if you're done or smtn like that, GL!
                                                                          # ALSO MAKE CODE LOOK NEATER LIKE NOT NESTED IF STATEMENTS. DONE!
                                                                          


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
   
def rotateX(xxx_todo_changeme,theta): # This is basically just applying a transformation matrix
   (x,y) = xxx_todo_changeme
   global center
   return center[0] + (x-center[0])*math.cos(theta) + (y-center[1])*math.sin(theta)
   
def rotateY(xxx_todo_changeme1,theta):
   (x,y) = xxx_todo_changeme1
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
  ####################################################################################################################################################################################
def rotateTo(temp_theta, rev):
   root.update_idletasks()
   root.update()
   time.sleep(0.01)
   global theta, ang_vel
   #print("TEST : ",temp_theta*math.pi/180)
   #print("TEST2 : ",theta)
   a = temp_theta*math.pi/180
   if(temp_theta*math.pi/180 > math.pi):
      a = 2*math.pi - temp_theta*math.pi/180
   ang_vel = 4#a * 0.8 + 1
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
   
def calcDistance(x1,y1,x2,y2):
   return math.sqrt( math.pow((x1 - x2),2) + math.pow((y1 - y2),2))

############MOVEMENT OF ROBOT########################

############UPDATE COORDS AND THETA##################

def updatecoords():
   global center, robo,p1,p2,p3,p4,theta
   center = (p1[0] - .5*(p1[0]-p3[0]),p1[1] - .5*(p1[1]-p3[1]))
   canvas.coords(robo,p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],p4[0],p4[1])
   canvas.coords(dir,center[0],center[1],p1[0] - .5*(p1[0]-p2[0])-20*math.sin(theta),p1[1] - .5*(p1[1]-p2[1])-20*math.cos(theta))
   if(theta >= 2*ymath.pi):
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
      
def bezX(xxx_todo_changeme2, xxx_todo_changeme3, xxx_todo_changeme4, xxx_todo_changeme5,t):
   (x1,y1) = xxx_todo_changeme2
   (x2,y2) = xxx_todo_changeme3
   (x3,y3) = xxx_todo_changeme4
   (x4,y4) = xxx_todo_changeme5
   return int(x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4)
   
def bezY(xxx_todo_changeme6, xxx_todo_changeme7, xxx_todo_changeme8, xxx_todo_changeme9,t):
   (x1,y1) = xxx_todo_changeme6
   (x2,y2) = xxx_todo_changeme7
   (x3,y3) = xxx_todo_changeme8
   (x4,y4) = xxx_todo_changeme9
   return int(y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4)
   
def drawBezier(xxx_todo_changeme10, xxx_todo_changeme11, xxx_todo_changeme12, xxx_todo_changeme13):
   (x1,y1) = xxx_todo_changeme10
   (x2,y2) = xxx_todo_changeme11
   (x3,y3) = xxx_todo_changeme12
   (x4,y4) = xxx_todo_changeme13
   t = 0.0
   while t < 1.0:
      x = int(x1*(1-t)*(1-t)*(1-t) + 3*t*x2*(1-t)*(1-t) + 3*t*t*x3*(1-t) + t*t*t*x4)
      y = int(y1*(1-t)*(1-t)*(1-t) + 3*t*y2*(1-t)*(1-t) + 3*t*t*y3*(1-t) + t*t*t*y4)
      create_circle((x,y),1,canvas, "red")
      t+= 0.001
def bezDevX(xxx_todo_changeme14, xxx_todo_changeme15, xxx_todo_changeme16, xxx_todo_changeme17, t):
   (x1,y1) = xxx_todo_changeme14
   (x2,y2) = xxx_todo_changeme15
   (x3,y3) = xxx_todo_changeme16
   (x4,y4) = xxx_todo_changeme17
   return int(-3*(1-t)*(1-t)*x1 + (3 - 12*t + 9*t*t)*x2 + (6*t - 9*t*t)*x3 + 3*t*t*x4)
   
def bezDevY(xxx_todo_changeme18, xxx_todo_changeme19, xxx_todo_changeme20, xxx_todo_changeme21, t):
   (x1,y1) = xxx_todo_changeme18
   (x2,y2) = xxx_todo_changeme19
   (x3,y3) = xxx_todo_changeme20
   (x4,y4) = xxx_todo_changeme21
   return int(-3*(1-t)*(1-t)*y1 + (3 - 12*t + 9*t*t)*y2 + (6*t - 9*t*t)*y3 + 3*t*t*y4)
   
def lineX(xxx_todo_changeme22,slope,t):
   (x,y) = xxx_todo_changeme22
   return x + (slope * t)
   
def lineY(xxx_todo_changeme23,slope,t):
   (x,y) = xxx_todo_changeme23
   return y + (slope * t)
   
def slopeLine(xxx_todo_changeme24,slopeX,slopeY,t):
   (x1,y1) = xxx_todo_changeme24
   canvas.create_line((x1,y1),(lineX((x1,y1),slopeX,t),lineY((x1,y1),slopeY,t)), fill = "pink")
   
def create_circle(xxx_todo_changeme25, r, canvasName, color): #center coordinates, radius
    (x, y) = xxx_todo_changeme25
    x0 = x - r
    y0 = y - r
    x1 = x + r
    y1 = y + r
    return canvasName.create_oval(x0, y0, x1, y1,outline=color,fill=color)
    #else:
      #return canvasName.create_oval(x0, y0, x1, y1,outline=color)
############FOLLOW LINES#################   

############TURN TO POINT###############
########################################################################################################################################################################################
def calcThetaToTurn(x,y):

   global center,theta
   if(x-center[0] == 0):
      if(y-center[1] > 0):
         return math.pi
      else:
         return 0
   print(" ")
   print(" ")
   print(" ")
   print(('center:',center))
   print(('x,y:',x,y))    
   print(("myTheta", theta)) 
   if(y <= center[1]):
      print((math.atan((x-center[0]) / (y- center[1]))))
      return math.atan((x-center[0]) / (y- center[1]))
   else:
      print(( math.pi + math.atan((x-center[0]) / (y - center[1]))))
      return math.pi + math.atan((x-center[0]) / (y - center[1]))

def turnTo(x,y):
   global theta
   root.update_idletasks()
   root.update()
   temp = calcThetaToTurn(x,y)
   print(("need to go", temp))
   if(temp == 0):
      return
   if(temp > 0):      #abs(temp) < 2*math.pi - abs(temp)):
      #print(temp*180/math.pi)
      while(theta < temp):
         rotateTo(temp*180/math.pi, False)
   else:
     #print("TEST")
     #print(temp*180/math.pi)
     while(theta > temp):
        rotateTo(temp*180/math.pi, True)
   

############TURN TO POINT###############
   
########BACKGROUND STUFF#############
root = tk.Tk()                         
Image_path="changeUPBackground.gif" 
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


#####################MAIN RUN STUFF#####################z


def run(event):
   create_circle((event.x,event.y),3,canvas,"red")
   turnTo(event.x,event.y)
   root.update_idletasks()
   root.update()
   time.sleep(0.01)
x = True

def update_x():
    global x
    x = False
canvas.bind("<Button 1>",run)
root.protocol("WM_DELETE_WINDOW", update_x)
f = Frame(root, height=32, width=128)
f.pack_propagate(0) # don't shrink
f.pack()
f.place(x=750,y=20)

b = Button(f, text="Activate Auton", command=run)
b.pack(fill=BOTH, expand=1)

while(x==True):
   root.update_idletasks()
   root.update()
   time.sleep(0.01)
   #canvas.after(1000)
   #run()
GOALS = [False,False,False,False]

#####################MAIN RUN STUFF#####################




   # global center, theta
#    robotX,robotY = center[0],center[1]
#    robotTheta = theta
#    radius = calcDist(x,y);
#    predictedX = radius*math.sin(robotTheta) + robotX;
#    predictedY = radius*math.cos(robotTheta) + robotY;
#    chord = calcDistance(predictedX, predictedY, x, y);
#    
#    angleError = 2*math.asin( (chord / 2) / (radius) );
#    
#    predictedX = radius*math.sin( math.fmod(angleError + robotTheta, 2*math.pi) ) + robotX;
#    predictedY = radius*math.cos( math.fmod(angleError + robotTheta, 2*math.pi) ) + robotY;
#    
#    if( (predictedX < x + 0.1) and (predictedX > x - 0.1) and (predictedY < y + 0.1) and (predictedY > y - 0.1) ):
#       return angleError;
#    else:
#       return angleError*-1;