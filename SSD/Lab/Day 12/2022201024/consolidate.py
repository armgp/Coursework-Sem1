import time 

file = open("Readings for 42x25 mat.txt").readlines()
rows = []
for line in file:
    rows.append(line.split('\t'))

def getMedian(t):
    obj = time.gmtime(0) 
    epoch = time.asctime(obj) 
    sum = 0
    count = 0
    for tm in t:
        count += 1
        sum += float(tm.split(":")[2])
    return sum/count


timestamps = []
mats = []
count = 0
t = []
m = []
for row in rows:
    if len(row) == 27:
        if(count == 42):
            timestamps.append(getMedian(t))
            mats.append(m)
            count = 0
            t = []
            m = []

        t.append(row[0])
        r = []
        for i in range (1, 26):
            r.append(int(row[i]))
        m.append(r)
        count+=1

timestamps.append(getMedian(t))
mats.append(m)

newMats = []
newMat = []
count = 0
for mat in mats:
    if(count == 3):
        newMats.append(newMat)
        newMat = []
    for row in mat:
        newMat.append(row)
    count+=1
newMats.append(newMat)

# divide into 35 boxes of 5x6
reducedMats = []
reducedData = []
for k in range(len(newMats)):
    mat = newMats[k]
    reducedMat = [[0]*5]*18
    for sectionNo in range (90):
        istart = int(int(sectionNo/5)*7)
        jstart = int((sectionNo*5)%25)
        i = int(istart/7)
        j = int(jstart/5)
        for ix in range(istart, istart+7):
            if(reducedMat[i][j] == 1): break
            for jx in range(jstart, jstart+5):
                if(mat[ix][jx] > 0):
                    reducedData.append([i, j, k, timestamps[k]])
                    reducedMat[i][j] = 1
                    break
    reducedMats.append(reducedMat)


steps = []
for data in reducedData:
    steps.append([data[0], data[1]])
stepMap = dict()
for data in reducedData:
    stepMap[str(data[0])+" "+str(data[1])] = steps.count([data[0], data[1]])

stepMap = sorted(stepMap.items(), key=lambda item: item[1])
n = len(stepMap)

s1 = stepMap[n-1][0].split(' ')
s2 = stepMap[n-2][0].split(' ')
s3 = stepMap[n-3][0].split(' ')
step1 = [int(s1[0]), int(s1[1])]
step2 = [int(s2[0]), int(s2[1])]
step3 = [int(s3[0]), int(s3[1])]

step1times = []
step2times = []
step3times = []
step1mat = 0
step2mat = 0
step3mat = 0
for data in reducedData:
    if(data[0] == step1[0] and data[1] == step1[1]): 
        step1times.append(data[3])
        step1mat = data[2]
    if(data[0] == step2[0] and data[1] == step2[1]): 
        step2times.append(data[3])
        step2mat = data[2]
    if(data[0] == step3[0] and data[1] == step3[1]): 
        step3times.append(data[3])
        step3mat = data[2]

print(step1times, step2times, step3times)

step1time = sum(step1times)/len(step1times)
step2time = sum(step2times)/len(step2times)
step3time = sum(step3times)/len(step3times)

if(len(steps) < 3):
    step3time = step2time
    step3 = step2

stride = 0
if(step1mat == step3mat):
    stride = (step3[1]-step1[1])*5
else:
    stride = ((5-step1[1])+step3[1])*5

strideVelocity = stride/(step3time-step1time)
cadence = (3*60)/(step3time-step1time)

print("Stride = ", stride)
print("Stride Velocity = ", strideVelocity)
print("Cadence = ", cadence)
