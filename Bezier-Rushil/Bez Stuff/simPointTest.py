#Name: Rushil Sidhu
#Date: 3/8/20
from random import *
from array import *
import numpy as np
from random import randint
from timeit import default_timer
import tkinter as tk
from PIL import Image, ImageTk
import os
import math

##############CUSTOMIZABLE VARIABLES##################
velocity = 1 # pixel/second, the picture field is 5 pixels/real inch






##############CURRENTLY USELESS CODE##################   
def drawRobo(p1,p2,p3,p4,p5):
   canvas.create_line(p1,p2)
   canvas.create_line(p2,p3)
   canvas.create_line(p3,p4)
   canvas.create_line(p4,p5)
   canvas.create_line(p5,p1) # I can use this for a trace model later
   
##############CURRENTLY USELESS CODE##################   

##############DERIVATIVES####################
##############DERIVATIVES####################


############ROTATION OF ROBOT########################
def rad(deg):
   return deg*math.pi/180
   
def rotateX(xxx_todo_changeme,theta):
   (x,y) = xxx_todo_changeme
   global center
   return center[0] + (x-center[0])*math.cos(theta) + (y-center[1])*math.sin(theta)
   
def rotateY(xxx_todo_changeme1,theta):
   (x,y) = xxx_todo_changeme1
   return center[1] + (y-center[1])*math.cos(theta) - (x-center[0])*math.sin(theta)
   
def rotateBot(temp_theta):
  global p1,p2,p3,p4,p5,theta
  global center
  temp_theta = rad(temp_theta)
  theta = theta + temp_theta
  p1 = (rotateX(p1,temp_theta),rotateY(p1,temp_theta))
  p2 = (rotateX(p2,temp_theta),rotateY(p2,temp_theta))
  p3 = (rotateX(p3,temp_theta),rotateY(p3,temp_theta))
  p4 = (rotateX(p4,temp_theta),rotateY(p4,temp_theta))
  p5 = (rotateX(p5,temp_theta),rotateY(p5,temp_theta))
  
############ROTATION OF ROBOT########################

############MOVEMENT OF ROBOT########################
def forward(vel):
   global p1,p2,p3,p4,p5,theta
   p1 = (p1[0] - vel*math.sin(theta),p1[1] - vel*math.cos(theta))
   p2 = (p2[0] - vel*math.sin(theta),p2[1] - vel*math.cos(theta))
   p3 = (p3[0] - vel*math.sin(theta),p3[1] - vel*math.cos(theta))
   p4 = (p4[0] - vel*math.sin(theta),p4[1] - vel*math.cos(theta))
   p5 = (p5[0] - vel*math.sin(theta),p5[1] - vel*math.cos(theta))
   
############MOVEMENT OF ROBOT########################

############UPDATE COORDS AND THETA##################

def updatecoords():
   global center, robo,p1,p2,p3,p4,p5
   center = (p1[0] - .5*(p1[0]-p4[0]),p1[1] - .5*(p1[1]-p4[1]))
   canvas.coords(robo,p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],p4[0],p4[1],p5[0],p5[1])
   
############UPDATE COORDS AND THETA##################

############FOLLOW LINES#################
def drawLine(x1,y1,x2,y2):
  t = 0.0
  while t < 1.0:
      x = int((x1 + t * (x2 - x1)))
      y = int((y1 + t * (y2 - y1))) 
      canvas.create_line((x,y),(x+1,y+1),fill="green")
      t += 0.001
############FOLLOW LINES#################   
   
########BACKGROUND STUFF#############
root = tk.Tk()                         
Image_path="changeUPBackground.gif" 
image1 = tk.PhotoImage(file=Image_path)
canvas = tk.Canvas(root, width=720, height=720, bg='white')
canvas.create_image(360,360,image=image1)
canvas.pack()
########BACKGROUND STUFF#############

#########STARTING POINTS###########
# KEEP WIDTH 90 px or 80px
p1 = (205,620)
p2 = (245,605)
p3 = (285,620)
p4 = (285,705)
p5 = (205,705)
center = (p1[0] - .5*(p1[0]-p4[0]),p1[1] - .5*(p1[1]-p4[1]))
theta = 0
drawLine(p2[0],p2[1],p2[0]-100,p2[1]-100)
robo = canvas.create_polygon(p1,p2,p3,p4,p5, outline = "green")

#########STARTING POINTS###########


###########RUN METHOD################

def tick():
   #rotateBot(1)
   updatecoords()
   canvas.create_line(p2,(p2[0]+1,p2[1]+1),fill="red")
   #if(theta < 2*math.pi):
   #forward(1)
   canvas.after(10,tick)
   
###########RUN METHOD################
canvas.after(500,tick)
root.mainloop()
