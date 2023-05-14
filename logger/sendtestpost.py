import requests
import time

times = range(50)

for t in times:
  time.sleep(1)
  data = {"time": t, "current.in_sun": t, "temp.in_sun": 2*t+1}
  response = requests.post("http://127.0.0.1:8050/add-data", json=data)
for t in times:
  time.sleep(1)
  data = {"time": t+50, "current.in_sun": t+50, "temp.in_sun": 2*(t+50)+1, "alpha.state": 4}
  response = requests.post("http://127.0.0.1:8050/add-data", json=data)
