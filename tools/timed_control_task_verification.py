import os
from datetime import datetime
times ={}
RUN_TIME = datetime().now().isoformat()
for line in open('output.log', 'r'):
  if 'CT' in line:
    control_task.split(':')
    control_task = line[line.index('-')+1,line.index(':')]
    if times[control_task]:
      times[control_task].append(int(line[line.inxex(":")+1]))
    else:
      times[control_task] = []
      times[control_task].append(int(line[line.inxex(":")+1]))
os.mkdir("logs" + datetime.isoformat())

for key in times.keys():
  os.mkdir(key + datetime.now().isoformat())
  