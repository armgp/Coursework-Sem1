import csv
import matplotlib.pyplot as plt
from math import sin, cos, pi, sqrt

commands = []

with open('commands.csv', 'r') as csvFile:
	csvReader = csv.reader(csvFile)
	for line in csvReader:
		command = []
		dist = line[0]
		n = len(dist)
		d=float(dist[:n-2])
		if dist[n-2]=='c':
			d*=10
		command.append(d)
		command.append(line[1])
		commands.append(command)
csvFile.close()

prevX=0
prevY=0
xVals=[0]
yVals=[0]
for comm in commands:
	x=0
	y=0
	if(comm[1]=='N'):
		x = prevX
		y = prevY+comm[0]
	elif(comm[1]=='S'):
		x = prevX
		y = prevY-comm[0]
	elif(comm[1]=='E'):
		x = prevX+comm[0]
		y = prevY
	elif(comm[1]=='W'):
		x = prevX-comm[0]
		y = prevY
	elif(comm[1]=='NE'):
		x = prevX+comm[0]*cos(pi/4)
		y = prevY+comm[0]*sin(pi/4)
	elif(comm[1]=='NW'):
		x = prevX-comm[0]*cos(pi/4)
		y = prevY+comm[0]*sin(pi/4)
	elif(comm[1]=='SE'):
		x = prevX+comm[0]*cos(pi/4)
		y = prevY-comm[0]*sin(pi/4)
	elif(comm[1]=='SW'):
		x = prevX-comm[0]*cos(pi/4)
		y = prevY-comm[0]*sin(pi/4)
	xVals.append(x)
	yVals.append(y)
	prevX=x
	prevY=y
	
#2
if prevX>0 and prevY>0:
	print('LAST LOCATION IS NORTH-EAST OF STARTING LOCATION')
elif prevX<0 and prevY>0:
	print('LAST LOCATION IS NORTH-WEST OF STARTING LOCATION')
elif prevX<0 and prevY<0:
	print('LAST LOCATION IS SOUTH-WEST OF STARTING LOCATION')
elif prevX>0 and prevY<0:
	print('LAST LOCATION IS SOUTH-EAST OF STARTING LOCATION')
elif prevX==0 and prevY>0:
	print('LAST LOCATION IS NORTH OF STARTING LOCATION')
elif prevX==0 and prevY<0:
	print('LAST LOCATION IS SOUTH OF STARTING LOCATION')
elif prevX>0 and prevY==0:
	print('LAST LOCATION IS EAST OF STARTING LOCATION')
elif prevX<0 and prevY==0:
	print('LAST LOCATION IS WEST OF STARTING LOCATION')
elif prevX==0 and prevY==0:
	print('LAST LOCATION IS THE SAME AS THE STARTING LOCATION')
	
#3
s = [0,0]
e = [prevX,prevY]
distance = sqrt(x*x+y*y)
print('DISTANCE BETWEEN STARTING AND ENDING LOCATION = ', distance, 'milli-meters')

#1
plt.plot(xVals, yVals)
plt.xlabel('x - axis in millimeters')
plt.ylabel('y - axis in millimeters')
plt.title('PERSON P MOVEMENT MAP')
plt.show()


	
